/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_txt_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:15:16 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 19:19:33 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

static t_door	*get_door(t_game *g, int x, int y)
{
	int	i;

	i = 0;
	while (i < g->door_count)
	{
		if (g->doors[i].x == x && g->doors[i].y == y)
			return (&g->doors[i]);
		i++;
	}
	return (NULL);
}

static mlx_texture_t	*get_wall_texture(t_app *app, t_ray *ray)
{
	char	adj;

	if (ray->side == 0)
		adj = safe_get_tile(&app->game, ray->map_x - ray->step.x, ray->map_y);
	else
		adj = safe_get_tile(&app->game, ray->map_x, ray->map_y - ray->step.y);
	if ((ray->side == 0 && adj == '2') || (ray->side == 1 && adj == '3'))
		return (app->game.tex_door_w);
	if (ray->side == 0)
	{
		if (ray->raydir.x < 0)
			return (app->game.tex_we);
		else
			return (app->game.tex_ea);
	}
	else
	{
		if (ray->raydir.y < 0)
			return (app->game.tex_no);
		else
			return (app->game.tex_so);
	}
}

static mlx_texture_t	*get_door_texture(t_game *game, t_door *d,
		t_ray *ray, double *w)
{
	bool	panel_face;

	panel_face = (d->orientation == '2' && ray->side == 1)
		|| (d->orientation == '3' && ray->side == 0);
	if (panel_face)
	{
		if (d->orientation == '2')
			*w = fmod(*w - d->open_offset + 1.0, 1.0);
		else
			*w = fmod(*w + d->open_offset + 1.0, 1.0);
		return (game->tex_door);
	}
	return (game->tex_door_w);
}

static int	compute_tx(mlx_texture_t *t, double w, t_ray *ray)
{
	int	tx;

	tx = (int)(w * t->width);
	if ((ray->side == 0 && ray->raydir.x > 0)
		|| (ray->side == 1 && ray->raydir.y < 0))
		tx = t->width - tx - 1;
	return ((tx + t->width) % t->width);
}

void	compute_texture_params(t_app *app, t_ray *ray, t_draw *draw)
{
	double			w;
	mlx_texture_t	*t;
	t_door			*d;

	if (ray->side == 0)
		w = app->camera.pos.y + ray->perpwalldist * ray->raydir.y;
	else
		w = app->camera.pos.x + ray->perpwalldist * ray->raydir.x;
	w -= floor(w);
	if (ray->hit_tile == '2' || ray->hit_tile == '3')
		d = get_door(&app->game, ray->map_x, ray->map_y);
	else
		d = NULL;
	if (d)
		t = get_door_texture(&app->game, d, ray, &w);
	else if (ray->hit_tile == '1')
		t = get_wall_texture(app, ray);
	else
		t = NULL;
	draw->tex = t;
	if (!t)
		return ;
	draw->tx = compute_tx(t, w, ray);
}
