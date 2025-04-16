/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aescande <aescande@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:15:37 by aescande          #+#    #+#             */
/*   Updated: 2025/04/16 20:14:01 by aescande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

t_quaternion	quaternion_product(t_quaternion	p, t_quaternion q)
{
	t_quaternion	r;

	r.s = p.s*q.s - p.i*q.i - p.j*q.j - p.k*q.k;
	r.i = p.s*q.i + p.i*q.s + p.j*q.k - p.k*q.j;
	r.j = p.s*q.j - p.i*q.k + p.j*q.s + p.k*q.i;
	r.k = p.s*q.k + p.i*q.j - p.j*q.i + p.k*q.s;
	return (r);
}

t_quaternion	conjugate(t_quaternion p)
{
	return ((t_quaternion){p.s, -p.i, -p.j, -p.k});
}

t_quaternion	plane_rotation(t_quaternion p, t_quaternion axis, double alpha)
{
	double			arg;

	alpha /= 2;
	arg = sqrt(axis.i*axis.i + axis.j*axis.j + axis.k*axis.k);
	axis.s = cos(alpha);
	axis.i = sin(alpha)*(axis.i / arg);
	axis.j = sin(alpha)*(axis.j / arg);
	axis.k = sin(alpha)*(axis.k / arg);

	return (quaternion_product(axis, quaternion_product(p, conjugate(axis))));
}
