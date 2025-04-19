/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:27:08 by victor            #+#    #+#             */
/*   Updated: 2025/04/18 19:14:41 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	safe_free(t_app *app)
{
	if (app->game.tex_no)
		mlx_delete_texture(app->game.tex_no);
	if (app->game.tex_so)
		mlx_delete_texture(app->game.tex_so);
	if (app->game.tex_we)
		mlx_delete_texture(app->game.tex_we);
	if (app->game.tex_ea)
		mlx_delete_texture(app->game.tex_ea);
	if (app->game.tex_door)
		mlx_delete_texture(app->game.tex_door);
	if (app->game.tex_door_w)
		mlx_delete_texture(app->game.tex_door_w);
	if (app->game.tex_enemy)
		mlx_delete_texture(app->game.tex_enemy);
	free(app->game.texture_no);
	free(app->game.texture_so);
	free(app->game.texture_we);
	free(app->game.texture_ea);
	free(app->game.texture_door);
	free(app->game.texture_door_w);
	free(app->game.texture_enemy);
	if (app->game.doors)
		free(app->game.doors);
	if (app->game.enemies)
		free(app->game.enemies);
}

static int	load_main_textures(t_app *app)
{
	t_game	*g;

	g = &app->game;
	g->tex_no = mlx_load_png(g->texture_no);
	g->tex_so = mlx_load_png(g->texture_so);
	g->tex_we = mlx_load_png(g->texture_we);
	g->tex_ea = mlx_load_png(g->texture_ea);
	if (!g->tex_no || !g->tex_so || !g->tex_we || !g->tex_ea)
	{
		printf("Error: Failed to load one or more textures.\n");
		cleanup(app);
		return (-1);
	}
	return (0);
}

static int	load_extra_textures(t_app *app)
{
	t_game	*g;

	g = &app->game;
	if (g->texture_enemy)
	{
		g->tex_enemy = mlx_load_png(g->texture_enemy);
		if (!g->tex_enemy)
		{
			printf("Error: Failed to load enemy textures.\n");
			cleanup(app);
			return (-1);
		}
	}
	if (g->texture_door)
		g->tex_door = mlx_load_png(g->texture_door);
	if (g->texture_door_w)
		g->tex_door_w = mlx_load_png(g->texture_door_w);
	if ((g->tex_door && !g->tex_door_w) || (!g->tex_door && g->tex_door_w))
	{
		printf("Error\nBoth door textures (D/W) must be provided\n");
		cleanup(app);
		return (-1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*   Loads texture PNG files using the texture paths stored in the game       */
/*   structure. If any texture path is missing or any texture fails to load,  */
/*   prints an error message, terminates MLX, and returns -1. Otherwise,      */
/*	 loads  all textures and returns 0.                                       */
/*                                                                            */
/* ************************************************************************** */
int	load_game_textures(t_app *app)
{
	t_game	*g;

	g = &app->game;
	if (!g->texture_no || !g->texture_so || !g->texture_we || !g->texture_ea)
	{
		printf("Error: Missing texture paths in map file.\n");
		mlx_terminate(app->mlx);
		return (-1);
	}
	if (load_main_textures(app) == -1 || load_extra_textures(app) == -1)
		return (-1);
	return (0);
}

void	init_enemy(t_game *g, int i, int j)
{
	g->enemy_count++;
	g->enemies = realloc(g->enemies, sizeof(t_enemy) * g->enemy_count);
	g->enemies[g->enemy_count - 1] = (t_enemy){
		.pos_x = j + 0.5,
		.pos_y = i + 0.5,
		.speed = 0.8,
		.is_active = 1,
		.anim_frame = 0.0,
		.time_since_last_move = 0.0,
		.radius = 0.7
	};
	g->map[i][j] = '0';
}
