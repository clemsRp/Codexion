/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 16:21:04 by crappo            #+#    #+#             */
/*   Updated: 2026/02/25 17:23:40 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	wait_all_threads(t_params *params)
{
	int	res;

	res = get_state(params);
	while (res == 0)
	{
		res = get_state(params);
		usleep(1000);
	}
	if (res < 0)
		return (-1);
	return (1);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (get_state(coder->params) < 0)
		return (NULL);
	while (get_is_running(coder->params))
	{
		compile(coder);
		if (!get_is_running(coder->params))
			return (NULL);
		print_message(coder->params, "is debugging", coder->id);
		usleep(1000 * coder->params->time_to_debug);
		if (!get_is_running(coder->params))
			return (NULL);
		print_message(coder->params, "is refactoring", coder->id);
		usleep(1000 * coder->params->time_to_refactor);
	}
	return (NULL);
}

static void	update_is_running(t_params *params)
{
	int	i;
	int	j;
	int	nb_complete_compile;

	i = 0;
	nb_complete_compile = 0;
	pthread_mutex_lock(&params->state_mutex);
	while (i < params->number_of_coders)
	{
		pthread_mutex_lock(&params->coders[i].mutex);
		if (params->coders[i].nb_compile
			>= params->number_of_compiles_required)
			nb_complete_compile++;
		if (timestamp_ms(params->start)
			>= params->coders[i].deadline)
			{
				print_message(params, "burned out", i);
				j = 0;
				params->is_running = 0;
				while (j < params->number_of_coders)
				{
					pthread_mutex_lock(&params->dongles[j].mutex);
					pthread_cond_broadcast(&params->dongles[j++].cond);
					pthread_mutex_unlock(&params->dongles[j - 1].mutex);
				}
				pthread_mutex_unlock(&params->coders[i].mutex);
				pthread_mutex_unlock(&params->state_mutex);
				return ;
			}
		pthread_mutex_unlock(&params->coders[i].mutex);
		i++;
	}
	if (nb_complete_compile == params->number_of_coders)
		params->is_running = 0;
	pthread_mutex_unlock(&params->state_mutex);
}

void	*monitoring_routine(void *arg)
{
	t_params	*params;

	params = (t_params *)arg;
	if (get_state(params) < 0)
		return (NULL);
	while (get_is_running(params))
	{
		update_is_running(params);
		usleep(1000);
	}
	return (NULL);
}
