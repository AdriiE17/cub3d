/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:51:23 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 18:43:59 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	init_door(t_game *g, int i, int j, char c)
{
	g->door_count++;
	g->doors = realloc(g->doors, sizeof(t_door) * g->door_count);
	g->doors[g->door_count - 1] = (t_door){
		.x = j,
		.y = i,
		.is_open = 0,
		.orientation = c,
		.open_offset = 0.0,
		.move_progress = DOOR_ANIM_DURATION
	};
	g->map[i][j] = c;
}

int	check_door_collision(t_app *app, t_ray *ray)
{
	int	i;

	i = -1;
	while (++i < app->game.door_count)
	{
		if (app->game.doors[i].x == ray->map_x
			&& app->game.doors[i].y == ray->map_y)
			return (!(app->game.doors[i].is_open
					&& app->game.doors[i].move_progress >= 0.2));
	}
	return (1);
}

void	update_door_animation(t_app *app, double dt)
{
	int		i;
	double	t;
	t_door	*door;

	i = -1;
	while (++i < app->game.door_count)
	{
		door = &app->game.doors[i];
		if (door->move_progress < DOOR_ANIM_DURATION)
		{
			door->move_progress += dt;
			if (door->move_progress > DOOR_ANIM_DURATION)
				door->move_progress = DOOR_ANIM_DURATION;
			t = door->move_progress / DOOR_ANIM_DURATION;
			if (door->is_open)
				door->open_offset = t;
			else
				door->open_offset = 1.0 - t;
		}
	}
}

static void	try_open_door(t_app *app, t_ray *ray)
{
	int		i;
	t_door	*door;

	i = -1;
	while (++i < app->game.door_count)
	{
		door = &app->game.doors[i];
		if (door->x == ray->map_x && door->y == ray->map_y)
		{
			door->is_open = 1;
			door->move_progress = 0.0;
			break ;
		}
	}
}

void	toggle_doors(t_app *app)
{
	static int	last_space = 0;
	int			current_space;
	t_ray		ray;

	current_space = mlx_is_key_down(app->mlx, MLX_KEY_SPACE);
	if (current_space && !last_space)
	{
		init_ray(app, WIDTH / 2, &ray);
		do_dda(app, &ray);
		if (ray.perpwalldist < 1.5
			&& (ray.hit_tile == '2' || ray.hit_tile == '3'))
			try_open_door(app, &ray);
	}
	last_space = current_space;
}
