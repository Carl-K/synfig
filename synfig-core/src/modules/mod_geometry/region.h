/* === S Y N F I G ========================================================= */
/*!	\file region.h
**	\brief Header file for implementation of the "Region" layer
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2012-2013 Carlos López
**	......... ... 2018 Ivan Mahonin
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

#ifndef __SYNFIG_REGION_H
#define __SYNFIG_REGION_H

/* === H E A D E R S ======================================================= */

#include <synfig/layers/layer_shape.h>
#include <list>
#include <vector>
#include <synfig/value.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig { struct Segment; }

class Region : protected synfig::Layer_Shape
{
	SYNFIG_LAYER_MODULE_EXT
private:
	//! Parameter: list of type BlinePoint
	synfig::ValueBase param_bline;

public:
	Region();

	virtual bool set_shape_param(const synfig::String & param, const synfig::ValueBase &value);
	virtual synfig::ValueBase get_param(const synfig::String & param)const;
	virtual Vocab get_param_vocab()const;

protected:
	virtual void sync_vfunc();
};

/* === E N D =============================================================== */

#endif
