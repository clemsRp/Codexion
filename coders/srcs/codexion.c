/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:30:38 by crappo            #+#    #+#             */
/*   Updated: 2026/02/25 14:35:23 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	basic_values(t_params params)
{
	if (params.number_of_coders == 0)
		return (0);
	if (params.number_of_coders == 1)
	{
		printf("0 0 has taken a dongle");
		usleep(params.time_to_burnout);
		printf("%ld 0 burned out\n", timestamp_ms(params.start));
		return (0);
	}
	return (1);
}

static void	algo(t_params *params)
{
	int	i;

	i = 0;
	wait_all_threads(params);
	while (i < params->number_of_coders)
	{
		if (params->coders[i].thread_res == 0)
			pthread_join(params->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_params	params;

	if (is_valid_input(argc, argv) == 0)
	{
		printf("Invalid arguments.\n");
		return (0);
	}
	init_params(&params, argv);
	if (basic_values(params) == 0)
		return (0);
	if (init_datas(&params) == 0)
		return (free_all(&params));
	algo(&params);
	return (free_all(&params));
}
