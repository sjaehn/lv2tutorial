/* LV2 Plugin Template
 *
 * Copyright (C) 2018 - 2021 by Sven Jähnichen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef URIDS_HPP_
#define URIDS_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

// URIDs used for this plugin
struct LV2PluginURIDs
{
	LV2_URID lv2plugin_example;

	void map (LV2_URID_Map* m);
};

void LV2PluginURIDs::map (LV2_URID_Map* m)
{
	lv2plugin_example = m->map(m->handle, "https://www.your-url.com/LV2Stub#lv2plugin_example");
}

#endif /* URIDS_HPP_ */
