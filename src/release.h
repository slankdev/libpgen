

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


#ifndef RELEASE_H
#define RELEASE_H



/*	rifactering codes 2015/08/19
 *	[ ] return char* function (ipaddr.c_str()  etc...) check
 *
 *	[*] pgen.h
 *
 *	[*] address.h
 *
 *	[*] packet.h
 *	[*] packet.cc
 *  [*] eth.cc
 *	[*] arp.cc
 *	[*] ip.cc
 *	[*] icmp.cc
 *	[*] tcp.cc
 *	[*] udp.cc
 *	[*] dns.cc
 *	[*] ardrone.cc
 *	[*] unknown.cc
 *
 *  [*] mytypes.h
 *	[*] netutil.cc
 *	[*] netutil.h
 *	[*] release.h
 *	[*] debug.h
 *	[*] debug.cc
 * */


/*		TODO		[date]  about  [fin-date]
 *
 *
 *		[2015.05.01]	unmuch checksum in tcp		 		[2015.05.06]
 *						because checksum is fail in TCP
 *		[2015.05.02]	add copy constractor 				[2015.05.06]
 *						in pgent_ip, pgent_mac	
 *   	[2015.05.07]	add default variable of packet val	[2015.05.07]
 *   					ex) ip.dst 's defalut is 127.0.0.1
 *   	[2015.05.07]	set TCP val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.07]	set UDP val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.07]	set IP val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.07]	set ICMP val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.07]	set ARP val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.07]	set ETH val in object 				[2015.05.08]
 *   					ex) pack.IP.dst="192.168.0.6";
 *   	[2015.05.10]	make myPacket structure in ETH		[2015.05.10]
 *   	[2015.05.10]	make myPacket structure in ARP		[2015.05.10]
 *   	[2015.05.10]	make myPacket structure in IP		[2015.05.10]
 *   	[2015.05.10]	make myPacket structure in ICMP		[2015.05.10]
 *   	[2015.05.10]	make myPacket structure in TCP		[2015.05.10]
 *		[2015.05.10]	make myPacket structure in UDP		[2015.05.10]
 *		[2015.05.10]	add macro of packet options			[2015.05.10]
 *		[2015.05.11]	add function pgen_port2service()	[2015.05.11]
 *		[2015.05.16]	add function summary(void)			[2015.06.23]
 *						print packet info inline
 *		[2015.07.29]	add packet type	unknown				[2015.07.30]
 *						for recv packet
 *		[2015.05.16]	add function nslookup(const)		[]
 *						ipaddr nslookup(url);		 
 *		[2015.09.02]	pgen_open_offline(filename);		[2015.09.02]
 *						read pcap file format
 *		[2015.09.02]	ardrone ctrl command 				[2015.09.04]
 *
 *
 *							
 *
 *		BUG			[date]  about  [fin-date]
 *		[2015.06.28]	address.h:239 has some warning 		[2015.07.30]
 *						near malloc() and memset() 
 *						in string size is not true
 *		[2015.06.28]	address.h:23 has some warning 		[2015.07.30]
 *						near malloc() and memset() 
 *						in string size is not true
 *		[2015.06.28]	dns.cc:60 has memory error			[2015.08.27]
 *		[2015.07.30]	CAST has big memory error			[2015.08.27]
 *						
 *		
 *		CHANGES
 *		[2015.06.12]	typedef int32 bit32
 *						typedef int16 bit16...
 */

#endif /* RELEASE_H */
