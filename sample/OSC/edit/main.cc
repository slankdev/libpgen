#include <pgen.h>

bool func(const u_char* packet, int len){
	pgen_unknown unknown(packet, len);
	if(unknown.isUDP && unknown.portis(53)){
		pgen_dns dns(packet, len);
		if(dns.DNS.flags.qr == 0){
			dns.summary(); 	 /* 情報表示 */
			dns.hex();		 /* hex表示  */

			dns.DNS.query[0].name = "www.ospn.jp";  /* queryのnameを変更*/
			dns.summary();   /* 情報表示 */ 
			dns.hex();       /* hex表示  */ 
		}
	}
	return true;
}

int main(int argc, char** argv){
	/* 独自ディスクリプタを確保 */
	pgen_t* handle = pgen_open("en0", NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;	
	}
	sniff(handle, func);  /* パケット受信ループ */
	pgen_close(handle);   /* ディスクリプタを解放 */
}


