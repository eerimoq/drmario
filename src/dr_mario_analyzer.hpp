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

namespace DrMario {

class Statistics {
public:
    float number_of_viruses_defeated_per_pill;
    float number_of_pills_per_second;
};

class Analyzer {
public:
    Analyzer();
    virtual ~Analyzer();
    void update_bottle();
    void update_next_pill();
    const Statistics& statistics();
};

}
