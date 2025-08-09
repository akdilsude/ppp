/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:11:21 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 13:38:22 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
		return (print_error("Error: wrong number of arguments\n"));
	while (i < argc)
	{
		if (!is_digit(argv[i]))
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
