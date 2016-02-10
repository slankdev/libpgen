
u_char b[1000];
int len;
pgen_unknown buf(p, len);
if(buf.isARP() && buf.Eth.dst() == "11:22:33:44:55:66") {
	pgen_arp pack(b, len);
	cout << pack.arp.operation ;
	pack.summary();
} 



packet* p0 = packetAnalyze(p.len);
cout << p0;
if (p0->type == arp) {
 	const arp& arppack = dynamic_cast<const arp&>(p0);
	arppack.arp.operation;
}



if (p0->type == tcp) {
 	const tcp& tcppack = dynamic_cast<const tcp&>(p0);
	tcppack.tcp.operation 
}



packet* packetAnalyze(const void* buffer, size_t len) {
	type = getPacketType(buffer, len);
	switch (type) {
		case ARP:
			return new arp(buffer, len);
	}
}
