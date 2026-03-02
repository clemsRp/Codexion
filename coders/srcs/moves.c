/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement <clement@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 09:20:37 by crappo            #+#    #+#             */
/*   Updated: 2026/03/02 11:36:45 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	get_dongle(t_coder *coder, int is_left)
{
	t_dongle *d;
	
	d = coder->right;
	if (is_left)
		d = coder->left;
	pthread_mutex_lock(&d->mutex);
	while (get_is_running(coder->params))
	{
		if (!d->is_taken && d->queue[0] == coder->id
			&& timestamp_ms(coder->params->start) >= d->end_of_cooldown)
		{
			d->is_taken = 1;
			pthread_mutex_unlock(&d->mutex);
			return (1);
		}
		pthread_cond_wait(&d->cond, &d->mutex);
	}
	pthread_mutex_unlock(&d->mutex);
	return (0);
}

void	release_dongle(t_coder *coder, int is_left)
{
	t_dongle *d;
	
	d = coder->right;
	if (is_left)
		d = coder->left;
	pthread_mutex_lock(&d->mutex);
	d->is_taken = 0;
	d->end_of_cooldown = timestamp_ms(coder->params->start) + coder->params->dongle_cooldown;
	d->queue[0] = d->queue[1];
	d->queue[1] = -1;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder, 0);
	release_dongle(coder, 1);
}

int	compile(t_coder *coder)
{
	int first;
	
	first = (coder->id % 2 == 0);
	add(coder, first);
	if (!get_dongle(coder, first))
		return (0);
	print_message(coder->params, "has taken a dongle", coder->id);
	add(coder, !first);
	if (!get_dongle(coder, !first)) {
		release_dongle(coder, first);
		return (0);
	}
	print_message(coder->params, "has taken a dongle", coder->id);
	pthread_mutex_lock(&coder->mutex);
	print_message(coder->params, "is compiling", coder->id);
	coder->nb_compile++;
	coder->last_compile = timestamp_ms(coder->params->start);
	coder->deadline = coder->last_compile + coder->params->time_to_burnout;
	usleep(coder->params->time_to_compile * 1000);
	pthread_mutex_unlock(&coder->mutex);
	release_dongle(coder, !first);
	release_dongle(coder, first);
	return (1);
}
