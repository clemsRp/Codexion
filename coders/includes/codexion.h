/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:42:30 by crappo            #+#    #+#             */
/*   Updated: 2026/02/07 11:09:19 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_params
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}				t_params;

typedef struct s_dongle
{
	long			end_of_cooldown;
	int				dongle_id;
	int				is_taken;
	int				*queue;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_dongle;

typedef struct s_coder
{
	long			last_compile;
	int				coder_id;
	int				nb_compile;
	int				id_dongle_left;
	int				id_dongle_right;
	pthread_t		thread;
	pthread_mutex_t	mutex;
}				t_coder;

// Init
int			is_valid_input(int argc, char *argv[]);
void		init_params(t_params *params, char *argv[]);
void		*init_variables(t_dongle **dongles, t_coder **coders,
				t_params params, long start);

// Moves
long		timestamp_ms(long start);

#endif