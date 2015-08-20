#ifndef RELEASE_H
#define RELEASE_H





class libpgen{
	static const char* version = "0.01";
	static const char* name = "libpgen";
	static const char* fullname = "libpgen ver 0.01";
	static const char* author = "slank (Hiroki Shirokura)";
	static const char* author_email = "slank.dev@gmail.com";
}


/*	rifactering codes 2015/08/19
 *
 *	[*] address.h
 *	[ ] ardrone.cc
 *	[ ] arp.cc
 *	[ ] dns.cc
 *	[ ] eth.cc
 *	[ ] icmp.cc
 *	[ ] ip.cc
 *	[ ] makefile
 *	[ ] mytypes.h
 *	[*] netutil.cc
 *	[*] netutil.h
 *	[ ] packconf.h
 *	[ ] packet.cc
 *	[ ] packet.h
 *	[ ] pgen.h
 *	[ ] release.h
 *	[ ] sniff.cc
 *	[ ] sniff.h
 *	[ ] tcp.cc
 *	[ ] udp.cc
 *	[ ] unknown.cc
 *	
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
 *		[2015.05.11]	add function port2service(int)		[2015.05.11]
 *		[2015.05.16]	add function summary(void)			[2015.06.23]
 *						print packet info inline
 *		[2015.05.16]	add function nslookup(const)		[]
 *						ipaddr nslookup(url);		 
 *		[2015.07.29]	add packet type	unknown				[2015.07.30]
 *						for recv packet
 *		[2015.08.21]    add func sniff_with_filter()		[]
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
 *		[2015.06.28]	dns.cc:60 has memory error			[]
 *		[2015.07.30]	CAST has big memory error			[]
 *						
 *		
 *		CHANGES
 *		[2015.06.12]	typedef int32 bit32
 *						typedef int16 bit16...
 */

#endif
