/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collition.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:17:00 by victor            #+#    #+#             */
/*   Updated: 2025/04/03 12:06:41 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* ************************************************************************** */
/*                                                                            */
/*   Safely retrieves a tile from the game map at the given (x, y) position.  */
/*                                                                            */
/*   - If the position is out of bounds (negative or exceeding map limits),   */
/*     it returns '1' (wall) to prevent access violations.                    */
/*   - Uses `ft_strlen` to check the row length dynamically, ensuring         */
/*     protection against accessing undefined memory.                         */
/*                                                                            */
/* ************************************************************************** */
char	safe_get_tile(t_game *game, int x, int y)
{
	int	len;

	if (y < 0 || y >= game->map_height)
		return ('1');
	len = ft_strlen(game->map[y]);
	if (x < 0 || x >= len)
		return ('1');
	return (game->map[y][x]);
}

/* ************************************************************************** */
/*                                                                            */
/*   Initializes a collision structure (`t_collision`) for a given position.  */
/*                                                                            */
/*   - The collision system considers the player as a circular shape with a   */
/*     radius `COLLISION_RADIUS`.                                             */
/*   - This function calculates the bounding box around the circle to check   */
/*     for potential collisions with walls.                                   */
/*   - The bounding box is defined by:                                        */
/*       - `min_i` and `max_i`: vertical range of tiles to check.             */
/*       - `min_j` and `max_j`: horizontal range of tiles to check.           */
/*                                                                            */
/* ************************************************************************** */
static void	init_collision(t_collision *c, double new_x, double new_y)
{
	double	r;

	r = COLLISION_RADIUS;
	c->r = r;
	c->min_i = (int)floor(new_y - r);
	c->max_i = (int)ceil(new_y + r);
	c->min_j = (int)floor(new_x - r);
	c->max_j = (int)ceil(new_x + r);
}

/* ************************************************************************** */
/*                                                                            */
/*   Checks if a circular player entity collides with a wall tile.            */
/*                                                                            */
/*   - The function determines the closest point on a wall cell to the        */
/*     player’s position.                                                     */
/*   - If the squared distance between this closest point and the player's    */
/*     center is less than the squared collision radius, a collision occurs.  */
/*                                                                            */
/*   - The logic follows the "Closest Point on AABB" technique:               */
/*     - If the player's x is inside the tile, `closest_x = new_x`.           */
/*     - If the player's x is left of the tile, `closest_x = tile_left_edge`. */
/*     - If the player's x is right of the tile, `closest_x = tile_right_edge`*/
/*     - The same logic applies for `y`.                                      */
/*                                                                            */
/*   - Finally, it calculates the squared distance `(dx * dx + dy * dy)`.     */
/*     If it's less than the squared radius, there is a collision.            */
/*                                                                            */
/* ************************************************************************** */
static int	check_cell_collision(double new_x, double new_y,
		t_collision *c)
{
	double	closest_x;
	double	closest_y;
	double	dx;
	double	dy;

	closest_x = new_x;
	if (new_x < c->j)
		closest_x = c->j;
	else if (new_x > c->j + 1)
		closest_x = c->j + 1;
	closest_y = new_y;
	if (new_y < c->i)
		closest_y = c->i;
	else if (new_y > c->i + 1)
		closest_y = c->i + 1;
	dx = new_x - closest_x;
	dy = new_y - closest_y;
	if ((dx * dx + dy * dy) < (c->r * c->r))
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*   Determines whether a given position (new_x, new_y) collides with walls.  */
/*                                                                            */
/*   - Initializes a `t_collision` structure to define the player's           */
/*     bounding area.                                                         */
/*   - Iterates over the surrounding tiles that could be in contact.          */
/*   - Calls `safe_get_tile()` to check if a tile is a wall ('1').            */
/*   - If the tile is a wall, `check_cell_collision()` is used to verify      */
/*     if the player's circular shape actually overlaps with the wall tile.   */
/*   - If any tile confirms a collision, it returns `1` (true). Otherwise,    */
/*     it returns `0` (false).                                                */
/*                                                                            */
/* ************************************************************************** */
int	collides(t_game *game, double new_x, double new_y)
{
	t_collision	c;

	init_collision(&c, new_x, new_y);
	c.i = c.min_i - 1;
	while (++c.i <= c.max_i)
	{
		c.j = c.min_j - 1;
		while (++c.j <= c.max_j)
		{
			if (safe_get_tile(game, c.j, c.i) == '1')
			{
				if (check_cell_collision(new_x, new_y, &c) == 1)
					return (1);
			}
		}
	}
	return (0);
}
