/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 09:20:37 by crappo            #+#    #+#             */
/*   Updated: 2026/02/25 17:57:38 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	get_dongle(t_coder *coder, int is_left)
{
	if (is_left)
	{
		pthread_mutex_lock(&coder->left->mutex);	
		while (coder->left->is_taken || coder->left->queue[0] != coder->id
			|| timestamp_ms(coder->params->start) <= coder->left->end_of_cooldown)
			pthread_cond_wait(&coder->left->cond, &coder->left->mutex);
		coder->left->is_taken = 1;
	}
	else
	{
		pthread_mutex_lock(&coder->right->mutex);	
		while (coder->right->is_taken || coder->right->queue[0] != coder->id
			|| timestamp_ms(coder->params->start) <= coder->right->end_of_cooldown)
			pthread_cond_wait(&coder->right->cond, &coder->right->mutex);
		coder->right->is_taken = 1;
	}
}

static void	get_dongles(t_coder *coder)
{
	enfile(coder, coder->id % 2);
	get_dongle(coder, coder->id % 2);
	print_message(coder->params, "has taken a dongle", coder->id);
	enfile(coder, (coder->id % 2 + 1) % 2);
	get_dongle(coder, (coder->id % 2 + 1) % 2);
	print_message(coder->params, "has taken a dongle", coder->id);
}

void	release_dongle(t_coder *coder, int is_left)
{
	long		new_eoc;
	t_params	*params;

	params = coder->params;
	if (is_left)
	{
		pthread_mutex_lock(&coder->left->mutex);
		coder->left->is_taken = 0;
		new_eoc = timestamp_ms(params->start) + params->dongle_cooldown;
		coder->left->end_of_cooldown = new_eoc;
		coder->left->queue[0] = coder->left->queue[1];
		coder->left->queue[1] = -1;
		pthread_cond_broadcast(&coder->left->cond);
		pthread_mutex_unlock(&coder->left->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->right->mutex);
		coder->right->is_taken = 0;
		new_eoc = timestamp_ms(params->start) + params->dongle_cooldown;
		coder->right->end_of_cooldown = new_eoc;
		coder->right->queue[0] = coder->right->queue[1];
		coder->right->queue[1] = -1;
		pthread_cond_broadcast(&coder->right->cond);
		pthread_mutex_unlock(&coder->right->mutex);
	}
}

static void	release_dongles(t_coder *coder)
{
	release_dongle(coder, 0);
	release_dongle(coder, 1);
}

int	compile(t_coder *coder)
{
	get_dongles(coder);
	print_message(coder->params, "is compiling", coder->id);
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compile++;
	usleep(1000 * coder->params->time_to_compile);
	coder->last_compile = timestamp_ms(coder->params->start);
	coder->deadline = coder->last_compile
	+ coder->params->time_to_burnout;
	pthread_mutex_unlock(&coder->mutex);
	release_dongles(coder);
	return (1);
}
