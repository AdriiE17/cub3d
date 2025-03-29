/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:02:45 by victor            #+#    #+#             */
/*   Updated: 2025/03/29 13:25:00 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	parse_color(char *line, int color[3])
{
	int		i;
	char	*s;

	i = -1;
	s = line;
	while (++i < 3)
	{
		while (*s == ' ' || *s == '\t')
			s++;
		if (!*s)
			exit(write(1, "Error: Formato de color inválido\n", 33));
		color[i] = ft_atoi(s);
		while (*s >= '0' && *s <= '9')
			s++;
		if (color[i] < 0 || color[i] > 255)
			exit(write(1, "Error: Valores RGB fuera de rango (0-255)\n", 42));
		if (i < 2)
		{
			while (*s == ' ' || *s == '\t')
				s++;
			if (*s != ',')
				exit(write(1, "Error: Formato de color inválido\n", 33));
			s++;
		}
	}
}

static void	process_txt_col_map(char *line, t_game *game, char ***temp_map,
		int *lines)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		game->texture_no = ft_strdup(line + 3);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		game->texture_so = ft_strdup(line + 3);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		game->texture_we = ft_strdup(line + 3);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		game->texture_ea = ft_strdup(line + 3);
	else if (ft_strncmp(line, "F ", 2) == 0)
		parse_color(line + 2, game->floor_color);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_color(line + 2, game->ceiling_color);
	else if (line[0] == '1' || line[0] == ' ')
	{
		*temp_map = ft_realloc(*temp_map, (*lines + 1) * sizeof(char *));
		(*temp_map)[*lines] = ft_strdup(line);
		(*lines)++;
	}
}

void	process_lines(char **lines, int count, t_game *game, t_camera *camera)
{
	int		lines_count;
	char	**temp_map;
	int		i;

	lines_count = 0;
	temp_map = NULL;
	i = -1;
	while (++i < count)
		process_txt_col_map(lines[i], game, &temp_map, &lines_count);
	game->map = temp_map;
	game->map_height = lines_count;
	validate_map(game, camera);
}
