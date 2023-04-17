/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 06:17:13 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/17 04:51:23 by cnascime         ###   ########.fr       */
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
	int				guests;
	t_timers		*timers;
	t_mutexes		*mutexes;
	t_journal		*journal;

	if (validation(argc, argv) == 0)
		return (0);
	guests = ft_atoi(argv[1]);
	timers = malloc(sizeof(t_timers));
	mutexes = malloc(sizeof(t_mutexes));
	if (loadtimers(guests, timers, argv) == 0)
		return (0);
	loadmutexes(guests, mutexes);
	journal = fillinguestlist(guests, timers, mutexes);
	sitattable(guests, journal);
	unloadmutexes(mutexes, journal);
	return (0);
}

// Setting up timers.
int	loadtimers(int guests, t_timers *timers, char **argv)
{
	printf("guests: %d \t\t argv[1]: %s\n", guests, argv[1]); // ! apagar
	timers->starvation = ft_atoi(argv[2]);
	printf("starvation: %ld \t argv[2]: %s\n", timers->starvation, argv[2]); // ! apagar
	timers->dining = ft_atoi(argv[3]);
	printf("dining: %ld \t\t argv[3]: %s\n", timers->dining, argv[3]); // ! apagar
	timers->dreaming = ft_atoi(argv[4]);
	printf("dreaming: %ld \t\t argv[4]: %s\n", timers->dreaming, argv[4]); // ! apagar
	if (guests < 1
		|| timers->starvation < 0 || timers->dining < 0 || timers->dreaming < 0)
		return (0);
	if (argv[5])
	{
		timers->mandatorymeals = ft_atoi(argv[5]);
		printf("mandatorymeals: %d \t\t argv[5]: %s\n", timers->mandatorymeals, argv[5]); // ! apagar
		if (timers->mandatorymeals < 1)
			return (0);
	}
	else
		timers->mandatorymeals = -1;
	return (1);
}

// Initialising mutexes.
// Unnecessary to start cantanymore because variables in a struct are already 0.
// stillhungry are the guests that haven't had the minimum amount of meals.
// pthread_mutex_init is used because the common macro PTHREAD_MUTEX_INITIALIZER
// is forbidden. The pthread_mutex_init function is called with the address of
// the mutex variable and the address of the mutex attribute variable, which
// sets the mutex type. The mutex type can be set to PTHREAD_MUTEX_NORMAL,
// PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_RECURSIVE, or PTHREAD_MUTEX_DEFAULT.
// The default mutex type is PTHREAD_MUTEX_NORMAL. The PTHREAD_MUTEX_NORMAL
// mutex type does not detect deadlock. The PTHREAD_MUTEX_ERRORCHECK mutex type
// detects deadlock by returning an error if a thread tries to lock a mutex that
// it already owns. The PTHREAD_MUTEX_RECURSIVE mutex type allows a thread to
// lock a mutex that it already owns.
void	loadmutexes(int guests, t_mutexes *mutexes)
{
	// mutexes->cantanymore = 0; // ! delete before sending
	mutexes->stillhungry = guests;
	pthread_mutex_init(&mutexes->printmutex, NULL);
	pthread_mutex_init(&mutexes->bellymutex, NULL);
	pthread_mutex_init(&mutexes->pulsemutex, NULL);
}

// Each guest fills in their own page of the guest list and passes to the next,
// thus filling the journal struct.
t_journal	*fillinguestlist(int guests, t_timers *timers, t_mutexes *mutexes)
{
	int			i;
	t_journal	*guestlist;

	i = 0;
	guestlist = NULL;
	while (i < guests)
	{
		addpage(&guestlist, newpage(i + 1, timers, mutexes));
		i++;
	}
	//! adicionar aqui a id certa do filósofo (+1)? verificar se já deu certo
	lastpage(guestlist)->next = guestlist;
	return (guestlist);
}

// Each guest takes their place at the table, which means threads are created,
// the routine is set and then we wait for them to end their cycles.
int	sitattable(int guests, t_journal *journal)
{
	int			i;
	pthread_t	*philosopher;

	i = 0;
	philosopher = malloc(sizeof(pthread_t) * guests);
	while (i < guests)
	{
		if (pthread_create(&philosopher[i], NULL, &routine, &journal) != 0)
			return (0); // ! retornar erro?
		journal = journal->next;
		i++;
	}
	i = 0;
	while (i < guests)
	{
		if (pthread_join(philosopher[i], NULL) != 0) //! testar sem if
			return (0); // ! retornar erro?
		i++;
	}
	return (1);
}
// The first argument of the pthread_create function is the address of the
// thread variable. The second argument is the address of the thread attribute
// variable, which is set to NULL to use the default attributes.
//*The third argument is the function to execute when the thread is created.
//*The fourth argument is the only argument that will be passed to the function.
//*The pthread_join function is called to wait for a thread to terminate.
// The first argument is the thread variable.
// The second argument is the variable that will be set to the return value.
// It is set to NULL if the return value is not needed.
// In this case it returns the same as the pthread_create function,
// so both functions return 0 if the thread is successfully created or joined.

// iniciar threads
/*
(id do filósofo + convidados) % convidados
! se o número de filósofos for par, o primeiro filósofo a pegar o garfo será o último
! se o número de filósofos for ímpar, o primeiro filósofo a pegar o garfo será o primeiro

! se o número de filósofos for par, o primeiro filósofo a soltar o garfo será o primeiro
! se o número de filósofos for ímpar, o primeiro filósofo a soltar o garfo será o último

! se o número de filósofos for par, o primeiro filósofo a comer será o último
! se o número de filósofos for ímpar, o primeiro filósofo a comer será o primeiro

! se o número de filósofos for par, o primeiro filósofo a dormir será o primeiro
! se o número de filósofos for ímpar, o primeiro filósofo a dormir será o último

! se o número de filósofos for par, o primeiro filósofo a pensar será o último
! se o número de filósofos for ímpar, o primeiro filósofo a pensar será o primeiro

! se o número de filósofos for par, o primeiro filósofo a morrer será o primeiro
! se o número de filósofos for ímpar, o primeiro filósofo a morrer será o último

! se o número de filósofos for par, o primeiro filósofo a ficar com fome será o último
! se o número de filósofos for ímpar, o primeiro filósofo a ficar com fome será o primeiro

! se o número de filósofos for par, o primeiro filósofo a ficar com sono será o primeiro
! se o número de filósofos for ímpar, o primeiro filósofo a ficar com sono será o último

! se o número de filósofos for par, o primeiro filósofo a ficar com sede será o último
! se o número de filósofos for ímpar, o primeiro filósofo a ficar com sede será o primeiro

thread init em todos
contudo iniciar só filósofos pares (ou ímpares)
pares pegam o garfo
ímpar tem que dormir (o mesmo tempo que leva para comer)
! só na primeira rodada
pares vão dormir e ímpares vão pegar os garfos soltos

*/
