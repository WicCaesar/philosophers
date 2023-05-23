/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makaronia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 07:05:15 by cnascime          #+#    #+#             */
/*   Updated: 2023/05/23 19:38:03 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

// A philosopher needs two forks for eating spaghetti, currently holding none.
// First, they try to grab the next philosopher's fork, then their own.
// This is because a philosopher might cause the death of a partaker if they
// won't let go of their own forks.
// The function returns the number of successful attempts.
int	forksinhand(t_journal *philosopher)
{
	int	forksinhand;

	forksinhand = 0;
	if (philosopher->next != philosopher)
		forksinhand += grabfork(philosopher, philosopher->next);
	forksinhand += grabfork(philosopher, philosopher);
	return (forksinhand);
}

// In this function, the philosopher (first argument) tries to take a fork,
// either his own or the next philosopher's (according to the second argument).
// First of all, the fork mutex is locked while verifications are made.
int	grabfork(t_journal *philosopher, t_journal *forkowner)
{
	pthread_mutex_lock(&forkowner->forksmutex);
	inform(philosopher, HASAFORK);
	return (SUCCESS);
}

// Simply makes two philosophers have their own forks again.
// Mutexes are used for safety reasons.
void	returnforks(t_journal *philosopher)
{
	pthread_mutex_unlock(&philosopher->forksmutex);
	pthread_mutex_unlock(&philosopher->next->forksmutex);
}

// Locks the belly mutex while verifications are made.
// If the philosopher has eaten enough, stillhungry decreases.
//*If there's no one else still hungry, cantanymore is set to 1, dinner is over!
// The belly mutex is unlocked and the function returns 1.
// Otherwise, unlocks the belly mutex and returns 0.
int	fullbelly(t_journal *philosopher)
{
	pthread_mutex_lock(&philosopher->mutexes->bellymutex);
	if (philosopher->meals == philosopher->timers->mandatorymeals)
	{
		philosopher->mutexes->stillhungry--;
		if (philosopher->mutexes->stillhungry == FALSE)
		{
			pthread_mutex_unlock(&philosopher->mutexes->bellymutex);
			pthread_mutex_lock(&philosopher->mutexes->pulsemutex);
			philosopher->mutexes->cantanymore = TRUE;
			pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
		}
		else
			pthread_mutex_unlock(&philosopher->mutexes->bellymutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&philosopher->mutexes->bellymutex);
	return (FALSE);
}
