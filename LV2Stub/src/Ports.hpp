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

#ifndef PORTS_HPP_
#define PORTS_HPP_

// Plugin ports as declared in the .ttl file
enum LV2PluginPorts
{
	LV2PLUGIN_CONTROL			= 0,
	LV2PLUGIN_NOTIFY			= 1,
	LV2PLUGIN_AUDIO_IN			= 2,
	LV2PLUGIN_AUDIO_OUT			= 3,

	LV2PLUGIN_CONTROLLERS		= 4,
	LV2PLUGIN_VALUE_CTRL		= 0,
	LV2PLUGIN_TOGGLE_CTRL		= 1,
	LV2PLUGIN_ENUM_CTRL			= 2,
	LV2PLUGIN_NR_CONTROLLERS	= 3
};

#endif /* PORTS_HPP_ */