
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:13:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:13:34 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	set_rules(t_rules *rules, int argc, char **argv)
{
	rules->count = ft_atoi(argv[0]);
	if (rules->count == -1)
		return (print_error("Error: invalid philosopher count\n"), 0);
	rules->die_time = ft_atoi(argv[1]);
	if (rules->die_time == -1)
		return (print_error("Error: invalid time to die\n"), 0);
	rules->eat_time = ft_atoi(argv[2]);
	if (rules->eat_time == -1)
		return (print_error("Error: invalid time to eat\n"), 0);
	rules->sleep_time = ft_atoi(argv[3]);
	if (rules->sleep_time == -1)
		return (print_error("Error: invalid time to sleep\n"), 0);
	rules->must_eat = -1;
	rules->has_limit = false;
	if (argc == 6)
	{
		rules->must_eat = ft_atoi(argv[4]);
		if (rules->must_eat == -1)
			return (print_error("Error: invalid meal limit\n"), 0);
		rules->has_limit = true;
	}
	return (1);
}

static int	init_forks(t_sim *sim)
{
	int	i;

	i = 0;
	sim->forks.forks = malloc(sizeof(pthread_mutex_t) * sim->rules.count);
	if (!sim->forks.forks)
		return (print_error("Error: memory allocation for forks failed\n"), 0);
	while (i < sim->rules.count)
	{
		if (pthread_mutex_init(&sim->forks.forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&sim->forks.forks[i]);
			}
			free(sim->forks.forks);
			sim->forks.forks = NULL;
			return (print_error("Error: fork mutex initialization failed\n"), 0);
		}
		i++;
	}
	return (1);
}

static int	init_philos(t_sim *sim)
{
	int	i;

	i = 0;
	sim->philos = malloc(sizeof(*sim->philos) * sim->rules.count);
	if (!sim->philos)
		return (print_error("Error: malloc philosophers\n"), 0);
	while (i < sim->rules.count)
	{
		sim->philos[i] = malloc(sizeof(*sim->philos[i]));
		if (!sim->philos[i]
			|| pthread_mutex_init(&sim->philos[i]->eat_lock, NULL))
			break;
		sim->philos[i]->id = i;
    	sim->philos[i]->sim = sim;
    	sim->philos[i]->last_meal = 0;
    	sim->philos[i]->eat_count = 0;
    	i++;
	}
	if (i < sim->rules.count)
	{
		cleanup_philos(sim, i);
		print_error("Error: init philosophers\n");
		return (0);
	}
	return (1);
}

static int	init_mutex(t_sim *sim)
{
	if (pthread_mutex_init(&sim->control.stop_lock, NULL) != 0)
	{
		free_sim(sim);
		return (print_error("Error: stop mutex initialization failed\n"), 0);
	}
	if (pthread_mutex_init(&sim->control.print_lock, NULL) != 0)
	{
		free_sim(sim);
		return (print_error("Error: print mutex initialization failed\n"), 0);
	}
	return (1);
}

t_sim	*init_sim(int argc, char **argv)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (print_error("Error: memory allocation for simulation failed\n"), NULL);
	if (!set_rules(&sim->rules, argc, &argv[1]))
	{
		free(sim);
		return (NULL);
	}
	if (!init_forks(sim) || !init_philos(sim))
	{
		free(sim);
		return (NULL);
	}
	if (!init_mutex(sim))
		return (NULL);
	sim->control.stop = false;
	return (sim);
}
