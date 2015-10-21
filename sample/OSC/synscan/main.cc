#include <pgen.h>
#include <thread>
pgen_t* handle; ipaddr  target_ip; macaddr target_mac; int run;

void synscan(){
	pgen_tcp pack;
	pack.ETH.src.setmacbydev("en0");
	pack.ETH.dst = target_mac;
	pack.IP.src.setipbydev("en0");
	pack.IP.dst = target_ip;
	pack.IP.tot_len = 20+20;
	pack.TCP.flags.syn = 1;
		
	pack.TCP.src = 12345;
	pack.TCP.dst = 80;  pack.send_handle(handle); pack.hex();
	pack.TCP.dst = 443; pack.send_handle(handle); pack.hex();
	printf("\n"); sleep(2); run = -1;
}
bool capture(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isTCP && buf.IP.src==target_ip){
		pgen_tcp pack(packet, len);
		if(pack.TCP.flags.syn==1 && pack.TCP.flags.ack==1) pack.summary();
	}
	if(run < 0)	return false;
	else 		return true;
}
int main(int argc, char** argv){
	target_ip  = "173.194.126.144"; target_mac = "0:0:c:7:ac:1";
	if((handle = pgen_open("en0", NULL)) == NULL)	return -1;
	
	std::thread scan(synscan);
	run = 1; sniff(handle, capture);
	scan.join(); pgen_close(handle);
}
