/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:20:35 by victor            #+#    #+#             */
/*   Updated: 2025/03/29 13:22:41 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	ft_draw_background(t_app *app)
{
	int32_t		floor_color;
	int32_t		ceiling_color;
	uint32_t	x;
	uint32_t	y;

	floor_color = ft_pixel(app->game.floor_color[0],
			app->game.floor_color[1], app->game.floor_color[2], 255);
	ceiling_color = ft_pixel(app->game.ceiling_color[0],
			app->game.ceiling_color[1], app->game.ceiling_color[2], 255);
	x = -1;
	while (++x < WIDTH)
	{
		y = -1;
		while (++y < HEIGHT / 2)
			mlx_put_pixel(app->image, x, y, ceiling_color);
		y = HEIGHT / 2;
		while (++y < HEIGHT)
			mlx_put_pixel(app->image, x, y, floor_color);
	}
}
