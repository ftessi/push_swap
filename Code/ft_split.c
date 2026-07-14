/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 21:17:19 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/14 16:06:01 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	element_counter(char const *str, char c)
{
	int	count;
	int	in_element;

	in_element = 0;
	count = 0;
	while (*str)
	{
		if (in_element == 0 && *str != c)
		{
			in_element = 1;
			count++;
		}
		else if (*str == c)
			in_element = 0;
		str++;
	}
	return (count);
}

static void	free_all(char **res, int i)
{
	while (i > 0)
		free(res[--i]);
	free(res);
}

static char	*allocate_element(const char *str, int len)
{
	char	*new_element;
	int		i;

	new_element = malloc(len + 1);
	if (!new_element)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_element[i] = str[i];
		i++;
	}
	new_element[i] = '\0';
	return (new_element);
}

static int	fill_the_res(char **res, char const *str, char c)
{
	int	i;
	int	len;

	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			len = 0;
			while (str[len] && str[len] != c)
				len++;
			res[i] = allocate_element(str, len);
			if (!res[i])
			{
				free_all(res, i);
				return (0);
			}
			str += len;
			i++;
		}
		else
			str++;
	}
	res[i] = NULL;
	return (1);
}

char	**ft_split(char const *str, char c)
{
	char	**res;

	if (!str)
		return (NULL);
	res = malloc(sizeof(char *) * (element_counter(str, c) + 1));
	if (!res)
		return (NULL);
	if (!fill_the_res(res, str, c))
		return (NULL);
	return (res);
}
