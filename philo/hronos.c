/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hronos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 07:37:17 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/18 09:08:08 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophos.h"
#include <stdint.h>

// TODO utils (calcular milissegundos)
// ! renomear para chronos, dependendo das funções que entrem aqui

// Converts the time in seconds and microseconds (usec) to miliseconds.
long int	ms(void)
{
	struct timeval	time;
	long int		miliseconds;

	gettimeofday(&time, NULL);
	miliseconds = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (miliseconds);
}

void	siesta(t_journal *philosopher, long int timer) // ! problema está aqui!
{
	long int	nap;

	nap = ms() + timer;
	while (ms() < nap) // enquanto não dormiu o tempo necessário
	{
		if (takepulse(philosopher) == 0) // se o filósofo morreu no meio do sono
			break ;
	}
	usleep(philosopher->timers->dining); // ! testar com mais
}

// If the philosopher died in the meantime, no false information is spread.
// Otherwise, locks the mutex, prints the message, then unlocks the mutex.
// ! tentar colocar isso dentro das próprias funções do ciclo de vida, talvez?
// * NÃÃÃÃÃO!!! 
void	inform(t_journal *philosopher, char *message)
{
	if (takepulse(philosopher) == 0) // !ao colocar isso posso estar impedindo a informação da morte chegar
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
	pthread_mutex_lock(&philosopher->mutexes->pulsemutex); // tranca enquanto realiza as verificações abaixo
	if (philosopher->mutexes->cantanymore == 1) // se ele já foi dado como morto
	{
		pthread_mutex_unlock(&philosopher->mutexes->pulsemutex); // destranca para não causar deadlock
		return (0); // não há pulso
	}
	if (philosopher->death <= ms()) // se o tempo de morte for menor que o tempo atual // menor ou menor ou igual
	{
		philosopher->mutexes->cantanymore = 1; // ele é dado como morto (flag sobe)
		pthread_mutex_unlock(&philosopher->mutexes->pulsemutex); // !reordenar para ser o último do if e testar
		pthread_mutex_lock(&philosopher->mutexes->printmutex);
		printf(DIED, ms() - philosopher->birth, philosopher->philosopher);
		pthread_mutex_unlock(&philosopher->mutexes->printmutex);
		return (0); // não há pulso
	}
	pthread_mutex_unlock(&philosopher->mutexes->pulsemutex);
	return (1); // há pulso, filósofo está vivo
}

void	unloadmutexes(t_mutexes *mutexes, t_journal *philosopher)
{
	pthread_mutex_destroy(&mutexes->printmutex);
	pthread_mutex_destroy(&mutexes->bellymutex);
	pthread_mutex_destroy(&mutexes->pulsemutex);
	clearpage(philosopher);
}
