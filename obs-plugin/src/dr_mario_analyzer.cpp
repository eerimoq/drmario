/*
Dr. Mario
Copyright (C) 2022 Erik Moqvist <erik.moqvist@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <obs-module.h>
#include "plugin_macros.h"
#include "dr_mario_analyzer.hpp"

namespace DrMario {

Analyzer::Analyzer()
{
    blog(LOG_INFO, "DrMario::Analyzer()");
}

Analyzer::~Analyzer()
{
    blog(LOG_INFO, "~DrMario::Analyzer()");
}

}
