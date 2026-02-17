/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crappo <crappo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:29:07 by crappo            #+#    #+#             */
/*   Updated: 2026/01/22 13:29:17 by crappo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_positive(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	is_integer(char *str)
{
	if (strlen(str) > 10)
		return (0);
	if (strcmp(str, "2147483647") > 0 && strlen(str) == 10)
		return (0);
	return (1);
}

int	is_valid_input(int argc, char *argv[])
{
	int	i;

	if (argc != 9)
		return (0);
	i = 1;
	while (i < argc - 1)
	{
		if (is_positive(argv[i]) == 0
			|| is_integer(argv[i]) == 0)
			return (0);
		i++;
	}
	if (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0)
		return (0);
	return (1);
}
