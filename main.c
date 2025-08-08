
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:11:21 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:11:21 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int only_digits(char *str)
{
    int i;

	i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}
/// @ef ///////////////////////////////////////////////////////////
/// param sim 

static void set_start_time(t_sim *sim)
{
    if (!sim)
        return ;
    sim->start_time = get_time_in_ms() + (sim->rules.count * 2 * 10);
}

static int create_philos(t_sim *sim)
{
    int i;

	i = 0;
    if (!sim || !sim->philos)
        return (0);
    while (i < sim->rules.count)
    {
        if (pthread_create(&sim->philos[i]->thread, NULL, ft_philo,
							sim->philos[i]) != 0)
            return (0);
		i++;
    }
    return (1);
}

static int create_thread(t_sim *sim)
{
    if (!sim)
        return (0);
    if (sim->rules.count <= 1)
        return (1);
    if (pthread_create(&sim->monitor, NULL, controller_loop, sim) != 0)
        return (0);
    return (1);
}

static int start_simulation(t_sim *sim)
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
/// @ef ///////////////////////////////////////////////////////////
/// param sim 
static void stop_simulation(t_sim *sim)
{
    int i;

	i = 0;
    while (i < sim->rules.count)
    {
        pthread_join(sim->philos[i]->thread, NULL);
        i++;
    }
    if (sim->rules.count > 1)
        pthread_join(sim->monitor, NULL);
    destroy_mutexes(sim);
    free_sim(sim);
}

static int run_simulation(int argc, char **argv)
{
    t_sim *sim;

    sim = init_sim(argc, argv, 1);
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

int main(int argc, char **argv)
{
    int i;

    i = 1;
    if (argc != 5 && argc != 6)
        return (print_error("Error: wrong number of arguments\n"));
    while (i < argc)
    {
        if (!only_digits(argv[i]))
            return (print_error("Error: arguments must be positive numbers\n"));
        if (ft_atoi(argv[i]) == -1)
            return (print_error("Error: invalid or too large argument\n"));
        i++;
    }
    if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > 200 || ft_atoi(argv[2]) <= 0
        || ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0
        || (argc == 6 && ft_atoi(argv[5]) <= 0))
        return (print_error("Error: arguments must be positive and within limits\n"));
    return (run_simulation(argc, argv));
}
