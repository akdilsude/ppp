
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:11:46 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:11:46 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_stop_flag(t_sim *sim, bool state)
{
	pthread_mutex_lock(&sim->control.stop_lock);
	sim->control.stop = state;
	pthread_mutex_unlock(&sim->control.stop_lock);
}

bool	is_stopped(t_sim *sim)
{
	bool	flag;

	pthread_mutex_lock(&sim->control.stop_lock);
	flag = sim->control.stop;
	pthread_mutex_unlock(&sim->control.stop_lock);
	return (flag);
}

static int	detect_death(t_philo *p)
{
	unsigned long	now;
	char			*msg;

	if (!p || !p->sim)
		return (0);
	now = get_time_in_ms();
	if (now - p->last_meal >= (unsigned long)p->sim->rules.die_time)
	{
		pthread_mutex_lock(&p->sim->control.print_lock);
		pthread_mutex_unlock(&p->eat_lock);
		msg = ft_format_status(p, "died");
		if (msg)
		{
			printf("%s", msg);
			free(msg);
		}
		set_stop_flag(p->sim, true);
		pthread_mutex_unlock(&p->sim->control.print_lock);
		return (1);
	}
	return (0);
}

static int	evaluate_stop_conditions(t_sim *sim)
{
	int		idx;
	bool	everyone_ate;

	idx = 0;
	everyone_ate = sim->rules.has_limit;
	while (idx < sim->rules.count)
	{
		pthread_mutex_lock(&sim->philos[idx]->eat_lock);
		if (detect_death(sim->philos[idx]))
			return (1);
		if (sim->rules.has_limit
			&& sim->philos[idx]->eat_count < sim->rules.must_eat)
			everyone_ate = false;
		pthread_mutex_unlock(&sim->philos[idx]->eat_lock);
		idx++;
	}
	if (sim->rules.has_limit && everyone_ate)
	{
		set_stop_flag(sim, true);
		return (1);
    }
	return (0);
}

void	*controller_loop(void *arg)
{
	t_sim	*sim ;

	sim = (t_sim *)arg;
	if (sim->rules.must_eat == 0)
		return (NULL);
	set_stop_flag(sim, false);
	wait_until_start(sim->start_time);
	while (!is_stopped(sim))
	{
		if (evaluate_stop_conditions(sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}
