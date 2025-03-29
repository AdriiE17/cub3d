/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:23:15 by victor            #+#    #+#             */
/*   Updated: 2025/03/29 13:53:38 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static double	get_wall_x(t_app *app, t_ray *ray)
{
	double	wx;

	if (ray->side == 0)
		wx = app->camera.pos.y + ray->perpwalldist * ray->raydir.y;
	else
		wx = app->camera.pos.x + ray->perpwalldist * ray->raydir.x;
	return (wx - floor(wx));
}

static void	select_texture(t_app *app, t_ray *ray, t_draw *draw, double wx)
{
	if (ray->side == 0)
	{
		if (ray->raydir.x < 0)
			draw->tex = app->game.tex_we;
		else
			draw->tex = app->game.tex_ea;
	}
	else
	{
		if (ray->raydir.y < 0)
			draw->tex = app->game.tex_no;
		else
			draw->tex = app->game.tex_so;
	}
	if (!draw->tex)
		return ;
	draw->tx = (int)(wx * draw->tex->width);
	if (ray->side == 0 && ray->raydir.x > 0)
		draw->tx = draw->tex->width - draw->tx - 1;
	else if (ray->side == 1 && ray->raydir.y < 0)
		draw->tx = draw->tex->width - draw->tx - 1;
}

static void	compute_texture_params(t_app *app, t_ray *ray, t_draw *draw)
{
	double	wx;

	wx = get_wall_x(app, ray);
	select_texture(app, ray, draw, wx);
}

static void	render_column(t_app *app, int x, t_ray *ray)
{
	t_draw	draw;

	compute_draw_boundaries(&draw, ray);
	compute_texture_params(app, ray, &draw);
	draw_pixels(app, x, &draw);
}

void	render_scene(void *param)
{
	t_app	*app;
	int		x;
	t_ray	ray;

	app = (t_app *)param;
	ft_draw_background(app);
	x = 0;
	while (x < WIDTH)
	{
		init_ray(app, x, &ray);
		do_dda(app, &ray);
		render_column(app, x, &ray);
		x++;
	}
}
