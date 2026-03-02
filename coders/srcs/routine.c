/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement <clement@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 16:21:04 by crappo            #+#    #+#             */
/*   Updated: 2026/03/02 11:51:57 by clement          ###   ########.fr       */
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
	t_coder	*coder = (t_coder *)arg;

	if (wait_all_threads(coder->params) < 0)
		return (NULL);
	if (coder->id % 2)
		usleep(1000);
	while (get_is_running(coder->params))
	{
		if (!compile(coder) || !get_is_running(coder->params))
			break;
		print_message(coder->params, "is debugging", coder->id);
		usleep(coder->params->time_to_debug * 1000);
		if (!get_is_running(coder->params))
			break;
		print_message(coder->params, "is refactoring", coder->id);
		usleep(coder->params->time_to_refactor * 1000);
	}
	return (NULL);
}

static void	burned_out(t_params *params, int index)
{
	int	j;

	print_message(params, "burned out", index);
	pthread_mutex_lock(&params->state_mutex);
	params->is_running = 0;
	pthread_mutex_unlock(&params->state_mutex);
	j = 0;
	while (j < params->number_of_coders)
	{
		pthread_mutex_lock(&params->dongles[j].mutex);
		pthread_cond_broadcast(&params->dongles[j].cond);
		pthread_mutex_unlock(&params->dongles[j].mutex);
		j++;
	}
	pthread_mutex_unlock(&params->coders[index].mutex);
	return;
}

static void	check_status(t_params *params)
{
	int i;
	int finished;

	i = 0;
	finished = 0;
	while (i < params->number_of_coders)
	{
		pthread_mutex_lock(&params->coders[i].mutex);
		if (timestamp_ms(params->start) >= params->coders[i].deadline)
			return (burned_out(params, i));
		if (params->coders[i].nb_compile >= params->number_of_compiles_required)
			finished++;
		pthread_mutex_unlock(&params->coders[i].mutex);
		i++;
	}
	if (finished == params->number_of_coders)
	{
		pthread_mutex_lock(&params->state_mutex);
		params->is_running = 0;
		pthread_mutex_unlock(&params->state_mutex);
	}
}

void	*monitoring_routine(void *arg)
{
	int		i;
	t_params *params;
	
	i = 0;
	params = (t_params *)arg;
	wait_all_threads(params);
	while (get_is_running(params))
	{
		check_status(params);
		while (i < params->number_of_coders)
			pthread_cond_broadcast(&params->dongles[i++].cond);
		usleep(1000);
	}
	return (NULL);
}
