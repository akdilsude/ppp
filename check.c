/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:10:05 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/08 22:19:50 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t ft_strlen(const char *s)
{
    int i;
    
    i = 0;
    while (s[i])
        i++;
    return i;
}

int ft_atoi(const char *str)
{
    int i;
    int res;
    
    i = 0;
    res = 0;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;
    if (str[i] == '-' || str[i] == '+')
        return (-1);
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = (res * 10) + (str[i] - '0');
        i++;
    }
    if (res > INT_MAX)
    {
        return (-1);
    }
    return (res);
}
