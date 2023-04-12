/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kyklos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 18:09:22 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/12 05:03:54 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophos.h"

void	*lunch(t_philosophers *philosopher)
{
	//! check if philosopher has two forks (mutexes);
	//printf("%d %d is eating\n", gettimeofday(tv_sec, NULL), philosopher);
	printf("%ld is eating\n", philosopher->id); // ! add timer
	philosopher->meals++;
	//! check if dead;
	return (NULL);
}

void	*think(t_philosophers *philosopher)
{
	printf("%ld is thinking\n", philosopher->id); // ! add timer
	return (NULL);
}

void	*sleep(t_philosophers *philosopher)
{
	printf("%ld is sleeping\n", philosopher->id); // ! add timer
	return (NULL);
}
