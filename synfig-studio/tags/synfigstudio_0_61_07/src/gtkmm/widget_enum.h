/* === S Y N F I G ========================================================= */
/*!	\file widget_enum.h
**	\brief Template Header
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_STUDIO_WIDGET_ENUM_H
#define __SYNFIG_STUDIO_WIDGET_ENUM_H

/* === H E A D E R S ======================================================= */

#include <synfig/layer.h>
#include <gtkmm/optionmenu.h>
#include <synfig/paramdesc.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace Gtk { class Menu; };

namespace studio {

class Widget_Enum : public Gtk::OptionMenu
{
	Gtk::Menu *enum_menu;
	synfig::ParamDesc param_desc;

	int value;
	void set_value_(int data);
public:

	Widget_Enum();
	~Widget_Enum();

	void set_param_desc(const synfig::ParamDesc &x);
	void refresh();

	void set_value(int data);
	int get_value() const;
}; // END of class Widget_Enum

}; // END of namespace studio

/* === E N D =============================================================== */

#endif
