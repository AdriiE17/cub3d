/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:36:39 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 18:10:45 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

static void	draw_player_indicator(t_app *app)
{
	int	i;

	mlx_put_pixel(app->image, MINI_X, MINI_Y, 0x00FF00FF);
	i = -1;
	while (++i < 10)
		mlx_put_pixel(app->image, MINI_X, MINI_Y + i, 0xFFFFFFFF);
	i = -1;
	while (++i < 5)
	{
		mlx_put_pixel(app->image, MINI_X - i, MINI_Y - 3 + i, 0xFFFFFFFF);
		mlx_put_pixel(app->image, MINI_X + i, MINI_Y - 3 + i, 0xFFFFFFFF);
	}
}

static void	draw_enemy(t_vec2 pos, t_app *app)
{
	int		dx;
	int		dy;
	t_vec2	pixel;

	dx = -3;
	while (++dx < 4)
	{
		dy = -3;
		while (++dy < 4)
		{
			pixel.x = MINI_X + (pos.x * MINI_SCALE) + dx;
			pixel.y = MINI_Y + (pos.y * MINI_SCALE) + dy;
			mlx_put_pixel(app->image, pixel.x, pixel.y, 0xFF0000FF);
		}
	}
}

static t_vec2	calc_enemy_pos(t_enemy *e, t_app *app, double angle)
{
	t_vec2	rel_pos;
	t_vec2	rotated;

	rel_pos.x = e->pos_x - app->camera.pos.x;
	rel_pos.y = e->pos_y - app->camera.pos.y;
	rotated.x = rel_pos.x * cos(angle) - rel_pos.y * sin(angle);
	rotated.y = rel_pos.x * sin(angle) + rel_pos.y * cos(angle);
	return (rotated);
}

void	draw_enemies(t_app *app)
{
	int		i;
	double	angle;
	t_vec2	pos;

	angle = -atan2(app->camera.dir.y, app->camera.dir.x) - M_PI / 2.0;
	i = -1;
	while (++i < app->game.enemy_count)
	{
		if (!app->game.enemies[i].is_active)
			continue ;
		pos = calc_enemy_pos(&app->game.enemies[i], app, angle);
		draw_enemy(pos, app);
	}
}

void	render_minimap(t_app *app)
{
	draw_minimap_background(app);
	draw_map_features(app);
	draw_enemies(app);
	draw_player_indicator(app);
}
