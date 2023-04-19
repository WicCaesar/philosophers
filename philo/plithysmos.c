/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plithysmos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 02:31:06 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/18 22:34:22 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophoi.h"

// Adds a page to the end of the guest list (journal).
void	addpage(t_journal **page, t_journal *newpage)
{
	t_journal	*penultimate;

	if (!newpage)
		return ;
	if (!*page)
	{
		*page = newpage;
		return ;
	}
	penultimate = lastpage(*page);
	penultimate->next = newpage;
}
// Simpler syntax: lastpage(*page)->next = newpage;

t_journal	*newpage(long id, t_timers *timers, t_mutexes *mutexes)
{
	t_journal	*page;

	page = malloc(sizeof(t_journal));
	page->philosopher = (pthread_t)id;
	page->hastheirownfork = 1;
	page->timers = *&timers;
	page->mutexes = *&mutexes;
	pthread_mutex_init(&page->forksmutex, NULL);
	page->next = NULL;
	return (page);
}

t_journal	*lastpage(t_journal *page)
{
	if (!page)
		return (NULL);
	while (page->next)
		page = page->next;
	return (page);
}

// While the assessed philosophers are descendants, not the ancestor, free them.
// When there's no one left, free the ancestor (our reference).
void	clearpage(t_journal *page)
{
	t_journal	*temp;
	t_journal	*ancestor;

	if (!page)
		return ;
	ancestor = page;
	while (page->next != ancestor)
	{
		temp = page->next;
		pthread_mutex_destroy(&page->forksmutex);
		free(page);
		page = temp;
	}
	pthread_mutex_destroy(&page->forksmutex);
	free(page);
}
