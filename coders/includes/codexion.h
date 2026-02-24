/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:42:30 by crappo            #+#    #+#             */
/*   Updated: 2026/02/24 06:16:41 by crappo           ###   ########.fr       */
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

typedef struct s_params	t_params;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_dongle
{
	long			end_of_cooldown;
	int				id;
	int				is_taken;
	int				queue[2];
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_dongle;

typedef struct s_coder
{
	long			last_compile;
	long			deadline;
	int				id;
	int				nb_compile;
	t_params		*params;
	t_dongle		*left;
	t_dongle		*right;
	pthread_t		thread;
	pthread_mutex_t	mutex;
}				t_coder;

typedef struct s_params
{
	long			start;
	int				is_running;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				state;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
}				t_params;

// Init
void	init_params(t_params *params, char *argv[]);
int		is_valid_input(int argc, char *argv[]);
int		init_datas(t_params *params);

// Moves
long	timestamp_ms(long start);
void	print_message(pthread_mutex_t mutex, char *message,
			long start, int coder_id);

// Routines
void	*routine(void *arg);
void	*monitoring_routine(void *arg);

// Free
int		free_all(t_params *params);

#endif