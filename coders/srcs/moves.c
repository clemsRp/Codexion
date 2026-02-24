/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 09:20:37 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 02:55:15 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	timestamp_ms(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start);
}

void	print_message(pthread_mutex_t mutex, char *message,
			long start, int coder_id)
{
	pthread_mutex_lock(&mutex);
	printf("%ld %d %s\n", timestamp_ms(start), coder_id, message);
	pthread_mutex_unlock(&mutex);
}
