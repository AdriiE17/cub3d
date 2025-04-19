/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:20:35 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 21:29:25 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

static int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	ft_draw_background(t_app *app)
{
	int32_t		floor_color;
	int32_t		ceiling_color;
	int32_t		horizon;
	int32_t		x;
	int32_t		y;

	floor_color = ft_pixel(app->game.floor_color[0],
			app->game.floor_color[1], app->game.floor_color[2], 255);
	ceiling_color = ft_pixel(app->game.ceiling_color[0],
			app->game.ceiling_color[1], app->game.ceiling_color[2], 255);
	horizon = HEIGHT / 2 - app->camera.view_z;
	if (horizon < 0)
		horizon = 0;
	if (horizon > HEIGHT)
		horizon = HEIGHT;
	x = -1;
	while (++x < WIDTH)
	{
		y = -1;
		while (++y < horizon)
			mlx_put_pixel(app->image, x, y, ceiling_color);
		y = horizon - 1;
		while (++y < HEIGHT)
			mlx_put_pixel(app->image, x, y, floor_color);
	}
}

/* ************************************************************************** */
/* Converts a pixel color value to a format suitable for rendering in the     */
/* graphical context. Returns the converted pixel color as a uint32_t value. */
/* ************************************************************************** */
uint32_t	convert_pixel(uint32_t px)
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	uint8_t	alpha;

	red = (px >> 16) & 0xFF;
	green = (px >> 8) & 0xFF;
	blue = px & 0xFF;
	alpha = (px >> 24) & 0xFF;
	return (ft_pixel(blue, green, red, alpha));
}

/* ************************************************************************** */
/*                                                                            */
/*   Draws a vertical strip of pixels for a wall texture on the screen.       */
/*                                                                            */
/*   - Iterates from `draw->ds` (draw start) to `draw->de` (draw end).        */
/*   - Calculates the texture y-coordinate (`ty`) for each screen pixel.      */
/*   - Retrieves the corresponding texture pixel and plots it with            */
/*     `mlx_put_pixel()`.                                                     */
/*                                                                            */
/* ************************************************************************** */
void	draw_pixels(t_app *app, int x, t_draw *draw)
{
	int			y;
	int			d;
	int			ty;
	uint32_t	px;

	y = draw->ds;
	while (y < draw->de)
	{
		d = (y + app->camera.view_z) * 256 - HEIGHT * 128 + draw->lh * 128;
		ty = ((d * draw->tex->height) / draw->lh) / 256;
		ty = ty % draw->tex->height;
		if (ty < 0)
			ty += draw->tex->height;
		px = ((uint32_t *)draw->tex->pixels)[ty * draw->tex->width + draw->tx];
		px = convert_pixel(px);
		mlx_put_pixel(app->image, x, y, px);
		y++;
	}
}
