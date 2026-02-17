/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:30:38 by crappo            #+#    #+#             */
/*   Updated: 2026/02/09 14:36:44 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_params(t_params *params)
{
	const char	*type[] = {"fifo", "edf"};

	printf("%d ", params->number_of_coders);
	printf("%d ", params->time_to_burnout);
	printf("%d ", params->time_to_compile);
	printf("%d ", params->time_to_debug);
	printf("%d ", params->time_to_refactor);
	printf("%d ", params->number_of_compiles_required);
	printf("%d ", params->dongle_cooldown);
	printf("%s\n", type[(int)params->scheduler]);
}

static int	test(t_params params, long start)
{
	if (params.number_of_coders == 0)
		return (0);
	if (params.number_of_coders == 1)
	{
		usleep(params.time_to_burnout);
		printf("%ld 1 burned out\n", timestamp_ms(start));
		return (0);
	}
	return (1);
}

static int	free_all(t_dongle *dongles, t_coder *coders, t_params params)
{
	int	i;

	while (i < params.number_of_coders)
	{
		if (dongles)
		{
			pthread_mutex_destroy(&dongles[i].mutex);
			pthread_cond_destroy(&dongles[i].cond);
		}
		if (coders)
			pthread_mutex_destroy(&coders[i].mutex);
		i++;
	}
	if (dongles)
		free(dongles);
	if (coders)
		free(coders);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_params	params;
	t_dongle	*dongles;
	t_coder		*coders;
	long		start;

	dongles = NULL;
	coders = NULL;
	if (is_valid_input(argc, argv) == 0)
	{
		printf("Invalid arguments.\n");
		return (0);
	}
	init_params(&params, argv);
	start = timestamp_ms(0);
	if (test(params, start) == 0)
		return (0);
	if (!init_variables(&dongles, &coders, params, start))
		return (free_all(dongles, coders, params));
	return (free_all(dongles, coders, params));
}
