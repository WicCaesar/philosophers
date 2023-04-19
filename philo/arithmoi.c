/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmoi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 01:54:08 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/19 02:55:28 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

// Simply checks if every argument is a valid number. Allows the plus sign.
int	validation(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if (argc < 5 || argc > 6)
		return (0);
	while (i < argc)
	{
		j = 0;
		if (!(argv[i][j] == '+' || (ft_isdigit(argv[i][j]))))
			return (0);
		if (argv[i][0] == '+')
			j++;
		while (argv[i][j])
		{
			if (!(ft_isdigit(argv[i][j])))
				return (0);
			j++;
		}
		if (ft_atoi(argv[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

// 9 (TAB) \t horizontal tab
// 10 (LF) \n line feed
// 11 (VT) \v vertical tab
// 12 (FF) \f form feed
// 13 (CR) \r carriage return
int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_operator(const char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
		return (-1);
	return (1);
}

// Converts a numeric character in a string to an integer by subtracting 0 from
// the corresponding ASCII decimal value to get a valid numerical value, then
// multiplies by 10 to calculate the following digit. It skips spaces and tabs.
int	ft_atoi(const char *str)
{
	int		i;
	long	number;

	i = 0;
	number = 0;
	while (ft_isspace(str[i]) || str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] != '\0' && ft_isdigit(str[i]))
	{
		number = number * 10 + (str[i] - '0');
		i++;
	}
	return (number * (ft_operator(str)));
}
