/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makaronia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 07:05:15 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/18 10:47:12 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

//TODO Funções estritamente ligadas ao ato de comer (pegar garfos, usar, soltar)

// A philosopher needs two forks for eating spaghetti, currently holding none.
// If the philosopher is alive, they try to take forks.
// First, they try to grab their own fork, then the next philosopher's.
// If the philosopher is able to take'em both, the function will return 2.
int	forksinhand(t_journal *philosopher)
{
	static int	forksinhand;

	forksinhand = 0;
	while (forksinhand < 2)
	{
		if (takepulse(philosopher) == 0)
			return (0); // se está morto, não tenta fazer nada
		forksinhand += grabfork(philosopher, philosopher); // pega seu próprio garfo
		if (takepulse(philosopher) == 0)
			return (0);
		forksinhand += grabfork(philosopher, philosopher->next); // pega o do comensal
	}
	return (forksinhand);
}

// In this function, the philosopher (first argument) tries to take a fork,
// either his own or the next philosopher's (second argument).
// First of all, the fork mutex is locked while verifications are made.
// If the fork IS available, it takes it, unlocks the mutex and returns 1.
// If the fork is NOT available, it unlocks the mutex and returns 0.
int	grabfork(t_journal *philosopher, t_journal *forkowner)
{
	pthread_mutex_lock(&forkowner->forksmutex);
	if (forkowner->hastheirownfork)
	{
		forkowner->hastheirownfork = 0; // não tem mais o garfo momentaneamente
		inform(philosopher, HASAFORK);
		pthread_mutex_unlock(&forkowner->forksmutex);
		return (1);
	}
	else
		pthread_mutex_unlock(&forkowner->forksmutex);
	return (0);
}

// Simply makes two philosophers have their own forks again.
// Mutexes are used for safety reasons.
//! posso fazer nessa ordem ou é melhor trancar e destrancar cada um?
void	returnforks(t_journal *philosopher)
{
	pthread_mutex_lock(&philosopher->forksmutex); // ! testar outra ordem:
	philosopher->hastheirownfork = 1; // ! (LOCK both, return both, UNLOCK both)
	pthread_mutex_unlock(&philosopher->forksmutex);
	pthread_mutex_lock(&philosopher->next->forksmutex);
	philosopher->next->hastheirownfork = 1;
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
	if (philosopher->meals == philosopher->timers->mandatorymeals) // se o filósofo já comeu o suficiente
	{
		philosopher->mutexes->stillhungry--; // diminui o número de filósofos ainda com fome
		if (philosopher->mutexes->stillhungry == 0) // se não tem mais ninguém com fome, o jantar está encerrado
		{
			pthread_mutex_unlock(&philosopher->mutexes->bellymutex); // ! reordenar para ser o último do if e testar
			pthread_mutex_lock(&philosopher->mutexes->pulsemutex);
			philosopher->mutexes->cantanymore = 1; // não aguentam mais (comer)
			pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
		}
		else
			pthread_mutex_unlock(&philosopher->mutexes->bellymutex);
		return (1); // estômago cheio
	}
	pthread_mutex_unlock(&philosopher->mutexes->bellymutex);
	return (0); // ainda não está de estômago cheio
}
