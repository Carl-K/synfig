/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/primitive/transformation.cpp
**	\brief Transformation
**
**	$Id$
**
**	\legal
**	......... ... 2015-2018 Ivan Mahonin
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "transformation.h"

#endif

using namespace synfig;
using namespace rendering;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

bool
Transformation::can_merge_outer_vfunc(const Transformation&) const
	{ return false; }

bool
Transformation::can_merge_inner_vfunc(const Transformation&) const
	{ return false; }

void
Transformation::merge_outer_vfunc(const Transformation&)
	{ }

void
Transformation::merge_inner_vfunc(const Transformation&)
	{ }

Mesh::Handle
Transformation::build_mesh_vfunc(const Rect &target_rect, const Vector &precision) const
{
	typedef std::pair<int, Mesh::Vertex> GridPoint;
	Transformation::Handle back_transform = create_inverted();
	if (!back_transform)
		return Mesh::Handle();

	const Vector grid_p0 = target_rect.get_min();
	const Vector grid_p1 = target_rect.get_max();
	const Vector grid_size = grid_p1 - grid_p0;
	const int grid_side_count_x = std::max(1, (int)round(grid_size[0]/precision[0])) + 1;
	const int grid_side_count_y = std::max(1, (int)round(grid_size[1]/precision[1])) + 1;

	const Vector grid_step(
		grid_size[0]/(Real)(grid_side_count_x - 1),
		grid_size[1]/(Real)(grid_side_count_y - 1) );
	//const Real grid_step_diagonal = grid_step.mag();

	// build grid
	int visible_vertex_count = 0;
	std::vector<GridPoint> grid;
	grid.reserve(grid_side_count_x * grid_side_count_y);
	for(int j = 0; j < grid_side_count_y; ++j)
		for(int i = 0; i < grid_side_count_x; ++i)
		{
			Vector p( grid_p0[0] + i*grid_step[0],
					  grid_p0[1] + j*grid_step[1] );
			Point tp = back_transform->transform(p);
			if (tp.is_valid()) {
				grid.push_back(GridPoint(visible_vertex_count, Mesh::Vertex(p, tp)));
				++visible_vertex_count;
			} else {
				grid.push_back(GridPoint(-1, Mesh::Vertex()));
			}
		}

	if (visible_vertex_count == 0)
		return Mesh::Handle();

	// copy vertices to mesh
	Mesh::Handle mesh(new Mesh());
	mesh->vertices.reserve(visible_vertex_count);
	for(std::vector<GridPoint>::const_iterator i = grid.begin(); i != grid.end(); ++i)
		if (i->first >= 0) mesh->vertices.push_back(i->second);

	// build triangles
	mesh->triangles.reserve(visible_vertex_count * 2);
	for(int j = 0; j < grid_side_count_y; ++j)
	{
		for(int i = 0; i < grid_side_count_x; ++i)
		{
			int tl = grid[(j-1)*grid_side_count_x + i-1].first;
			int tr = grid[(j-1)*grid_side_count_x + i  ].first;
			int bl = grid[    j*grid_side_count_x + i-1].first;
			int br = grid[    j*grid_side_count_x + i  ].first;
			int mode = (tl >= 0 ? 1 : 0)
					 | (tr >= 0 ? 2 : 0)
					 | (bl >= 0 ? 4 : 0)
					 | (br >= 0 ? 8 : 0);
			switch(mode)
			{
			case 1|2|4|8:
				mesh->triangles.push_back(Mesh::Triangle(tl, tr, bl));
				mesh->triangles.push_back(Mesh::Triangle(bl, tr, br));
				break;
			case 2|4|8:
				mesh->triangles.push_back(Mesh::Triangle(bl, tr, br));
				break;
			case 1|4|8:
				mesh->triangles.push_back(Mesh::Triangle(tl, br, bl));
				break;
			case 1|2|8:
				mesh->triangles.push_back(Mesh::Triangle(tl, tr, br));
				break;
			case 1|2|4:
				mesh->triangles.push_back(Mesh::Triangle(tl, tr, bl));
				break;
			default:
				break;
			}
		}
	}

	return mesh;
}

Transformation*
Transformation::create_merged(const Transformation& other) const
{
	if (can_merge_inner(other)) {
		Transformation *t = clone();
		if (!t) return 0;
		if (!t->can_merge_inner(other)) return 0;
		t->merge_inner(other);
		return t;
	} else
	if (other.can_merge_outer(*this)) {
		Transformation *t = other.clone();
		if (!t) return 0;
		if (!t->can_merge_outer(*this)) return 0;
		t->merge_outer(*this);
		return t;
	}
	return 0;
}

bool
Transformation::can_merge_outer(const Transformation& other) const {
	return (bool)dynamic_cast<const TransformationNone*>(&other)
		 || can_merge_outer_vfunc(other);
}

bool
Transformation::can_merge_inner(const Transformation& other) const {
	return (bool)dynamic_cast<const TransformationNone*>(&other)
		 || can_merge_inner_vfunc(other);
}

bool
Transformation::merge_outer(const Transformation& other) {
	if (!can_merge_outer(other)) return false;
	merge_outer_vfunc(other);
	return true;
}

bool
Transformation::merge_inner(const Transformation& other) {
	if (!can_merge_inner(other)) return false;
	merge_inner_vfunc(other);
	return true;
}

Mesh::Handle
Transformation::build_mesh(const Rect &target_rect, const Vector &precision) const
{
	const Real epsilon = real_low_precision<Real>();

	if (!target_rect.is_valid())
		return Mesh::Handle();

	Vector valid_precision(fabs(precision[0]), fabs(precision[1]));
	if (std::isnan(valid_precision[0]) || std::isinf(valid_precision[0]))
		valid_precision[0] = target_rect.get_width();
	if (std::isnan(valid_precision[1]) || std::isinf(valid_precision[1]))
		valid_precision[1] = target_rect.get_height();
	if (valid_precision[0] < epsilon)
		valid_precision[0] = epsilon;
	if (valid_precision[1] < epsilon)
		valid_precision[1] = epsilon;

	return build_mesh_vfunc(target_rect, valid_precision);
}

/* === E N T R Y P O I N T ================================================= */
