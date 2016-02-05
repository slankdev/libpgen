
# DesignExample 0205

とりあえず今日までの設計例。
クラス名などは(仮)でお願いします！



## 全体構造

全体的な構造はこんな感じ


```
namespace pgen {

	/* Module that implemented with core and io */
	namespace module {}

	/* Packet and address classes */
	namespace core {}

	/* IO functions and classes */
	namespace io {}

	/* define and typedef types */
	namespace types {}

}
```


## 細部構造

細部構造はこのように設計していく。
関数のプロトタイプは引数や戻り値などを省略している。

### PGEN-Module

```
namespace module {
	// This component's designing will be do after designed core and io. 
}
```


### PGEN-Core


```
namespace core {

	namespace protocol_header {}

	class packet;
	class ethernet  : packet; /* has eth header              */
	class arp       : packet; /* has eth and arp header      */
	class ip        : packet; /* has eth and ip header       */
	class icmp      : packet; /* has eth, ip and icmp header */
	class tcp       : packet; /* has eth, ip and tcp header  */
	class udp       : packet; /* has eth, ip and udp header  */

	class address;
	class ipaddr  : address;
	class macaddr : address;
	class port;
	          
}
```

#### packet classes

拡張データに関しての機能をこれから設計していく

パケットクラスの基本クラスを以下に示す。

```
class packet {
	private:
		pgen::types::uint8* _raw_data;
		pgen::types::uint64 _raw_length;
	public:
		packet();
		~packet();

		raw();
		length();

		hex();

		virtual clear();
		virtual compile();
		virtual cast();

		virtual summary();
		virtual info();
		virtual help();
};

```

次に様々なプロトコルに対してのパケットクラスを示す。ここでは例として、ethernetとarpのみを説明していく。

```
class eth : packet {
	private:
	public:
		pgen::core::protocol_header::eth eth;

		clear();
		compile();
		cast();
		summary();
		info();
		help();
};



class arp : packet {
	private:
	public:
		pgen::core::protocol_header::eth eth;
		pgen::core::protocol_header::arp arp;

		clear();
		compile();
		cast();
		summary();
		info();
		help();
};
```

実際の使用例を以下に簡単に示す。

```
pgen::core::packet::arp pack;

pack.arp.hwsrc() = "sdfadf";
pack.arp.qsrc() = asdfa;
pack.ethernet.src() = asdfa;
...
```

***この時必ず、上のレイヤから設定を行わせるようにユーザを束縛する。***
 

##### protocol header class

プロトコルごとの変数をもつクラス。全ての要素に対してのアクセスはsetter, getterを使ってのみアクセス可能。
例としてethernetとarpクラスについてのみ記述してく。

```
namespace protocol_header {
	
	/* ethernet */
	namespace ethernet {
		enum type {
			...etc
		}
		class header;
	}

	/* arp */
	namespace arp {
		class header;
	}

	/* ip */
	namespace ip {
		enum protocol {
			...etc
		}
		class header;
	}                                            

	/* icmp */
	namespace icmp {
		enum type {
			...etc;
		}
		enum destination_unreachable_code {}
		enum redirect_message_code {}
		enum timeexceeded_code {}
		...etc

		class header;
	}
}
```


#### address class


#### パケットクラスのバイナリデータの扱いについて (仮) -> 光成さんに相談する

パケットクラスではprivate変数でパケットのバイナリ本体を確保する方法を取っていたが、
固定長なため、一般的でなくなることがある。また、無駄ができてしまうことも。
以上の理由から、パケットのバイナリの確保、開放はmalloc,realloc,freeをパケットクラスの
コンストラクタ、デストラクタで呼び出して、その都度メモリを動的確保することとする。

コンストラクタでは、長さ0のメモリ領域をmalloc()で確保して、その都度、compile()時に必要に応じた
メモリ領域をrealloc()で再確保していく感じにする。

光成さんはvector<char>でやると言っていたが、その方法が具体的にまだ不明なので、もう少し考える。

追記)
realloc()はいらないことをやりすぎるらしい。。。やめとくか.






### PGEN-IO


```
namespace io {
	class IO; // IO handler to files and netifs.

	/* Utility classes */
	namespace util {
		
	}

	/* Network utility classes */
	namespace netutil {
		
	}

}
```





### Others


```
namespace types {
	typedef unsigned char  uint8;
	typedef unsigned short uint16;
	typedef unsigned int   uint32;
	typedef unsigned long  uint64;
	typedef unsigned logn long uint128;
	...etc
}
```



