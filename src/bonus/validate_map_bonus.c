/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:58:25 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 21:47:07 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*   Removes empty lines from the bottom of the map.                         */
/*                                                                            */
/*   - Iterates over the map lines from the bottom.                           */
/*   - Trims the line to remove spaces and checks if the line is empty.      */
/*   - If the line is empty, it frees the memory of the line and decreases    */
/*     the map height.                                                       */
/*   - Stops when a non-empty line is encountered or the whole map is empty. */
/*                                                                            */
/* ************************************************************************** */
static void	check_empty_line(t_game *game)
{
	char	*trimmed;

	while (game->map_height > 0)
	{
		trimmed = ft_strtrim(game->map[game->map_height - 1], " ");
		if (ft_strlen(trimmed) == 0)
		{
			free(game->map[game->map_height - 1]);
			game->map[game->map_height - 1] = NULL;
			game->map_height--;
		}
		else
		{
			free(trimmed);
			break ;
		}
		free(trimmed);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Validates the borders of each row in the map.                            */
/*                                                                            */
/*   - If `is_border` is true, it checks if the entire line is composed of    */
/*     '1' (wall) or spaces.                                                  */
/*   - If `is_border` is false, it ensures the line starts and ends with      */
/*     '1' (wall) after trimming spaces from the left and right.              */
/*   - Exits the program with an error if any of these conditions are violated*/
/*                                                                            */
/* ************************************************************************** */
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
				exit(write(2, "Error\nMap is not closed\n", 25));
	}
	else
	{
		j = -1;
		while (++j < len && line[j] == ' ')
			;
		if (j < len && line[j] != '1')
			exit(write(2, "Error\nMap is left open\n", 24));
		j = len;
		while (--j >= 0 && line[j] == ' ')
			;
		if (j >= 0 && line[j] != '1')
			exit(write(2, "Error\nMap is right open\n", 25));
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Sets the camera's position and direction based on the map.              */
/*                                                                            */
/*   - Based on the character ('N', 'S', 'E', 'W'), it sets the camera's     */
/*     position to be at the center of the corresponding grid cell.          */
/*   - The direction vector is set according to the cardinal direction.      */
/*     - North ('N') has direction (0, -1) and plane (0.66, 0).               */
/*     - South ('S') has direction (0, 1) and plane (-0.66, 0).              */
/*     - East ('E') has direction (1, 0) and plane (0, 0.66).                */
/*     - West ('W') has direction (-1, 0) and plane (0, -0.66).              */
/*   - The camera's direction vector defines which way it faces.             */
/*   - The plane vector helps in calculating the view direction for the ray. */
//*    double	fov; fov = 60 * (M_PI / 180);// dir * tan(FOV/2), dir = 1    */
/* ************************************************************************** */
static void	set_camera(t_camera *camera, char dir, int x, int y)
{
	double	plane;

	plane = tan(86 * (M_PI / 180) / 2);
	camera->pos.x = x + 0.5;
	camera->pos.y = y + 0.5;
	if (dir == 'N')
	{
		camera->dir = (t_vec2){0, -1};
		camera->plane = (t_vec2){plane, 0};
	}
	else if (dir == 'S')
	{
		camera->dir = (t_vec2){0, 1};
		camera->plane = (t_vec2){-plane, 0};
	}
	else if (dir == 'E')
	{
		camera->dir = (t_vec2){1, 0};
		camera->plane = (t_vec2){0, plane};
	}
	else if (dir == 'W')
	{
		camera->dir = (t_vec2){-1, 0};
		camera->plane = (t_vec2){0, -plane};
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Processes each character in a row of the map.                            */
/*                                                                            */
/*   - For each row, checks whether each character is valid                   */
/*     (' ', '0', '1', 'N', 'S', 'E', 'W').                                   */
/*   - If a spawn character ('N', 'S', 'E', or 'W') is found, the camera's    */
/*     position and direction are set accordingly.                            */
/*   - Replaces the spawn character with '0' to mark the cell as empty.       */
/*   - Exits the program with an error if an invalid character is found.      */
/*                                                                            */
/* ************************************************************************** */
static void	process_row_chars(t_game *g, t_camera *cam, int i, int *count)
{
	int		j;
	char	c;

	j = -1;
	while (g->map[i][++j])
	{
		c = g->map[i][j];
		if (!ft_strchr(" 01NSEW23B", c))
			exit(write(2, "Error\nInvalid character in map\n", 31));
		if (ft_strchr("NSEW", c))
		{
			set_camera(cam, g->map[i][j], j, i);
			(*count)++;
			g->map[i][j] = '0';
		}
		else if (c == '2' || c == '3')
			init_door(g, i, j, c);
		else if (c == 'B')
			init_enemy(g, i, j);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Validates the map by checking for empty lines, borders, and spawn.       */
/*                                                                            */
/*   - Calls `check_empty_line()` to remove empty lines at the bottom.        */
/*   - Ensures that the map borders are closed and valid `validate_borders()` */
/*   - Processes each row for valid characters and checks for exact one spawn */
/*   - Exits with an error if any conditions are violated.                    */
/*                                                                            */
/* ************************************************************************** */
void	validate_map(t_game *game, t_camera *camera)
{
	int		i;
	int		spawn_count;

	check_empty_line(game);
	spawn_count = 0;
	camera->view_z = 0;
	i = -1;
	while (++i < game->map_height)
	{
		if (ft_strlen(game->map[i]) == 0)
			exit(write(2, "Error\nEmpty map line\n", 21));
		validate_borders(game->map[i], (i == 0 || i == game->map_height - 1));
		process_row_chars(game, camera, i, &spawn_count);
	}
	if (spawn_count != 1)
		exit(write(2, "Error\nMap must have only one spawn point\n", 42));
}
