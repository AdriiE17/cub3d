/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:23:15 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 21:07:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*   Computes the necessary parameters for drawing the texture.              */
/*                                                                            */
/*   - First, it calculates the wall's intersection X-coordinate using the  */
/*     `get_wall_x()` function.                                              */
/*   - Then, it selects the correct texture based on the ray's side and      */
/*     direction using `select_texture()`.                                    */
/*                                                                            */
/* ************************************************************************** */
static void	compute_draw_boundaries(t_draw *draw, t_ray *ray, t_app *app)
{
	draw->lh = (int)(HEIGHT / ray->perpwalldist);
	draw->ds = -draw->lh / 2 + HEIGHT / 2 - app->camera.view_z;
	if (draw->ds < 0)
		draw->ds = 0;
	draw->de = draw->lh / 2 + HEIGHT / 2 - app->camera.view_z;
	if (draw->de >= HEIGHT)
		draw->de = HEIGHT - 1;
}

/* ************************************************************************** */
/*                                                                            */
/*   Renders a column of pixels for the given ray at the specified X.        */
/*                                                                            */
/*   - First, it computes the boundaries of the vertical line (column) to    */
/*     be drawn using `compute_draw_boundaries()`.                            */
/*   - It then calculates the texture parameters and applies them to the     */
/*     drawing process using `compute_texture_params()`.                     */
/*   - Finally, it draws the pixels for the column by calling `draw_pixels()`.*/
/*                                                                            */
/* ************************************************************************** */
static void	render_column(t_app *app, int x, t_ray *ray)
{
	t_draw	draw;

	compute_draw_boundaries(&draw, ray, app);
	compute_texture_params(app, ray, &draw);
	if (!draw.tex)
		return ;
	app->z_buffer[x] = ray->perpwalldist;
	draw_pixels(app, x, &draw);
}

/* ************************************************************************** */
/*                                                                            */
/*   Renders the entire scene.                                               */
/*                                                                            */
/*   - Loops over every column (X-coordinate) in the screen.                */
/*   - For each column, it initializes the ray, performs the DDA (raycasting),*/
/*     and renders the corresponding column.                                  */
/*   - The function calls `render_column()` to draw each vertical line.      */
/*                                                                            */
/* ************************************************************************** */
static void	sort_enemies_by_distance(t_enemy **enemies, int count, t_app *a)
{
	int		i;
	int		j;
	t_enemy	*temp;

	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (hypot(enemies[j]->pos_x - a->camera.pos.x,
					enemies[j]->pos_y - a->camera.pos.y)
				< hypot(enemies[j + 1]->pos_x - a->camera.pos.x,
					enemies[j + 1]->pos_y - a->camera.pos.y))
			{
				temp = enemies[j];
				enemies[j] = enemies[j + 1];
				enemies[j + 1] = temp;
			}
		}
	}
}

static void	sort_and_render(t_app *a)
{
	t_enemy	**sorted;
	int		i;

	sorted = malloc(a->game.enemy_count * sizeof(t_enemy *));
	if (!sorted)
		return ;
	i = -1;
	while (++i < a->game.enemy_count)
		sorted[i] = &a->game.enemies[i];
	sort_enemies_by_distance(sorted, a->game.enemy_count, a);
	i = -1;
	while (++i < a->game.enemy_count)
		if (sorted[i]->is_active)
			render_enemy(a, sorted[i]);
	free(sorted);
}

void	render_scene(void *param)
{
	t_app	*a;
	int		x;
	t_ray	r;

	a = param;
	ft_draw_background(a);
	x = -1;
	while (++x < WIDTH)
	{
		init_ray(a, x, &r);
		do_dda(a, &r);
		render_column(a, x, &r);
	}
	render_minimap(a);
	sort_and_render(a);
}
