#include <pgen.h>

int main(){
	/* 独自ディスクリプタを確保 */
	pgen_t* handle = pgen_open("en0", NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;
	}

	/* パケットのインスタンスを生成し、設定 */
	pgen_arp packet;
	packet.ETH.src       = "00:11:22:33:44:55";
	packet.ETH.dst       = "aa:bb:cc:dd:ee:ff";
	packet.ETH.type      = 0x0806;
	packet.ARP.operation = 2;
	packet.ARP.srcEth    = "77:77:77:77:77:77";
	packet.ARP.srcIp     = "192.168.10.11";
	packet.ARP.dstEth    = "aa:bb:cc:dd:ee:ff";
	packet.ARP.dstIp     = "192.168.10.1";
	
	packet.send_handle(handle); /* パケット送信 */
	pgen_close(handle);  /* ディスクリプタを解放 */
}




