
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




#ifndef PGEN_ERROR_H
#define PGEN_ERROR_H



extern int pgen_errno;
extern int pgen_errno2;

#define PG_ERRNO_SOCKET  0
#define PG_ERRNO_BIND    1
#define PG_ERRNO_HDRINC  2
#define PG_ERRNO_PROMISC 3

#define PG_ERRNO_WONLY   4
#define PG_ERRNO_RONLY   5
#define PG_ERRNO_FWRITE  6
#define PG_ERRNO_FREAD   7
#define PG_ERRNO_WRITE   8
#define PG_ERRNO_READ    9
#define PG_ERRNO_SENDTO 10

#define PG_ERRNO_FOPEN  11


#ifndef __linux
#define PG_ERRNOBSD_OPENBPF -1
#define PG_ERRNOBSD_SETBUF  -2
#define PG_ERRNOBSD_BIND    -3
#define PG_ERRNOBSD_PROMISC -4
#define PG_ERRNOBSD_IMDAT   -5
#define PG_ERRNOBSD_RCVALL  -6
#define PG_ERRNOBSD_FLUSH   -7
#define PG_ERRNOBSD_NCMPMAC -8
#endif




char* pgen_strerror(int errno);
void pgen_perror(const char* str);


#endif
