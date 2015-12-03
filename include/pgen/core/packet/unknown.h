
/*
 * copyright (c) <2015>  <slank hiroki shirokura>
 * 
 * this program is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 3 of the license, or
 * (at your option) any later version.
 * 
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 * 
 * you should have received a copy of the gnu general public license
 * along with this program.  if not, see <http://www.gnu.org/licenses/>
 *
 */




#ifndef UNKNOWN_H
#define UNKNOWN_H



#include <pgen/core/address/address.h>




class _macsrcdst{
	public:
	macaddr src;
	macaddr dst;
	bool operator==(macaddr addr){ return (addr==src | addr==dst);}
};

class _ipsrcdst{
	public:
	ipaddr src;
	ipaddr dst;
	bool operator==(ipaddr addr){ return (addr==src | addr==dst);}
};

class _portsrcdst{
	public:
	unsigned short src;
	unsigned short dst;
	bool operator==(unsigned short port){ return (port==src | port==dst);}
};


class _unknown_mac{
	public:
	_macsrcdst _addr;
	
	macaddr src(){ return _addr.src; }
	macaddr dst(){ return _addr.dst; }
	_macsrcdst addr(){ return _addr; }
};


class _unknown_ip{
	public:
	_ipsrcdst _addr;
	
	ipaddr src(){ return _addr.src; }
	ipaddr dst(){ return _addr.dst; }
	_ipsrcdst addr(){ return _addr; }
};


class _unknown_port{
	public:
	_portsrcdst _port;

	unsigned short src(){ return _port.src;}
	unsigned short dst(){ return _port.dst;}
	_portsrcdst port(){ return _port; }
};


class pgen_unknown{
	private:
		bit8 __data[PGEN_MAX_PACKET_LEN];
		int  __len;
		bool __isETH;
		bool __isARP;
		bool __isIP;
		bool __isICMP;
		bool __isTCP;
		bool __isUDP;

	public:
		_unknown_mac ETH;
		_unknown_ip IP;
		_unknown_port TCP;
		_unknown_port UDP;

		pgen_unknown();
		pgen_unknown(const void*, int);
		void clear();
		int  cast(const void*, int);
		void send(pgen_t*);
		void summary();
		void hex();

		bool isETH();
		bool isARP();
		bool isIP();
		bool isICMP();
		bool isTCP();
		bool isUDP();
};




#endif /* UNKNOWN_H */
