

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
#include <string.h>


#define PGEN_ERRSTR_LEN 256

int pgen_errno=0;
char pgen_errstr[PGEN_ERRSTR_LEN]={0};





char* pgen_strerror(int errno){
	strncpy(pgen_errstr, strerror(errno), PGEN_ERRSTR_LEN);

	return pgen_errstr;	
}


void pgen_perror(const char* str){
	fprintf(stderr, "%s: %s \n", str, pgen_strerror(pgen_errno));	
}
