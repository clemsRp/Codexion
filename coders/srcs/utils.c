/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clement <clement@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 10:51:38 by crappo            #+#    #+#             */
/*   Updated: 2026/03/02 10:04:44 by clement          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fill_coder(t_params *params, int i, int ind)
{
	int	mutex_res;

	params->coders[i].last_compile = 0;
	params->coders[i].deadline = params->time_to_burnout;
	params->coders[i].id = i + 1;
	params->coders[i].nb_compile = 0;
	params->coders[i].params = params;
	params->coders[i].left = &params->dongles[i];
	params->coders[i].right = &params->dongles[ind];
	mutex_res = pthread_mutex_init(&params->coders[i].mutex, NULL);
	params->coders[i].mutex_res = mutex_res;
}

long	timestamp_ms(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start);
}

void	print_message(t_params *params, char *message, int coder_id)
{
	pthread_mutex_lock(&params->print_mutex);
	if (get_is_running(params) || strcmp(message, "burned out") == 0)
		printf("%ld %d %s\n", timestamp_ms(params->start), coder_id, message);
	pthread_mutex_unlock(&params->print_mutex);
}
