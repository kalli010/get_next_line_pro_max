/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelkalai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 12:02:32 by zelkalai          #+#    #+#             */
/*   Updated: 2024/02/23 14:56:51 by zelkalai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	newline_checker(t_list *list)
{
	int	i;

	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			if (list->content[i] == '\n')
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}

int	line_length(t_list *list)
{
	int	len;
	int	i;

	len = 0;
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			if (list->content[i] == '\n')
				return (len + 1);
			i++;
			len++;
		}
		list = list->next;
	}
	return (len);
}

void	copy_line(t_list *list, char *the_line)
{
	int	j;
	int	i;

	j = 0;
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			the_line[j++] = list->content[i];
			if (list->content[i] == '\n')
			{
				the_line[j] = '\0';
				return ;
			}
			i++;
		}
		list = list->next;
	}
	the_line[j] = '\0';
}

void	clean_list(t_list **list)
{
	char	*buf;
	int		i;
	int		k;
	t_list	*clean_node;
	t_list	*last;
	t_list	*tmp;

	buf = malloc(BUFFER_SIZE + 1);
	clean_node = malloc(sizeof(t_list));
	if (!buf || !clean_node)
		return ;
	last = *list;
	while (last && last->next)
		last = last->next;
	i = 0;
	k = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	while (last->content[i] && last->content[++i])
		buf[k++] = last->content[++i];
	buf[k] = '\0';
	clean_node->content = buf;
	clean_node->next = NULL;
	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->content);
		free(*list);
		*list = tmp;
	}
	if (clean_node->content[0])
		*list = clean_node;
	else
		*list = NULL;
	if (!*list)
	{
		free(clean_node);
		free(buf);
	}
}

void	add_to_list(t_list **list, char *buf)
{
	t_list	*new_node;
	t_list	*last_node;

	new_node = malloc(sizeof(t_list));
	new_node->content = buf;
	new_node->next = NULL;
	if (*list == NULL)
		*list = new_node;
	else
	{
		last_node = *list;
		while (last_node->next != NULL)
		{
			last_node = last_node->next;
		}
		last_node->next = new_node;
	}
}

char	*get_next_line(int fd)
{
	static t_list	*list;
	char			*buf;
	ssize_t			r;
	int				len;
	char			*line;

	list = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (!newline_checker(list))
	{
		buf = malloc(BUFFER_SIZE + 1);
		if (!buf)
			return (NULL);
		if ((r = read(fd, buf, BUFFER_SIZE)) <= 0)
		{
			free(buf);
			break ;
		}
		buf[r] = '\0';
		add_to_list(&list, buf);
	}
	if (!list)
		return (NULL);
	len = line_length(list);
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	copy_line(list, line);
	clean_list(&list);
	return (line);
}
