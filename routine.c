
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:13:17 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:13:17 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void ft_eat_sleep_routine(t_philo *philo)
{
    int fork1;
    int fork2;

    fork1 = philo->id;
    fork2 = (philo->id + 1) % philo->sim->rules.count;
    if (philo->id % 2)
    {
        fork1 = (philo->id + 1) % philo->sim->rules.count;
        fork2 = philo->id;
    }
    pthread_mutex_lock(&philo->sim->forks.forks[fork1]);
    ft_write_status(philo, "has taken a fork");
    pthread_mutex_lock(&philo->sim->forks.forks[fork2]);
    ft_write_status(philo, "has taken a fork");
    ft_write_status(philo, "is eating");
    pthread_mutex_lock(&philo->eat_lock);
    philo->last_meal = get_time_in_ms();
    pthread_mutex_unlock(&philo->eat_lock);
    ft_philo_sleep(philo->sim, philo->sim->rules.eat_time);
    if (!is_stopped(philo->sim))
    {
        pthread_mutex_lock(&philo->eat_lock);
        philo->eat_count += 1;
        pthread_mutex_unlock(&philo->eat_lock);
    }
    ft_write_status(philo, "is sleeping");
    pthread_mutex_unlock(&philo->sim->forks.forks[fork2]);
    pthread_mutex_unlock(&philo->sim->forks.forks[fork1]);
    ft_philo_sleep(philo->sim, philo->sim->rules.sleep_time);
}

static void ft_think_routine(t_philo *philo, bool silent)
{
    time_t time_to_think;

    pthread_mutex_lock(&philo->eat_lock);
    time_to_think = (philo->sim->rules.die_time
                     - (get_time_in_ms() - philo->last_meal)
                     - philo->sim->rules.eat_time) / 2;
    pthread_mutex_unlock(&philo->eat_lock);
    if (time_to_think < 0)
        time_to_think = 0;
    if (time_to_think == 0 && silent)
        time_to_think = 1;
    if (time_to_think > 600)
        time_to_think = 200;
    if (!silent)
        ft_write_status(philo, "is thinking");
    ft_philo_sleep(philo->sim, time_to_think);
}

static void *ft_alone_philo_routine(t_philo *philo)
{
    int fork1;
    
    fork1 = philo->id;
    pthread_mutex_lock(&philo->sim->forks.forks[fork1]);
    ft_write_status(philo, "has taken a fork");
    ft_philo_sleep(philo->sim, philo->sim->rules.die_time);
    ft_write_status(philo, "died");
    pthread_mutex_unlock(&philo->sim->forks.forks[fork1]);
    return (NULL);
}

void *ft_philo(void *data)
{
    t_philo *philo;
    
    philo = (t_philo *)data;
    if (philo->sim->rules.must_eat == 0)
        return (NULL);
    pthread_mutex_lock(&philo->eat_lock);
    philo->last_meal = philo->sim->start_time;
    pthread_mutex_unlock(&philo->eat_lock);
    ft_routine_start_delay(philo->sim->start_time);
    if (philo->sim->rules.die_time == 0)
        return (NULL);
    if (philo->sim->rules.count == 1)
        return ft_alone_philo_routine(philo);
    else if (philo->id % 2)
        ft_think_routine(philo, true);
    while (!is_stopped(philo->sim))
    {
        ft_eat_sleep_routine(philo);
        ft_think_routine(philo, false);
    }
    return (NULL);
}
