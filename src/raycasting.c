/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:50:51 by victor            #+#    #+#             */
/*   Updated: 2025/03/29 13:56:23 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_ray(t_app *app, int x, t_ray *ray)
{
	double	camx;

	camx = 2 * x / (double)WIDTH - 1;
	ray->raydir.x = app->camera.dir.x + app->camera.plane.x * camx;
	ray->raydir.y = app->camera.dir.y + app->camera.plane.y * camx;
	ray->map_x = (int)app->camera.pos.x;
	ray->map_y = (int)app->camera.pos.y;
	ray->deltadist.x = fabs(1 / ray->raydir.x);
	ray->deltadist.y = fabs(1 / ray->raydir.y);
}

static t_vec2	init_step(double pos, int map, double deltadist, double raydir)
{
	t_vec2	ret;

	if (raydir < 0)
	{
		ret.x = -1;
		ret.y = (pos - map) * deltadist;
	}
	else
	{
		ret.x = 1;
		ret.y = ((map + 1.0) - pos) * deltadist;
	}
	return (ret);
}

static void	dda_loop(t_app *app, t_ray *ray)
{
	while (1)
	{
		if (ray->sidedist.x < ray->sidedist.y)
		{
			ray->sidedist.x += ray->deltadist.x;
			ray->map_x += ray->step.x;
			ray->side = 0;
		}
		else
		{
			ray->sidedist.y += ray->deltadist.y;
			ray->map_y += ray->step.y;
			ray->side = 1;
		}
		if (ray->map_y < 0
			|| ray->map_y >= app->game.map_height
			|| ray->map_x < 0
			|| ray->map_x >= (int)ft_strlen(app->game.map[ray->map_y])
			|| safe_get_tile(&app->game, ray->map_x, ray->map_y) == '1')
			break ;
	}
}

void	do_dda(t_app *app, t_ray *ray)
{
	t_vec2	steps;

	steps = init_step(app->camera.pos.x, ray->map_x, ray->deltadist.x,
			ray->raydir.x);
	ray->step.x = steps.x;
	ray->sidedist.x = steps.y;
	steps = init_step(app->camera.pos.y, ray->map_y, ray->deltadist.y,
			ray->raydir.y);
	ray->step.y = steps.x;
	ray->sidedist.y = steps.y;
	dda_loop(app, ray);
	if (ray->side == 0)
		ray->perpwalldist = ray->sidedist.x - ray->deltadist.x;
	else
		ray->perpwalldist = ray->sidedist.y - ray->deltadist.y;
}

void	compute_draw_boundaries(t_draw *draw, t_ray *ray)
{
	draw->lh = (int)(HEIGHT / ray->perpwalldist);
	draw->ds = -draw->lh / 2 + HEIGHT / 2;
	if (draw->ds < 0)
		draw->ds = 0;
	draw->de = draw->lh / 2 + HEIGHT / 2;
	if (draw->de >= HEIGHT)
		draw->de = HEIGHT - 1;
}
