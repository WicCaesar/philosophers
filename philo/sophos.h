/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 08:14:11 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/17 04:02:16 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//* 	“There’s a nearby possible world in which I take you out to dinner.”
//					 									David Lewis, possibly.

#ifndef SOPHOS_H
# define SOPHOS_H

//# include <bits/pthreadtypes.h> // De onde saiu isso?
# include <stdio.h> // printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // usleep
# include <pthread.h> // pthread, mutex
# include <sys/time.h> // gettimeofday

# define EATS "%ld %ld is eating\n"
# define HASAFORK "%ld %ld has taken a fork\n"
# define THINKS "%ld %ld is thinking\n"
# define SLEEPS "%ld %ld is sleeping\n"
# define DIED "%ld %ld died\n"

/*
! Each philosopher should be a thread.
There is one fork between each pair of philosophers.
Therefore, if there are several philosophers, each philosopher has a fork on
their left side and one on their right side. If there is only one philosopher, 
there should be only one fork on the table.
! To prevent duplicating forks, protect their state with a mutex for each. */

typedef struct s_journal
{
	pthread_t				philosopher;
	int						hastheirownfork; //! flag para saber se filósofo está com seu garfo
	pthread_mutex_t			forksmutex;
	int						meals;
	long					birth; // ! renomear para birth ou start, talvez?
	long					death;
	struct s_timers			*timers;
	struct s_mutexes		*mutexes;
	struct s_journal		*next;
}	t_journal; //! colocar timers nessa struct?
// ! LAST MEAL??? RIGHT FORK AND LEFT FORK???

// ! ESTUDAR DATA RACE E DEADLOCK
// TODO ESTUDAR DATA RACE E DEADLOCK
// ! CHECAR VAZAMENTOS DE MEMÓRIA
// TODO ESTUDAR DATA RACE E DEADLOCK
// ! ESTUDAR DATA RACE E DEADLOCK
// TODO ESTUDAR DATA RACE E DEADLOCK
// ! CHECAR VAZAMENTOS DE MEMÓRIA

typedef struct s_timers //! renomear  variáveis abaixo, talvez?
{
	long	dining; //? The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
	long	dreaming; //? The time a philosopher will spend sleeping.
	long	starvation; //? If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
	int		mandatorymeals;
}	t_timers;

// The variable stillhungry starts with the same number of philosophers.
// The number decreases each time a guest repeats the mandatory amount of meals.
// If that number reaches 0, the simulation stops, no one can't take anymore.
// On the other hand, if a philosopher hadn't eaten for quite some time,
// the hunger is unbearable, they also can't take anymore.
//*If the canttakeanymore variable is raised, the simulation stops.
typedef struct s_mutexes
{
	int				stillhungry; //! flag para saber se todos os filósofos já repetiram o prato a quantidade obrigatória
	int				cantanymore; //! flag quando o filósofo não aguenta mais de fome ou não aguenta mais comer
	pthread_mutex_t	bellymutex;
	pthread_mutex_t	pulsemutex;
	pthread_mutex_t	printmutex;
}	t_mutexes;

int			validation(int argc, char **argv);
int			loadtimers(int guests, t_timers *timers, char **argv);
void		loadmutexes(int guests, t_mutexes *mutexes);
t_journal	*fillinguestlist(int guests, t_timers *timers, t_mutexes *mutexes);
void		addpage(t_journal **page, t_journal *newpage);
t_journal	*newpage(int id, t_timers *timers, t_mutexes *mutexes);
t_journal	*lastpage(t_journal *page);
void		clearpage(t_journal *page);
int			sitattable(int guests, t_journal *philosophers);
void		*routine(void *journal);
long		ms(void);
void		inform(t_journal *philosopher, char *message);
int			think(t_journal *philosopher);
int			eat(t_journal *philosopher);
int			forksinhand(t_journal *philosopher);
int			grabfork(t_journal *philosopher, t_journal *forkowner);
void		returnforks(t_journal *philosopher);
int			fullbelly(t_journal *philosopher);
int			dream(t_journal *philosopher);
int			siesta(t_journal *philosopher, long timer);
int			takepulse(t_journal *philosopher);
void		unloadmutexes(t_mutexes *mutexes, t_journal *philosopher);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			ft_isspace(char c);
int			ft_operator(const char *str);

#endif
