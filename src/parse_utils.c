/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 22:51:37 by victor            #+#    #+#             */
/*   Updated: 2025/03/29 13:24:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	process_line(char **line, char ***temp, int *count, int is_newline)
{
	size_t	len;

	if (is_newline)
	{
		len = ft_strlen(*line);
		if (len > 0)
			(*line)[len - 1] = '\0';
	}
	*temp = ft_realloc(*temp, ((*count) + 1) * sizeof(char *));
	if (!*temp)
		return (-1);
	(*temp)[*count] = ft_strdup(*line);
	if (!(*temp)[*count])
		return (free(*line), -1);
	(*count)++;
	if (is_newline)
	{
		free(*line);
		*line = ft_strdup("");
		if (!*line)
			return (-1);
	}
	return (0);
}

static int	read_loop(int fd, char ***temp, int *count, char **line)
{
	int		r;
	char	buf[2];
	char	*new_line;

	r = read(fd, buf, 1);
	while (r > 0)
	{
		buf[1] = '\0';
		new_line = ft_strjoin(*line, buf);
		if (!new_line)
			return (-1);
		free(*line);
		*line = new_line;
		if (buf[0] == '\n')
		{
			if (process_line(line, temp, count, 1) == -1)
				return (-1);
		}
		r = read(fd, buf, 1);
	}
	return (0);
}

int	read_lines(const char *filename, char ***lines, int *line_count)
{
	int		fd;
	int		count;
	char	**temp;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	count = 0;
	temp = NULL;
	line = ft_strdup("");
	if (!line)
		return (close(fd), -1);
	if (read_loop(fd, &temp, &count, &line) == -1)
		return (close(fd), -1);
	if (ft_strlen(line) > 0)
		if (process_line(&line, &temp, &count, 0) == -1)
			return (close(fd), -1);
	free(line);
	close(fd);
	*lines = temp;
	*line_count = count;
	return (0);
}

void	draw_pixels(t_app *app, int x, t_draw *draw)
{
	int	y;
	int	d;
	int	ty;

	y = draw->ds;
	while (y < draw->de)
	{
		d = y * 256 - HEIGHT * 128 + draw->lh * 128;
		ty = ((d * draw->tex->height) / draw->lh) / 256;
		mlx_put_pixel(app->image, x, y,
			((uint32_t *)draw->tex->pixels)[ty * draw->tex->width + draw->tx]);
		y++;
	}
}
