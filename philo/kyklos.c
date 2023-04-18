/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kyklos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 18:09:22 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/18 10:47:09 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

// The journal passed on thread creation becomes each philosopher's very own.
// The philosopher's birth is recorded and their lifespan is set.
// The philosopher's whose number is even sleeps for the same time it takes
// to eat, so the forks are available for the other round.
void	*routine(void *journal)
{
	t_journal	*philosopher;

	philosopher = (t_journal *)journal;
	philosopher->birth = ms();
	philosopher->death = philosopher->birth + philosopher->timers->starvation;
	if (philosopher->philosopher % 2 == 0)
		usleep(philosopher->timers->dining * 1000); // pares dormem
	// só os ímpares estão coisando
	while (!philosopher->mutexes->cantanymore)
	{
		think(philosopher);
		eat(philosopher);
		returnforks(philosopher); // ! mover para eat?
		if (fullbelly(philosopher)) // ! mover para eat?
			return (NULL);
		dream(philosopher);
	}
	return (NULL);
}
//*The death variable stores the time at which the philosopher will die.
// A philosopher dies if the time elapsed since the start of the simulation
// is greater than the time it takes for starvation.
//!The philosopher's death is checked every time he eats, sleeps or thinks.
// If the philosopher is dead, the simulation ends.
// Part of the philosophers start eating, while others "sleep" for the same
// time it takes them to eat, so the forks are available for the other round.

/*
thread init em todos
contudo iniciar só filósofos pares (ou ímpares)
pares pegam o garfo
ímpar tem que dormir (o mesmo tempo que leva para comer)
! só na primeira rodada
pares vão dormir e ímpares vão pegar os garfos soltos
*/

// In this function, the philosopher attempts to take forks for eating.
// If the philosopher is dead, the function returns 1.
// If the philosopher is not dead, the function tries to take both forks.
// If the philosopher is able to take both forks, the function returns 0.
// If the philosopher is not able to take both forks, the function returns 1.
// ! Atualizar acima, já está diferente (dividi em outras funções).
void	eat(t_journal *philosopher)
{
	if (forksinhand(philosopher) != 2)
		return ;
	inform(philosopher, EATS);
	philosopher->meals++;
	philosopher->death = ms() + philosopher->timers->starvation;
	siesta(philosopher, philosopher->timers->dining);
	return ;
}

// Not necessary anymore.
void	think(t_journal *philosopher)
{
	inform(philosopher, THINKS);
	return ;
}

void	dream(t_journal *philosopher)
{
	inform(philosopher, SLEEPS);
	siesta(philosopher, philosopher->timers->dreaming);
	return ;
}
