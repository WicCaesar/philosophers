/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronos.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 07:37:17 by cnascime          #+#    #+#             */
/*   Updated: 2023/05/23 19:34:35 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"
#include <stdint.h>

// Converts the time in seconds and microseconds (usec) to miliseconds.
long int	ms(void)
{
	struct timeval	time;
	long int		miliseconds;

	gettimeofday(&time, NULL);
	miliseconds = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (miliseconds);
}

void	siesta(t_journal *philosopher, long timer)
{
	long int	nap;

	nap = ms() + timer;
	while (nap > ms())
	{
		if (takepulse(philosopher) == FAILURE)
			break ;
		usleep(500);
	}
}

// If the philosopher died in the meantime, no false information is spread.
// Otherwise, locks the mutex, prints the message, then unlocks the mutex.
void	inform(t_journal *philosopher, char *message)
{
	if (takepulse(philosopher) == FAILURE)
		return ;
	pthread_mutex_lock(&philosopher->mutexes->printmutex);
	printf(message, ms() - philosopher->birth, philosopher->philosopher);
	pthread_mutex_unlock(&philosopher->mutexes->printmutex);
}
// It is necessary to lock the mutex so the messages don't overlap.

// Takes the pulse of a philosopher, id est, if the thread exceeded its lifespan
// or if they can't take anymore for other reasons (full belly or starvation).
// If the philosopher is dead, informs and returns 0 (there is no pulse).
// If the philosopher is not dead, returns 1 (there is a pulse).
int	takepulse(t_journal *philosopher)
{
	long int	id;

	id = (long int)philosopher->philosopher;
	pthread_mutex_lock(&philosopher->mutexes->pulsemutex);
	if (philosopher->mutexes->cantanymore)
	{
		pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
		return (FAILURE);
	}
	if (philosopher->death <= ms())
	{
		philosopher->mutexes->cantanymore = TRUE;
		pthread_mutex_lock(&philosopher->mutexes->printmutex);
		printf(DIED, ms() - philosopher->birth, id);
		pthread_mutex_unlock(&philosopher->mutexes->printmutex);
		pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
		return (FAILURE);
	}
	pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
	return (SUCCESS);
}

// Destroy mutexes and frees allocations made on the philosophers' struct.
void	unloadmutexes(t_mutexes *mutexes, t_journal *philosopher)
{
	pthread_mutex_destroy(&mutexes->printmutex);
	pthread_mutex_destroy(&mutexes->bellymutex);
	pthread_mutex_destroy(&mutexes->pulsemutex);
	pthread_mutex_destroy(&philosopher->forksmutex);
	clearpage(philosopher);
}
