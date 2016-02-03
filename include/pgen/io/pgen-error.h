
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
 */




#ifndef PGEN_ERROR_H
#define PGEN_ERROR_H


char* pgen_strerror(int errno);			/* return error message */

extern int pgen_errno_native;
extern int pgen_errno;






/* Pgen-Func's Errno */
#define PG_ERRNO_UNDEFINED          -1  /* undefined error             */ 
#define PG_ERRNO_NOWRITE         10001  /* descriptor isn't writeable  */
#define PG_ERRNO_NOREAD          10002  /* descriptor isn't readable   */
#define PG_ERRNO_TIMEOUT         10003  /* timeout                     */
#define PG_ERRNO_OPENBPF         10004  /* cant open bpf               */
#define PG_ERRNO_GETHARDADDR_BSD 10005  /* get hardware address on bsd */
#define PG_ERRNO_NOSUPPORT       10006  /* not support                 */
#define PG_ERRNO_BUFLEN_FAIL     10007  /* buffer length no support    */
#define PG_ERRNO_MODENFOUND       10008  /* mode not found              */




/* System-Call's Errno */
#define PG_NERRNO_WRITE              8
#define PG_NERRNO_READ               9
#define PG_NERRNO_FOPEN             11
#define PG_NERRNO_FREAD              7
#define PG_NERRNO_FWRITE             6
#define PG_NERRNO_SOCKET            13
#define PG_NERRNO_SETSOCKOPT        15
#define PG_NERRNO_IOCTL             12
#define PG_NERRNO_SELECT            14
#define PG_NERRNO_BIND               1
#define PG_NERRNO_SENDTO            10







#endif
