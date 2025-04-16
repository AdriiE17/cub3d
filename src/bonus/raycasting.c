/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:50:51 by victor            #+#    #+#             */
/*   Updated: 2025/04/16 21:24:48 by aescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*   Initializes a ray for the given screen column (x).                       */
/*                                                                            */
/*   - Computes `camx`, which represents the camera's x-coordinate in        */
/*     normalized device coordinates (ranging from -1 to 1).                  */
/*     Formula: camx = 2 * x / WIDTH - 1                                      */
/*                                                                            */
/*   - Calculates the ray direction `raydir` using the camera direction       */
/*     (`dir`) and plane (`plane`):                                           */
/*       raydir.i = dir.i + plane.i * camx                                    */
/*       raydir.j = dir.j + plane.j * camx                                    */
/*                                                                            */
/*   - Determines `map_x` and `map_y`, which are the integer coordinates      */
/*     of the current grid cell (truncated from `pos`).                       */
/*                                                                            */
/*   - Computes `deltadist`, the distance the ray travels between grid        */
/*     lines in each axis. It is calculated as:                               */
/*       deltadist.i = |raydir_modulus / raydir.i|                            */
/*       deltadist.j = |raydir_modulus / raydir.j|                            */
/*                                                                            */
/* ************************************************************************** */

void	init_ray(t_app *app, int x, t_ray *ray)
{
	double	camx;

	camx = 2 * x / (double)WIDTH - 1;
	ray->raydir.i = app->camera.dir.i + app->camera.plane.i * camx;
	ray->raydir.j = app->camera.dir.j + app->camera.plane.j * camx;
	ray->map_x = (int)app->camera.pos.i;
	ray->map_y = (int)app->camera.pos.j;
	ray->raydir_mod = sqrt(pow(ray->raydir.i, 2) + pow(ray->raydir.j, 2));
	ray->deltadist.i = fabs(ray->raydir_mod / ray->raydir.i);
	ray->deltadist.j = fabs(ray->raydir_mod / ray->raydir.j);
}

/* ************************************************************************** */
/*                                                                            */
/*   Determines the step direction and initial side distance.                 */
/*                                                                            */
/*   - The step `ret.i` determines whether the ray moves left (-1) or right   */
/*     (+1) in the x-direction, and similarly for `ret.j` in the y-direction. */
/*                                                                            */
/*   - The initial `sidedist` is calculated using:                            */
/*       sidedist_x = (map_x + 1 - pos_x) * deltadist_x  (if moving right)    */
/*       sidedist_x = (pos_x - map_x) * deltadist_x  (if moving left)         */
/*     Similarly for `sidedist_y`.                                            */
/*                                                                            */
/* ************************************************************************** */
static t_quaternion	init_step(double pos, int map, double deltadist, double raydir)
{
	t_quaternion	ret;

	if (raydir < 0)
	{
		ret.i = -1;
		ret.j = (pos - map) * deltadist;
	}
	else
	{
		ret.i = 1;
		ret.j = ((map + 1.0) - pos) * deltadist;
	}
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*   Implements the Digital Differential Analyzer (DDA) algorithm.            */
/*                                                                            */
/*   - This algorithm finds the first intersection of the ray with a wall.    */
/*   - The ray moves in discrete steps, choosing either the x or y direction  */
/*     depending on which `sidedist` is smaller.                              */
/*                                                                            */
/*   - If `sidedist.i < sidedist.j`, the ray moves in the x-direction:        */
/*       sidedist.i += deltadist.i                                            */
/*       map_x += step_x                                                      */
/*                                                                            */
/*   - Otherwise, it moves in the y-direction:                                */
/*       sidedist.j += deltadist.j                                            */
/*       map_y += step_y                                                      */
/*                                                                            */
/*   - This process repeats until a wall (`'1'`) is found or out of bounds.   */
/*                                                                            */
/* ************************************************************************** */
static void	dda_loop(t_app *app, t_ray *ray)
{
	while (1)
	{
		if (ray->sidedist.i < ray->sidedist.j)
		{
			ray->sidedist.i += ray->deltadist.i;
			ray->map_x += ray->step.i;
			ray->side = 0;
		}
		else
		{
			ray->sidedist.j += ray->deltadist.j;
			ray->map_y += ray->step.j;
			ray->side = 1;
		}
		if (safe_get_tile(&app->game, ray->map_x, ray->map_y) == '1')
			break ;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Executes the full DDA (raycasting) routine for a single ray.             */
/*                                                                            */
/*   - Calls `init_step()` to determine stepping direction and initial        */
/*     side distances.                                                        */
/*                                                                            */
/*   - Runs `dda_loop()` to trace the ray through the grid until it hits      */
/*     a wall.                                                                */
/*                                                                            */
/*   - Computes the perpendicular distance to the wall (`perpwalldist`),      */
/*     which is essential for perspective projection.                         */
/*                                                                            */
/*     Formula:                                                               */
/*       perpwalldist = (sidedist_x - deltadist_x)/raydir_mod  (if hit		  */
/*       was on x-side)														  */
/*       perpwalldist = (sidedist_y - deltadist_y)/raydir_mod  (if hit		  */
/*       was on y-side)														  */
/*                                                                            */
/* ************************************************************************** */
void	do_dda(t_app *app, t_ray *ray)
{
	t_quaternion	steps;

	steps = init_step(app->camera.pos.i, ray->map_x, ray->deltadist.i,
			ray->raydir.i);
	ray->step.i = steps.i;
	ray->sidedist.i = steps.j;
	steps = init_step(app->camera.pos.j, ray->map_y, ray->deltadist.j,
			ray->raydir.j);
	ray->step.j = steps.i;
	ray->sidedist.j = steps.j;
	dda_loop(app, ray);
	if (ray->side == 0)
		ray->perpwalldist = (ray->sidedist.i - ray->deltadist.i) / ray->raydir_mod;
	else
		ray->perpwalldist = (ray->sidedist.j - ray->deltadist.j) / ray->raydir_mod;
	//printf("sidedist.j: %lf, sidedist.i: %lf\n", ray->sidedist.j/ray->deltadist.j, ray->sidedist.i/ray->deltadist.i);
	//printf("raydir.j: %lf, raydir.i: %lf\n", ray->raydir.j, ray->raydir.i);
	//printf("product: %lf, normal: %lf\n", ray->perpwalldist*ray->raydir.j, ray->perpwalldist);
}

/* ************************************************************************** */
/*                                                                            */
/*   Computes the vertical drawing boundaries for the wall slice.             */
/*                                                                            */
/*   - Uses `perpwalldist` to determine the height of the projected wall:     */
/*       line_height = HEIGHT / (perpwalldist / raydir_modulus)               */

/*                                                                            */
/*   - Calculates the top (`draw_start`) and bottom (`draw_end`) positions    */
/*     on the screen, centering the wall slice:                               */
/*       draw_start = -line_height / 2 + HEIGHT / 2                           */
/*       draw_end = line_height / 2 + HEIGHT / 2                              */
/*                                                                            */
/*   - Ensures the values stay within screen limits.                          */
/*                                                                            */
/* ************************************************************************** */

