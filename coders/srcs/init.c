/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:47:20 by crappo            #+#    #+#             */
/*   Updated: 2026/02/07 11:10:50 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_params(t_params *params, char *argv[])
{
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = 1000 * atoi(argv[2]);
	params->time_to_compile = 1000 * atoi(argv[3]);
	params->time_to_debug = 1000 * atoi(argv[4]);
	params->time_to_refactor = 1000 * atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	params->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = FIFO;
	else
		params->scheduler = EDF;
}

static int	init_dongles(t_dongle *dongles, t_params params, long start)
{
	int	i;

	i = 0;
	while (i < params.number_of_coders)
	{
		dongles[i].dongle_id = i;
		dongles[i].is_taken = 0;
		dongles[i].end_of_cooldown = start;
		dongles[i].queue = malloc(sizeof(int) * 2);
		if (!dongles[i].queue)
			return (0);
		dongles[i].queue[0] = -1;
		dongles[i].queue[1] = -1;
		if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
			return (0);
		if (pthread_cond_init(&dongles[i].cond, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	init_coders(t_dongle *dongles, t_coder *coders,
				t_params params, long start)
{
	int	i;

	i = 0;
	while (i < params.number_of_coders)
	{
		coders[i].coder_id = i;
		coders[i].last_compile = start;
		coders[i].nb_compile = params.number_of_compiles_required;
		coders[i].id_dongle_left = i;
		coders[i].id_dongle_right = (i + 1) % params.number_of_coders;
		if (pthread_mutex_init(&coders[i].mutex, NULL) != 0)
			return (0);
		if ()
		i++;
	}
	return (1);
}

void	*init_variables(t_dongle **dongles, t_coder **coders,
			t_params params, long start)
{
	*dongles = malloc(sizeof(t_dongle) * params.number_of_coders);
	*coders = malloc(sizeof(t_coder) * params.number_of_coders);
	if (!*dongles || !*coders)
		return (NULL);
	if (init_dongles(*dongles, params, start) == 0)
		return (NULL);
	if (init_coders(*dongles, *coders, params, start) == 0)
		return (NULL);
	return (coders);
}
