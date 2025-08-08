/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:10:53 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 22:26:26 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void free_sim(t_sim *sim)
{
    int i;

    i = 0;
    if (!sim)
        return;
    if (sim->forks.forks)
        free(sim->forks.forks);
    if (sim->philos)
    {
        while (i < sim->rules.count)
        {
            if (sim->philos[i])
                free(sim->philos[i]);
            i++;
        }
        free(sim->philos);
    }
    free(sim);
}

void destroy_mutexes(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->rules.count)
    {
        pthread_mutex_destroy(&sim->forks.forks[i]);
        pthread_mutex_destroy(&sim->philos[i]->eat_lock);
        i++;
    }
    pthread_mutex_destroy(&sim->control.print_lock);
    pthread_mutex_destroy(&sim->control.stop_lock);
}

int print_error(char *msg)
{
    write(2, msg, ft_strlen(msg));
    return (1);
}
