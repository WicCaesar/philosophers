/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kyklos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 18:09:22 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/19 02:44:18 by cnascime         ###   ########.fr       */
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
	if ((long int)philosopher->philosopher % 2 == 0)
		usleep(philosopher->timers->dining * 1000);
	while (!philosopher->mutexes->cantanymore)
	{
		eat(philosopher);
		returnforks(philosopher);
		if (fullbelly(philosopher))
			return (NULL);
		dream(philosopher);
		inform(philosopher, THINKS);
	}
	return (NULL);
}
//*The death variable stores the time at which the philosopher will die.
// A philosopher dies if the time elapsed since the start of the simulation
// is greater than the time it takes for starvation.
// The philosopher's death is checked every time they eat or sleep.
// If the philosopher is dead, the simulation ends.
// Part of the philosophers start eating, while others "sleep" for the same
// time it takes them to eat, so the forks are available for the other round.

// If the philosopher has two forks in their hand, they are able and must eat.
// The number of meals is incremented and their death is postponed.
void	eat(t_journal *philosopher)
{
	if (forksinhand(philosopher) < 2)
	{
		inform(philosopher, THINKS);
		usleep(philosopher->timers->starvation * 1000);
		return ;
	}
	inform(philosopher, EATS);
	philosopher->meals++;
	philosopher->death = ms() + philosopher->timers->starvation;
	siesta(philosopher, philosopher->timers->dining);
}

// The philosopher rests after a meal. Do they also dream of electric sheep?
void	dream(t_journal *philosopher)
{
	inform(philosopher, SLEEPS);
	siesta(philosopher, philosopher->timers->dreaming);
}
