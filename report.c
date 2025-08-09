
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:12:21 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 14:12:21 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

char	*ft_format_status(t_philo *philo, char *str)
{
	char	*buffer;
	char	time_str[32];

	sprintf(time_str, "%ld %d %s\n", get_time_in_ms() - philo->sim->start_time,
			philo->id + 1, str);
	buffer = malloc(ft_strlen(time_str) + 1);
	if (!buffer)
		return (NULL);
	sprintf(buffer, "%s", time_str);
	return (buffer);
}

void	ft_write_status(t_philo *philo, char *str)
{
	char	*formatted;

	formatted = ft_format_status(philo, str);
	pthread_mutex_lock(&philo->sim->control.print_lock);
	if (is_stopped(philo->sim))
	{
		pthread_mutex_unlock(&philo->sim->control.print_lock);
		return;
	}
	if (formatted)
	{
		printf("%s", formatted);
		free(formatted);
	}
	pthread_mutex_unlock(&philo->sim->control.print_lock);
}
