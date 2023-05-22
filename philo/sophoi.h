/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophoi.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 08:14:11 by cnascime          #+#    #+#             */
/*   Updated: 2023/05/22 11:13:39 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//* 		“There’s a nearby possible world in which I take you out to dinner.”
//					 									  David Lewis, possibly.
//* “Vamos parar de um matar o outro! Para quê? Para comer? Matando para comer?”
//																	Inês Brasil.

#ifndef SOPHOI_H
# define SOPHOI_H

# include <stdio.h> // printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // usleep
# include <pthread.h> // pthread, mutex
# include <sys/time.h> // gettimeofday

//! COLOCAR DE NOVO DEPOIS DE AJEITAR TUDO
/*
# define THINKS "\033[3;36m%4ld %3ld is thinking\033[0m\n"
# define HASAFORK "\033[2;3;32m%4ld %3ld has taken a fork\033[0m\n"
# define EATS "\033[1;32m%4ld %3ld is eating\033[0m\n"
# define SLEEPS "\033[2;37m%4ld %3ld is sleeping\033[0m\n"
# define DIED "%4ld %3ld \033[1;31md\033[1;33mi\033[1;32me\033[1;35md\033[0m\n"
# define SUCCESS 1
# define FAILURE 0
# define TRUE 1
# define FALSE 0
*/

//! SÓ PARA TESTAR NO VISUALIZER, APAGAR DEPOIS
# define THINKS		"\033[3;36m%ld %ld is thinking\033[0m\n"
# define HASAFORK	"\033[2;3;32m%ld %ld has taken a fork\033[0m\n"
# define EATS		"\033[1;32m%ld %ld is eating\033[0m\n"
# define SLEEPS		"\033[2;37m%ld %ld is sleeping\033[0m\n"
# define DIED		"%ld %ld \033[1;31md\033[1;33mi\033[1;32me\033[1;35md\033[0m\n"
# define SUCCESS	1
# define FAILURE	0
# define TRUE		1
# define FALSE		0

typedef struct s_journal
{
	pthread_t				philosopher;
	int						hastheirownfork;
	pthread_mutex_t			forksmutex;
	int						meals;
	long int				birth;
	long int				death;
	struct s_timers			*timers;
	struct s_mutexes		*mutexes;
	struct s_journal		*next;
}	t_journal;

typedef struct s_timers
{
	long int	dining;
	long int	dreaming;
	long int	starvation;
	int			mandatorymeals;
}	t_timers;

// The variable stillhungry starts with the same number of philosophers.
// The number decreases each time a guest repeats the mandatory amount of meals.
// If it reaches 0, the simulation stops, no one can't take anymore food.
// On the other hand, if a philosopher hadn't eaten for a specified time,
// their hunger is unbearable, they also can't take anymore.
//*If the canttakeanymore variable is raised, the simulation stops.
typedef struct s_mutexes
{
	int				stillhungry;
	int				cantanymore;
	pthread_mutex_t	bellymutex;
	pthread_mutex_t	pulsemutex;
	pthread_mutex_t	printmutex;
}	t_mutexes;

int			validation(int argc, char **argv);
int			loadtimers(int guests, t_timers *timers, char **argv);
void		loadmutexes(int guests, t_mutexes *mutexes);
t_journal	*fillinguestlist(int guests, t_timers *timers, t_mutexes *mutexes);
void		addpage(t_journal **page, t_journal *newpage);
t_journal	*newpage(long id, t_timers *timers, t_mutexes *mutexes);
t_journal	*lastpage(t_journal *page);
void		clearpage(t_journal *page);
int			sitattable(int guests, t_journal *philosophers);
void		*routine(void *journal);
long		ms(void);
void		inform(t_journal *philosopher, char *message);
void		think(t_journal *philosopher);
void		eat(t_journal *philosopher);
int			forksinhand(t_journal *philosopher);
int			grabfork(t_journal *philosopher, t_journal *forkowner);
void		returnforks(t_journal *philosopher);
int			fullbelly(t_journal *philosopher);
void		dream(t_journal *philosopher);
void		siesta(t_journal *philosopher, long timer);
int			takepulse(t_journal *philosopher);
void		unloadmutexes(t_mutexes *mutexes, t_journal *philosopher);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			ft_isspace(char c);
int			ft_operator(const char *str);

#endif
