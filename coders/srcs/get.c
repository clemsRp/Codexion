/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 08:51:05 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 08:51:38 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_state(t_params *params)
{
	int	res;

	pthread_mutex_lock(&params->state_mutex);
	if (params->state < params->number_of_coders && params->state != -1)
		res = 0;
	else if (params->state == params->number_of_coders)
		res = 1;
	else
		res = -1;
	pthread_mutex_unlock(&params->state_mutex);
	return (res);
}

int	get_is_running(t_params *params)
{
	int	res;

	pthread_mutex_lock(&params->state_mutex);
	res = params->is_running;
	pthread_mutex_unlock(&params->state_mutex);
	return (res);
}
