

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


#include "pgen-error.h"
#include <stdio.h>


int pgen_errno=0;


char* pgen_strerror(int errno){
	printf("pgen-errno = %d \n", pgen_errno);
	return nullptr;	
}


void pgen_perror(const char* str){
	fprintf(stderr, "%s: %s", str, pgen_strerror(pgen_errno));	
}
