/* === S Y N F I G ========================================================= */
/*!	\file curvewarp.h
**	\brief Header file for implementation of the "Curve Warp" layer
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007-2008 Chris Moore
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
**
** === N O T E S ===========================================================
**
** ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_CURVEWARP_H
#define __SYNFIG_CURVEWARP_H

/* === H E A D E R S ======================================================= */

#include <vector>
#include <synfig/vector.h>
#include <synfig/layer.h>
#include <synfig/blinepoint.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

using namespace synfig;
using namespace std;
using namespace etl;

class CurveWarp : public Layer
{
	SYNFIG_LAYER_MODULE_EXT

private:
	std::vector<synfig::BLinePoint> bline;

	Point origin;
	Real width;
	Point start_point;
	Point end_point;
	Real curve_length_;
	Vector perp_;
	bool fast;

	void sync();

	synfig::Color color_func(const synfig::Point &x, int quality=10, float supersample=0)const;

	float calc_supersample(const synfig::Point &x, float pw,float ph)const;

public:
	CurveWarp();

	virtual bool set_param(const String &param, const ValueBase &value);
	virtual ValueBase get_param(const String &param)const;
	virtual Point transform(const Point &point_, int quality=10, float supersample=0)const;
	virtual Color get_color(Context context, const Point &pos)const;
	virtual bool accelerated_render(Context context,Surface *surface,int quality, const RendDesc &renddesc, ProgressCallback *cb)const;
	synfig::Layer::Handle hit_check(synfig::Context context, const synfig::Point &point)const;

	virtual Vocab get_param_vocab()const;
};

/* === E N D =============================================================== */

#endif