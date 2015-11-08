# Pgen-IO

## Function List

### in pgen-error.cc

	char* pgen_strerror(int);
	void  pgen_perror(const cahr*);

### in pgen-io.cc

	pgen_t* pgen_open(const char*, void*);
	pgen_t* pgen_open_offline(const char*, int);
	void    pgen_close(pgen_t*);
	bool    sniff(pgen_t*, bool (*callback)(const u_char*, int));
	int     pgen_sendpacket_handle(pgen_t*, const void*, int);
	int     pgen_sendpacket_L3(const char*, const void*, int);
	int     pgen_sendpacket_L2(const char*, const void*, int);

### in pgen-netutil.cc

	int   pgen_send_to_netif (int fd, const void* buf, int len);
	int   pgen_recv_from_netif_to (int fd, void* buf, int len, struct timeval to);
	int   pgen_recv_from_netif (int fd, void* buf, int len);
	int   open_bpf (const char* dev, int promisc);
	int   initRawSocket (const char* dev, int promisc, int overIp);
	int   pgen_getipbydev (const char* dev, char* ip);
	int   pgen_getmaskbydev (const char* dev, char* ip);
	int   pgen_getmacbydev (const char* dev, char* mac);
	char* pgen_port2service (int port, const char* protocol, char* buf);

### in pgen-util.cc

	int pgen_send_to_pcap (FILE* fp, const void* buf, int len);
	int pgen_recv_from_pcap (FILE* fp, void* buf, int len);
	void pgen_hex (const void* d, int len);
	unsigned short checksum (unsigned short *data, int len);
	unsigned short checksumTcp (struct ip_header ip, struct tcp_header tcp, const void* data, int total_len);
	unsigned short checksumUdp (struct ip_header ip, struct udp_header udp, const void* data, int total_len);
