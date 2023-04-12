/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 08:14:11 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/12 05:07:44 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//* 	“There’s a nearby possible world in which I take you out to dinner.”
//					 									David Lewis, possibly.

#ifndef SOPHOS_H
# define SOPHOS_H

//# include <bits/pthreadtypes.h> // De onde saiu isso?
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

/*
! Each philosopher should be a thread.
There is one fork between each pair of philosophers.
Therefore, if there are several philosophers, each philosopher has a fork on
their left side and one on their right side. If there is only one philosopher, 
there should be only one fork on the table.
! To prevent duplicating forks, protect their state with a mutex for each. */

typedef struct s_philosophers
{
	pthread_t				id;
	int						meals;
	pthread_mutex_t			forks; // ! começa com 1, como atribuir quantidade?
	struct s_philosophers	*next;
}	t_philosophers;

typedef struct s_timers
{
	long	hunger;
	long	fatigue;
	long	defunction;
	int		mandatorymeals;
}	t_timers;

// uma struct só para mutexes?

int				timer(int population, t_timers *timers, char **argv);

t_philosophers	*table(t_timers *timers, int population);
void			addguru(t_philosophers **guru, t_philosophers *newguru);
t_philosophers	*newphilosopher(int id);
t_philosophers	*lastphilosopher(t_philosophers *philosopher);
void			clearphilosopher(t_philosophers *philosopher);

int				cycle(t_philosophers *philosopher); // ! test

void			*lunch(t_philosophers *philosopher);
void			*think(t_philosophers *philosopher);
void			*sleep(t_philosophers *philosopher);

int				ft_atoi(const char *str);
int				ft_isdigit(int c);
int				ft_isspace(char c);
int				ft_operator(const char *str);
int				validation(int argc, char **argv);

#endif
