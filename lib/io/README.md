

# PGEN-IO
This component has many functions that send/recv packet to network interface and pcap files.  



## Function List


### pgen-io.cc

	pgen_open();                 --> Open PGEN-discripter available handling network interface
	pgen_open_offline();         --> Open PGEN-discripter available handling pcap file
	pgen_close();                --> Close PGEN-discripter
	pgen_sendpacket_handle();    --> Send binary with PGEN-discripter
	pgen_sendpacket_L2();        --> send binary to L2 network interface from device-name

### pgen-netutil.cc

	pgen_send_to_netif ();       --> Send binary to network-interface with datalink socket
	pgen_recv_from_netif();      --> Recv binary from network-interface with datalink socket
	pgen_recv_from_netif_to();   --> Timeout recv binary from network-interface with datalink socket 
                                      
	pgen_getipbydev();           --> Get IP address from network-interface
	pgen_getmaskbydev();         --> Get NetMask from network-interface
	pgen_getmacbydev();          --> Get Mac address from network-interface
	pgen_port2service();         --> Get service from port-number

### pgen-util.cc

	pgen_send_to_pcap();         --> Send binary to file with file pointer
	pgen_recv_from_pcap();       --> Recv binary from file with file pointer
	pgen_hex();                  --> Hexdump function


### pgen-error.cc

	pgen_strerror();             --> Get error message that is char* string from error number
	pgen_perror();               --> Print error message with original message like perror()
