/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 06:17:13 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/12 17:52:37 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophos.h"
#include <bits/pthreadtypes.h> // ! apareceu sozinho no VS Code?

/* One or more philosophers sit at a round table.
There is a large bowl of spaghetti in the middle of the table.
The philosophers alternatively eat, think, or sleep.
! While eating, they are not thinking nor sleeping.
! While thinking, they are not eating nor sleeping.
! While sleeping, they are not eating nor thinking.
! There are as many forks as philosophers.
! A philosopher takes their right and their left forks to eat, one in each hand.
! When a philosopher has finished eating, they put their forks back on the table
!and start sleeping. Once awake, they start thinking again. The simulation stops 
!when a philosopher dies of starvation.
Every philosopher needs to eat and should never starve.
Philosophers don’t speak with each other.
Philosophers don’t know if another philosopher is about to die.
Philosophers should avoid dying.

! Global variables are forbidden!
! Your(s) program(s) should take the following arguments:
! ./philo <#PHILOSOFORKS> <time_to_DIE> <time_to_EAT> <time_to_SLEEP> [#MEALS]
? #PHILOSOFORKS: The number of philosophers and also the number of forks.
? time_to_DIE (milliseconds): If a philosopher didn’t start eating time_to_DIE
?						milliseconds since the beginning of their last meal
?						or the beginning of the simulation, they die.
? time_to_EAT (milliseconds): The time it takes for a philosopher to eat.
?						During that time, they will need to hold two forks.
? time_to_SLEEP (milliseconds): The time a philosopher will spend sleeping.
? (OPTIONAL) #MEALS: If all philosophers have eaten at least #MEALS times,
?						the simulation stops.
! If not specified, the simulation stops when a philosopher dies.
! Each philosopher has a number ranging from 1 to #PHILOSOFORKS.
! Philosopher number 1 sits next to philosopher number #PHILOSOFORKS.
! Any other philosopher N sits between philosopher N - 1 and philosopher N + 1.

! Any state change of a philosopher must be formatted as follows:
timestamp N has taken a fork
timestamp N is eating
timestamp N is thinking
timestamp N is sleeping
timestamp N died
Replace timestamp with the current time in ms and N with the philosopher number.
! A displayed state message should not be mixed up with another message.
! A message announcing a philosopher died should be displayed
! no more than 10 ms after the actual death of the philosopher.
! Your program must not have any data races.

* ALLOWED FUNCTIONS:
? memset, printf, malloc, free, write, usleep, gettimeofday,
? pthread_create, pthread_detach, pthread_join, pthread_mutex_init,
? pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
*/

//pthread_t	philosopher[ft_atoi(argv[1])];
/*int	main(int argc, char **argv)
{
	pthread_t		philosopher;
	pthread_mutex_t	fork;
	void			*result;

	pthread_mutex_init(fork, NULL); // ! adicionar atributo (padrão: normal)
	if (argc == 5 || argc == 6)
	{
		//pthread_mutex_lock(&fork);
		if (pthread_create(&philosopher, NULL, lunch, NULL) == -1)
			return (0);
		//pthread_mutex_unlock(&fork);
		if (pthread_create(&philosopher, NULL, think, NULL) == -1)
			return (0);
		if (pthread_create(&philosopher, NULL, sleep, NULL) == -1)
			return (0);
		//if (pthread_join(philosopher, &result) == -1)
		//	return (0);
	}
	else
		return (printf("./philo    filósofos à mesa    tempo de vida    tempo que \
	levam para comer    tempo que levam para dormir    opcional: refeições"));
	return (0);
}*/
// The pthread_mutex_init function is called because we cannot use the common
// macro PTHREAD_MUTEX_INITIALIZER. The pthread_mutex_init function is called
// with the address of the mutex variable and the address of the mutex attribute
// variable, which sets the mutex type.
// The mutex type can be set to PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_ERRORCHECK,
// PTHREAD_MUTEX_RECURSIVE, or PTHREAD_MUTEX_DEFAULT. The default mutex type is
// PTHREAD_MUTEX_NORMAL. The PTHREAD_MUTEX_NORMAL mutex type does not detect
// deadlock. The PTHREAD_MUTEX_ERRORCHECK mutex type detects deadlock by
// returning an error if a thread tries to lock a mutex that it already owns.
// The PTHREAD_MUTEX_RECURSIVE mutex type allows a thread to lock a mutex that
// it already owns.

int	main(int argc, char **argv)
{
	int				population;
	t_timers		*timers;
	t_philosophers	*philosopher;

	if (validation(argc, argv) == 0)
		return (0); // ! verificar o que o pdf pede para fazer em caso de erro
	population = ft_atoi(argv[1]);
	timers = NULL;
	if (timer(population, timers, argv) == 0) // ? setting up timers/counters
		return (0); // ! verificar o que o pdf pede para fazer em caso de erro
	// ! mutexes ???
	philosopher = table(timers, population); //(calling philosophers) mutexes?)
	printf("population: %d, defunction: %ld, hunger: %ld, fatigue: %ld, \n", population, timers->defunction, timers->hunger, timers->fatigue);
	printf("mandatorymeals: %d\n", timers->mandatorymeals);
	printf("philosopher: %lu\n", philosopher->id);
	printf("meals: %d\n", philosopher->meals);
	// ! cycles (threads?)
	cycle(philosopher); // ! test
	// ! (free)
	return (0);
}

// 
int	timer(int population, t_timers *timers, char **argv)
{
	population = ft_atoi(argv[1]);
	timers->defunction = ft_atoi(argv[2]);
	timers->hunger = ft_atoi(argv[3]);
	timers->fatigue = ft_atoi(argv[4]);
	if (population < 1
		|| timers->defunction < 1 || timers->hunger < 1 || timers->fatigue < 1)
		return (1);
	if (argv[4])
	{
		timers->mandatorymeals = ft_atoi(argv[5]);
		if (timers->mandatorymeals < 1)
			return (1);
	}
	else
		timers->mandatorymeals = -1;
	return (0);
}

t_philosophers	*table(t_timers *timers, int population)
{
	int				i;
	t_philosophers	*philosopher;

	i = 0;
	(void)timers;
	philosopher = NULL;
	while (i < population)
	{
		addguru(&philosopher, newphilosopher(i + 1));
		i++;
	}
	lastphilosopher(philosopher)->next = philosopher;
	return (philosopher);
}

int	cycle(t_philosophers *philosopher) // ! test
{
	// ! test
	think(philosopher);
	sleep(philosopher);
	lunch(philosopher);
	return (0);
}