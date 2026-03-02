/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement <clement@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 11:13:17 by clement           #+#    #+#             */
/*   Updated: 2026/02/28 11:22:35 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void release_left(t_coder *coder)
{
    long		new_eoc;
	t_params	*params;

	params = coder->params;
    pthread_mutex_lock(&coder->left->mutex);
	coder->left->is_taken = 0;
	new_eoc = timestamp_ms(params->start) + params->dongle_cooldown;
	coder->left->end_of_cooldown = new_eoc;
	coder->left->queue[0] = coder->left->queue[1];
	coder->left->queue[1] = -1;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->mutex);
}

static void release_right(t_coder *coder)
{
    long		new_eoc;
	t_params	*params;

	params = coder->params;
	pthread_mutex_lock(&coder->right->mutex);
	coder->right->is_taken = 0;
	new_eoc = timestamp_ms(params->start) + params->dongle_cooldown;
	coder->right->end_of_cooldown = new_eoc;
	coder->right->queue[0] = coder->right->queue[1];
	coder->right->queue[1] = -1;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->mutex);
}

void	release_dongles(t_coder *coder)
{
	release_left(coder);
	release_right(coder);
}
