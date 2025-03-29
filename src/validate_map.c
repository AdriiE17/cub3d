/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:58:25 by victor            #+#    #+#             */
/*   Updated: 2025/03/28 23:51:29 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	check_empty_line(char *line)
{
	if (ft_strlen(line) == 0)
		exit(write(2, "Error\nEmpty map line\n", 21));
}

static void	validate_borders(char *line, int is_border)
{
	int	j;
	int	len;

	len = ft_strlen(line);
	if (is_border)
	{
		j = -1;
		while (++j < len)
			if (line[j] != '1' && line[j] != ' ')
				exit(write(2, "Error\nMap not closed at row XX, col YY\n", 39));
	}
	else
	{
		j = -1;
		while (++j < len && line[j] == ' ')
			;
		if (j < len && line[j] != '1')
			exit(write(2, "Error\nMap left open at row XX\n", 30));
		j = len;
		while (--j >= 0 && line[j] == ' ')
			;
		if (j >= 0 && line[j] != '1')
			exit(write(2, "Error\nMap right open at row XX\n", 31));
	}
}

static void	set_camera(t_camera *camera, char dir, int x, int y)
{
	camera->pos.x = x + 0.5;
	camera->pos.y = y + 0.5;
	if (dir == 'N')
	{
		camera->dir = (t_vec2){0, -1};
		camera->plane = (t_vec2){0.66, 0};
	}
	else if (dir == 'S')
	{
		camera->dir = (t_vec2){0, 1};
		camera->plane = (t_vec2){-0.66, 0};
	}
	else if (dir == 'E')
	{
		camera->dir = (t_vec2){1, 0};
		camera->plane = (t_vec2){0, 0.66};
	}
	else if (dir == 'W')
	{
		camera->dir = (t_vec2){-1, 0};
		camera->plane = (t_vec2){0, -0.66};
	}
}

static void	process_row_chars(t_game *g, t_camera *cam, int i, int *count)
{
	int		j;
	char	c;

	j = -1;
	while (g->map[i][++j])
	{
		c = g->map[i][j];
		if (!ft_strchr(" 01NSEW", c))
			exit(write(2, "Error\nInvalid character in map\n", 31));
		if (ft_strchr("NSEW", c))
		{
			set_camera(cam, c, j, i);
			(*count)++;
			g->map[i][j] = '0';
		}
	}
}

void	validate_map(t_game *game, t_camera *camera)
{
	int	i;
	int	spawn_count;

	spawn_count = 0;
	i = -1;
	while (++i < game->map_height)
	{
		check_empty_line(game->map[i]);
		validate_borders(game->map[i], (i == 0 || i == game->map_height - 1));
		process_row_chars(game, camera, i, &spawn_count);
	}
	if (spawn_count != 1)
		exit(write(2, "Error\nMap must have only one spawn point\n", 42));
}
