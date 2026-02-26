/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enfile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:14:33 by crappo            #+#    #+#             */
/*   Updated: 2026/02/25 15:09:01 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	enfile_fifo(t_coder *coder, int is_left)
{
	if (is_left && coder->left->queue[0] == -1)
		coder->left->queue[0] = coder->id;
	else if (is_left && coder->left->queue[1] == -1)
		coder->left->queue[1] = coder->id;
	else if (!is_left && coder->right->queue[0] == -1)
		coder->right->queue[0] = coder->id;
	else if (!is_left && coder->right->queue[1] == -1)
		coder->right->queue[1] = coder->id;
}

static void	enfile_edf_left(t_coder *coders, t_coder *coder)
{
	int	cur_coder_id;

	if (coder->left->queue[0] == -1)
		coder->left->queue[0] = coder->id;
	else if (coder->left->queue[1] == -1)
	{
		cur_coder_id = coder->left->queue[0];
		if (coders[cur_coder_id].deadline < coder->deadline)
			coder->left->queue[1] = coder->id;
		else
		{
			coder->left->queue[1] = cur_coder_id;
			coder->left->queue[0] = coder->id;
		}
	}
}

static void	enfile_edf_right(t_coder *coders, t_coder *coder)
{
	int	cur_coder_id;

	if (coder->right->queue[0] == -1)
		coder->right->queue[0] = coder->id;
	else if (coder->right->queue[1] == -1)
	{
		cur_coder_id = coder->right->queue[0];
		if (coders[cur_coder_id].deadline < coder->deadline)
			coder->right->queue[1] = coder->id;
		else
		{
			coder->right->queue[1] = cur_coder_id;
			coder->right->queue[0] = coder->id;
		}
	}
}

static void	enfile_edf(t_coder *coder, int is_left)
{
	if (is_left)
		enfile_edf_left(coder->params->coders, coder);
	else
		enfile_edf_right(coder->params->coders, coder);
}

void	enfile(t_coder *coder, int is_left)
{
	if (coder->params->scheduler == FIFO)
		enfile_fifo(coder, is_left);
	else
		enfile_edf(coder, is_left);
}
