
/*
 * パケットクラスの使用例
 * ヘッダ要素のアクセスは全て内部でsetter, getterを使用しているものとする。
 * 幾つか例を示すが、これはただ、インターフェースの差なので
 * 対して差はないが、個人的には少しだけこだわりがある。。。。(笑)
 */
#include <pgen2.h>



/* 一番無難(とりあえず、これで実装して行こうと思う) */
int example2() {
	pgen::core::arp pack;

	pack.ARP.set_operation(pgen::arp::reply);
	pack.ARP.set_hwsrc("22:22:22:22:22:22");
	pack.ARP.set_psrc("192.168.0.2");
	pack.ARP.set_hwdst("11:11:11:11:11:11");
	pack.ARP.set_pdst("192.168.0.1");
	pack.ETH.set_src("22:22:22:22:22:22");
	pack.ETH.set_dst("11:11:11:11:11:11");
	
	pack.summary();
	pack.hex();
}

/* 全ての要素をクラス化(超コード増えそうだけど、好き) */
int example2() {
	pgen::core::arp pack;

	pack.ARP.operation.set(pgen::arp::reply);
	pack.ARP.hwsrc.set("22:22:22:22:22:22");
	pack.ARP.psrc.set("192.168.0.2");
	pack.ARP.hwdst.set("11:11:11:11:11:11");
	pack.ARP.pdst.set("192.168.0.1");
	pack.ETH.src.set("22:22:22:22:22:22");
	pack.ETH.dst.set("11:11:11:11:11:11");
	
	pack.summary();
	pack.hex();
}


/* 全ていままでどうりにアクセス可能(どうやって表現すればいいのか不明) */
int example1() {

	pgen::arp pack;

	pack.ARP.operation = pgen::arp::reply;
	pack.ARP.hwsrc = "22:22:22:22:22:22";
	pack.ARP.psrc  = "192.168.0.2";
	pack.ARP.hwdst = "11:11:11:11:11:11";
	pack.ARP.pdst  = "192.168.0.1";
	pack.ETH.src   = "22:22:22:22:22:22";
	pack.ETH.dst   = "11:11:11:11:11:11";
	
	pack.summary();
	pack.hex();
}
