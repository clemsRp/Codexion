/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:47:20 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 06:17:39 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_params(t_params *params, char *argv[])
{
	params->start = timestamp_ms(0);
	params->is_running = 1;
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = atoi(argv[2]);
	params->time_to_compile = atoi(argv[3]);
	params->time_to_debug = atoi(argv[4]);
	params->time_to_refactor = atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	params->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = FIFO;
	else
		params->scheduler = EDF;
	params->state = 0;
	params->dongles = NULL;
	params->coders = NULL;
}

static int	init_dongles(t_params *params)
{
	int	i;

	i = 0;
	while (i > params->number_of_coders)
	{
		params->dongles[i].end_of_cooldown = 0;
		params->dongles[i].id = i;
		params->dongles[i].is_taken = 0;
		params->dongles[i].queue[0] = -1;
		params->dongles[i].queue[1] = -1;
		if (pthread_mutex_init(&params->dongles[i].mutex, NULL) != 0)
			return (0);
		if (pthread_cond_init(&params->dongles[i].cond, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	set_error(t_params *params)
{
	pthread_mutex_lock(&params->state_mutex);
	params->state = -1;
	pthread_mutex_unlock(&params->state_mutex);
	return (0);
}

static int	init_coders(t_params *params)
{
	int	i;
	int	ind;

	i = 0;
	while (i < params->number_of_coders)
	{
		ind = (i + 1) % params->number_of_coders;
		params->coders[i].last_compile = 0;
		params->coders[i].deadline = params->time_to_burnout;
		params->coders[i].id = i;
		params->coders[i].nb_compile = 0;
		params->coders[i].params = params;
		params->coders[i].left = &params->dongles[i];
		params->coders[i].right = &params->dongles[ind];
		if (pthread_mutex_init(&params->coders[i].mutex, NULL) != 0)
			return (0);
		if (pthread_create(&params->coders[i].thread,
				NULL, &routine, (void *)&params->coders[i]) != 0)
			return (set_error(params));
		pthread_mutex_lock(&params->state_mutex);
		params->state++;
		pthread_mutex_unlock(&params->state_mutex);
		i++;
	}
	return (1);
}

int	init_datas(t_params *params)
{
	params->dongles = malloc(sizeof(t_dongle) * params->number_of_coders);
	params->coders = malloc(sizeof(t_coder) * params->number_of_coders);
	if (!params->dongles || !params->coders)
		return (0);
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&params->state_mutex, NULL) != 0)
		return (0);
	if (init_dongles(params) == 0)
		return (0);
	if (init_coders(params) == 0)
		return (0);
	return (1);
}
