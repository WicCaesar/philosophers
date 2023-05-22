/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makaronia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 07:05:15 by cnascime          #+#    #+#             */
/*   Updated: 2023/05/22 11:26:21 by cnascime         ###   ########.fr       */
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
	{
		forksinhand += grabfork(philosopher, philosopher->next);
		printf("%ld pegou o garfo do próximo (%ld).\n", philosopher->philosopher, philosopher->next->philosopher); // ! apagar
	}
	//forksinhand += grabfork(philosopher, philosopher);
	// ! APAGAR if abaixo, manter acima
	if (grabfork(philosopher, philosopher))
	{
		forksinhand++;
		printf("%ld pegou o próprio garfo.\n", philosopher->philosopher); // ! apagar
	}
	printf("%d\n", forksinhand); // ! apagar
	return (forksinhand);
}

// In this function, the philosopher (first argument) tries to take a fork,
// either his own or the next philosopher's (according to the second argument).
// First of all, the fork mutex is locked while verifications are made.
int	grabfork(t_journal *philosopher, t_journal *forkowner)
{
	pthread_mutex_lock(&forkowner->forksmutex);
	if (forkowner->hastheirownfork == TRUE)
	{
		forkowner->hastheirownfork = FALSE;
		inform(philosopher, HASAFORK);
		pthread_mutex_unlock(&forkowner->forksmutex);
		return (SUCCESS);
	}
	else
		pthread_mutex_unlock(&forkowner->forksmutex);
	return (FAILURE);
}

// Simply makes two philosophers have their own forks again.
// Mutexes are used for safety reasons.
void	returnforks(t_journal *philosopher)
{
	pthread_mutex_lock(&philosopher->forksmutex);
	philosopher->hastheirownfork = TRUE;
	printf("%ld devolveu o próprio garfo.\n", philosopher->philosopher); // ! apagar
	pthread_mutex_unlock(&philosopher->forksmutex);
	pthread_mutex_lock(&philosopher->next->forksmutex);
	philosopher->next->hastheirownfork = TRUE;
	printf("%ld devolveu o garfo do próximo.\n", philosopher->philosopher); // ! apagar
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
