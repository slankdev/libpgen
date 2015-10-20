#include <pgen.h>

bool func(const u_char* data, int len){
	pgen_unknown unknown(data, len);
	unknown.summary();  /* 情報表示 */
	unknown.hex();      /* hex表示  */
	return true;	
}


int main(){
	/* 独自ディスクリプタを確保 */
	pgen_t* handle = pgen_open("en0", NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;
	}
	sniff(handle, func); /* パケット受信ループ */
	pgen_close(handle);  /* ディスクリプタを解放 */
}


