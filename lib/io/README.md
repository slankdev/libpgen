

# PGEN-IO
This component has many functions that send/recv packet to network interface and pcap files.  



## Function List

### pgen-error.cc

	pgen_strerror();           --> Get error message that is char* string from error number
	pgen_perror();             --> Print error message with original message like perror()

### pgen-io.cc

	pgen_open();               --> Open PGEN-discripter available handling network interface
	pgen_open_offline();       --> Open PGEN-discripter available handling pcap file
	pgen_close();              --> Close PGEN-discripter
	pgen_sendpacket_handle();  --> Send binary with PGEN-discripter
	pgen_sendpacket_L3();      --> Send binary to L3 network interface from device-name
	pgen_sendpacket_L2();      --> send binary to L2 network interface from device-name

### pgen-netutil.cc

	pgen_send_to_netif ();
	pgen_recv_from_netif_to();
	pgen_recv_from_netif();

	pgen_getipbydev();
	pgen_getmaskbydev();
	pgen_getmacbydev();
	pgen_port2service();

### pgen-util.cc

	pgen_send_to_pcap();
	pgen_recv_from_pcap();
	pgen_hex();
	checksum();
	checksumTcp();
	checksumUdp();
