
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:25:04 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:11:07 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct	s_rules
{
	int				count;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	int				must_eat;
	bool			has_limit;
}	t_rules;

typedef struct	s_forks
{
	pthread_mutex_t	*forks;
}	t_forks;

typedef struct	s_control
{
	bool			stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
}	t_control;

typedef struct	s_philo t_philo;

typedef struct	s_sim
{
	t_rules			rules;
	t_control		control;
	t_forks			forks;
	t_philo			**philos;
	long			start_time;
	pthread_t		monitor;
}	t_sim;

struct	s_philo
{
	int				id;
	int				eat_count;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	eat_lock;
	t_sim			*sim;
};

int		run_simulation(int argc, char **argv);
int		is_digit(char *str);
void	cleanup_philos(t_sim *sim, int n);
void	stop_simulation(t_sim *sim);



















int		ft_atoi(const char *str);
size_t	ft_strlen(const char *s);

t_sim	*init_sim(int argc, char **argv);
char	*ft_format_status(t_philo *philo, char *str);

void	*philo_main(void *data);

time_t	get_time_in_ms(void);
void	ft_philo_sleep(t_sim *sim, time_t sleep_time);
void	wait_until_start(time_t start_time);

void	ft_write_status(t_philo *philo, char *str);
int		print_error(char *msg);

void	*controller_loop(void *data);
bool	is_stopped(t_sim *sim);

void	free_sim(t_sim *sim);

#endif