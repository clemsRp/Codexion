/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 16:21:04 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 06:18:37 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	get_state(t_params *params)
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

static int	wait_all_threads(t_params *params)
{
	int	res;

	res = get_state(params);
	while (res == 0)
	{
		res = get_state(params);
		usleep(1000);
	}
	if (res == -1)
		return (0);
	return (1);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (wait_all_threads(coder->params) == 0)
		return (NULL);
	pthread_mutex_lock(&coder->params->print_mutex);
	printf("id: %d\n", coder->id);
	pthread_mutex_unlock(&coder->params->print_mutex);
	return (NULL);
}

void	*monitoring_routine(void *arg)
{
	t_params	*params;

	params = (t_params *)arg;
	if (wait_all_threads(params) == 0)
		return (NULL);
	return (NULL);
}
