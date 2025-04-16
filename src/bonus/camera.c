/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:27:02 by victor            #+#    #+#             */
/*   Updated: 2025/04/16 21:29:51 by aescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

/* ************************************************************************** */
/*                                                                            */
/*   Checks if the Escape key is pressed. If so, it closes the MLX window,    */
/*   effectively terminating the program.                                     */
/*                                                                            */
/* ************************************************************************** */
static void	check_escape(t_app *app)
{
	if (mlx_is_key_down(app->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(app->mlx);
}

/* ************************************************************************** */
/*                                                                            */
/*   Updates the camera's position based on user input.                       */
/*                                                                            */
/*   - Moves forward (W) or backward (S) along the camera's direction vector. */
/*   - Moves left (A) or right (D) perpendicular to the direction vector.     */
/*   - Prevents movement if a collision is detected.                          */
/*                                                                            */
/*   The movement logic works as follows:                                     */
/*   - The direction vector (dir.i, dir.j) represents where the camera is     */
/*     facing.                                                                */
/*   - Moving forward (W) means adding this vector scaled by speed.           */
/*   - Moving backward (S) means subtracting this vector scaled by speed.     */
/*   - Moving sideways (A/D) requires a perpendicular movement:               */
/*     - Left (A) is along the negative perpendicular vector (-dir.j, dir.i)  */
/*     - Right (D) is along the positive perpendicular vector (dir.j, -dir.i) */
/*                                                                            */
/* ************************************************************************** */
static void	update_camera_movement(t_app *app, double delta_time)
{
	double	new_x;
	double	new_y;
	double	speed;

	new_x = app->camera.pos.i;
	new_y = app->camera.pos.j;
	speed = app->camera.move_speed * delta_time;
	if (mlx_is_key_down(app->mlx, MLX_KEY_LEFT_SHIFT))
		speed *= 2;
	new_x += (mlx_is_key_down(app->mlx, MLX_KEY_W)
			- mlx_is_key_down(app->mlx, MLX_KEY_S))
		* app->camera.dir.i * speed;
	new_y += (mlx_is_key_down(app->mlx, MLX_KEY_W)
			- mlx_is_key_down(app->mlx, MLX_KEY_S))
		* app->camera.dir.j * speed;
	new_x += (mlx_is_key_down(app->mlx, MLX_KEY_A)
			- mlx_is_key_down(app->mlx, MLX_KEY_D))
		* app->camera.dir.j * speed;
	new_y += (mlx_is_key_down(app->mlx, MLX_KEY_D)
			- mlx_is_key_down(app->mlx, MLX_KEY_A))
		* app->camera.dir.i * speed;
	if (!collides(&app->game, new_x, new_y))
	{
		app->camera.pos.i = new_x;
		app->camera.pos.j = new_y;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Rotates the camera left or right by a given angle.                       */
/*                                                                            */
/*   - Uses a standard 2D rotation matrix:                                    */
/*       [ cos(theta)  -sin(theta) ]                                          */
/*       [ sin(theta)   cos(theta) ]                                          */
/*   - Applies this rotation to both the direction vector and the camera      */
/*     plane to maintain correct rendering of perspective.                    */
/*                                                                            */
/*   The rotation logic works as follows:                                     */
/*   - The `dir` vector represents where the camera is facing.                */
/*   - The `plane` vector determines how much of the world is visible         */
/*     (used for raycasting projection).                                      */
/*   - Applying the rotation matrix rotates both vectors around the origin.   */
/*                                                                            */
/* ************************************************************************** */
/*static void	rotate_camera(t_app *app, double rotation)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = app->camera.dir.i;
	old_plane_x = app->camera.plane.i;
	
}
*/
/* ************************************************************************** */
/*                                                                            */
/*   Detects if the left or right arrow keys are pressed and rotates the      */
/*   camera accordingly.                                                      */
/*                                                                            */
/*   - Left Arrow (←) rotates counterclockwise (negative rotation).           */
/*   - Right Arrow (→) rotates clockwise (positive rotation).                 */
/*                                                                            */
/* ************************************************************************** */
static void	update_camera_rotation(t_app *app, double delta_time)
{
	double	alpha;
	double	view_speed;
	int32_t	xpos;
	int32_t	zpos;

	mlx_get_mouse_pos(app->mlx, &xpos, &zpos);
	alpha = (xpos - WIDTH/2) * delta_time / 20;
	app->camera.dir = plane_rotation(app->camera.dir, (t_quaternion){0, 0, 0, xpos}, alpha);
	app->camera.plane = plane_rotation(app->camera.plane, (t_quaternion){0, 0, 0, xpos}, alpha);
	view_speed = (zpos - HEIGHT/2) * delta_time * 100;
	if (!(fabs(app->camera.view_z) >= HEIGHT && app->camera.view_z * view_speed > 0))
		app->camera.view_z += view_speed;
	mlx_set_mouse_pos(app->mlx, WIDTH/2, HEIGHT/2);
}

/* ************************************************************************** */
/*                                                                            */
/*   Handles camera movement and rotation each frame.                         */
/*                                                                            */
/* ************************************************************************** */

void	move_camera(void *param)
{
	static double	last_time = 0;
	double			current_time;
	double			delta_time;
	t_app	*app;	

	app = (t_app *)param;
	current_time = mlx_get_time();
	delta_time = current_time - last_time;
	last_time = current_time;	
	check_escape(app);
	update_camera_movement(app, delta_time);
	update_camera_rotation(app, delta_time);
}
