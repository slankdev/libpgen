

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#ifndef DEBUG_H
#define DEBUG_H



#include <sys/types.h>
#include "pgen.h"
#include "pgen-types.h"

void debugprint(int flag, const char* str);
void pgen_hex(const u_char* data, int len);
int pgen_check(pgen_packet*, struct pgen_checkopt*);

#endif /* DEBUG_H */
