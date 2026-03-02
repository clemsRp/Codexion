/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement <clement@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:14:33 by crappo            #+#    #+#             */
/*   Updated: 2026/02/28 11:10:12 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	add_fifo(t_dongle *dongle, int coder_id)
{
	if (dongle->queue[0] == -1)
		dongle->queue[0] = coder_id;
	else if (dongle->queue[1] == -1)
		dongle->queue[1] = coder_id;
}

static void	add_edf(t_coder *coders, t_dongle *dongle, int coder_id)
{
	int	cur_id;

	if (dongle->queue[0] == -1)
		dongle->queue[0] = coder_id;
	else if (dongle->queue[1] == -1)
	{
		cur_id = dongle->queue[0];
		if (coders[cur_id].deadline < coders[coder_id].deadline)
			dongle->queue[1] = coder_id;
		else
		{
			dongle->queue[1] = cur_id;
			dongle->queue[0] = coder_id;
		}
	}
}

void	add(t_coder *coder, int is_left)
{
	t_dongle *d;
	
	d = coder->right;
	if (is_left)
		d = coder->left;
	pthread_mutex_lock(&d->mutex);
	if (d->queue[0] == coder->id || d->queue[1] == coder->id)
	{
		pthread_mutex_unlock(&d->mutex);
		return ;
	}
	if (coder->params->scheduler == FIFO)
		add_fifo(d, coder->id);
	else
		add_edf(coder->params->coders, d, coder->id);
	pthread_mutex_unlock(&d->mutex);
}
