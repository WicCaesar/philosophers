/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophoi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 06:17:13 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/19 03:41:27 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

// The pthread_mutex_init function is called because we cannot use the common
// macro PTHREAD_MUTEX_INITIALIZER. The pthread_mutex_init function is called
// with the address of the mutex variable and the address of the mutex attribute
// variable, which sets the mutex type.

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
	free(timers);
	free(mutexes);
	return (0);
}

// Setting up timers.
int	loadtimers(int guests, t_timers *timers, char **argv)
{
	timers->starvation = ft_atoi(argv[2]);
	timers->dining = ft_atoi(argv[3]);
	timers->dreaming = ft_atoi(argv[4]);
	if (guests < 1
		|| timers->starvation < 1 || timers->dining < 1 || timers->dreaming < 1)
		return (0);
	if (argv[5])
	{
		timers->mandatorymeals = ft_atoi(argv[5]);
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
// is forbidden. The second argument of pthread_mutex_init function is the 
// attribute. It is set to NULL because we want the default mutex type. Why tho?
void	loadmutexes(int guests, t_mutexes *mutexes)
{
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
		pthread_create(&philosopher[i], NULL, &routine, (void *)journal);
		journal = journal->next;
		i++;
	}
	i = 0;
	while (i < guests)
	{
		pthread_join(philosopher[i], NULL);
		i++;
	}
	free(philosopher);
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