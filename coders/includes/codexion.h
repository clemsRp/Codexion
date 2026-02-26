/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:42:30 by crappo            #+#    #+#             */
/*   Updated: 2026/02/25 15:31:40 by crappo           ###   ########.fr       */
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
	int				mutex_res;
	pthread_cond_t	cond;
	int				cond_res;
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
	int				thread_res;
	pthread_mutex_t	mutex;
	int				mutex_res;
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
	pthread_t		monitor_thread;
	int				monitor_thread_res;
	pthread_mutex_t	print_mutex;
	int				print_res;
	pthread_mutex_t	state_mutex;
	int				state_res;
}				t_params;

// Init
void	init_params(t_params *params, char *argv[]);
int		is_valid_input(int argc, char *argv[]);
int		init_datas(t_params *params);

// Moves
long	timestamp_ms(long start);
int		compile(t_coder *coder);
void	print_message(t_params *params, char *message, int coder_id);
void	release_dongle(t_coder *coder, int is_left);

// Routines
void	*routine(void *arg);
void	*monitoring_routine(void *arg);
int		wait_all_threads(t_params *params);

// Utils
void	fill_coder(t_params *params, int i, int ind);

// Get
int		get_state(t_params *params);
int		get_is_running(t_params *params);

// Free
int		free_all(t_params *params);

// Enfile
void	enfile(t_coder *coder, int is_left);

#endif