/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:44:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 13:32:39 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_start_time(t_sim *sim)
{
	if (!sim)
		return ;
	sim->start_time = get_time_in_ms() + (sim->rules.count * 2 * 10);
}

static int	create_philos(t_sim *sim)
{
	int	i;

	i = 0;
	if (!sim || !sim->philos)
		return (0);
	while (i < sim->rules.count)
	{
		if (pthread_create(&sim->philos[i]->thread, NULL, philo_main,
						sim->philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	create_thread(t_sim *sim)
{
	if (!sim)
		return (0);
	if (sim->rules.count <= 1)
		return (1);
	if (pthread_create(&sim->monitor, NULL, controller_loop, sim) != 0)
		return (0);
	return (1);
}

static int	start_simulation(t_sim *sim)
{
	if (!sim)
		return (print_error("Error: sim is NULL\n"), 0);
	set_start_time(sim);
	if (!create_philos(sim))
		return (print_error("Error: failed to create philosopher threads\n"), false);
	if (!create_thread(sim))
    	return (print_error("Error: failed to create monitor thread\n"), false);
	return (1);
}

int	run_simulation(int argc, char **argv)
{
	t_sim	*sim;

	sim = init_sim(argc, argv);
	if (!sim)
		return (print_error("Error: simulation initialization failed\n"), 1);
	if (!start_simulation(sim))
	{
		free_sim(sim);
		return (print_error("Error: thread creation failed\n"), 1);
	}
	stop_simulation(sim);
	return (0);
}
