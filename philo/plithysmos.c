/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plithysmos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnascime <cnascime@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 02:31:06 by cnascime          #+#    #+#             */
/*   Updated: 2023/04/17 05:03:29 by cnascime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophos.h"

// Adds a page to the end of the guest list (journal).
void	addpage(t_journal **page, t_journal *newpage)
{
	if (!newpage)
		return ;
	if (!page)
		*page = newpage;
	else
		lastpage(*page)->next = newpage;
}

t_journal	*newpage(int id, t_timers *timers, t_mutexes *mutexes)
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
