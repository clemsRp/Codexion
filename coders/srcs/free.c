/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 13:41:57 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 04:28:30 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_dongles(t_params *params)
{
	int	i;

	if (!params->dongles)
		return ;
	i = 0;
	while (i < params->number_of_coders)
	{
		pthread_mutex_destroy(&params->dongles[i].mutex);
		pthread_cond_destroy(&params->dongles[i].cond);
		i++;
	}
	free(params->dongles);
}

static void	free_coders(t_params *params)
{
	int	i;

	if (!params->coders)
		return ;
	i = 0;
	while (i < params->number_of_coders)
		pthread_mutex_destroy(&params->coders[i++].mutex);
	free(params->coders);
}

int	free_all(t_params *params)
{
	free_dongles(params);
	free_coders(params);
	pthread_mutex_destroy(&params->print_mutex);
	pthread_mutex_destroy(&params->state_mutex);
	return (0);
}
