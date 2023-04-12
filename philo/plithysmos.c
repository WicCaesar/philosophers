/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plithysmos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 02:31:06 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/12 05:07:36 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophos.h"

// ? Calling philosophers gurus so it doesn't trespass the limit of 80 columns.
// Adds a philosopher to the end of the list.
void	addguru(t_philosophers **guru, t_philosophers *newguru)
{
	if (!newguru)
		return ;
	if (!guru)
		*guru = newguru;
	else
		lastphilosopher(*guru)->next = newguru;
}

t_philosophers	*newphilosopher(int id)
{
	t_philosophers	*philosopher;

	philosopher = malloc(sizeof(t_philosophers));
	philosopher->id = (pthread_t)id;
	philosopher->meals = 0;
	pthread_mutex_init(&philosopher->forks, NULL); // ! como atribuir quantidade de garfos na mutex?
	philosopher->next = NULL;
	return (philosopher);
}

t_philosophers	*lastphilosopher(t_philosophers *philosopher)
{
	if (!philosopher)
		return (NULL);
	while (philosopher->next)
		philosopher = philosopher->next;
	return (philosopher);
}

// While the assessed philosopher is a descendant, not the ancestor, free them.
// When there's no one left, free the ancestor (reference).
void	clearphilosopher(t_philosophers *philosopher)
{
	t_philosophers	*temp;
	t_philosophers	*ancestor;

	if (!philosopher)
		return ;
	ancestor = philosopher;
	while (philosopher->next != ancestor)
	{
		temp = philosopher->next;
		pthread_mutex_destroy(&philosopher->forks);
		free(philosopher);
		philosopher = temp;
	}
	pthread_mutex_destroy(&philosopher->forks);
	free(philosopher);
}
