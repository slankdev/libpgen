

# LibPGEN version 2.0 alpha

Version1.0は設計が安定していなく、C, C++のどちらのスタイルにもどっちつかずな
やり方で実装していた。



## コンセプト

Version2.0では以下の内容をコンセプトに設計、開発をしていきたい。

 - C++的設計思想を貫く(ある程度は)
 - パケットクラスの使い方などはあまり変えたくない
 - パケットの各要素へのアクセスが自由すぎて危険。ある程度束縛をする
 - 入出力系関数がCのスタイルなので、それをC++スタイルに書き直す

また、開発を進めるにあたり、以下だけは絶対に誰にも譲れないものがあり、それを以下に示す。

 - パケット単位での通信の考え方。(もしpingを作るなら、icmpパケットを作って送る)
 - パケットの各プロトコルヘッダはユーザが直接扱えるようにする


## これ使ってやりたい



## LibPGEN Overview


### 名前空間

pgen_arpとかじゃなくてpgen::arpてきな使い方をしたいね


```
pgen::pgen_t handle(filename, pgen::mode::pcap_write);
```

### 例外処理

try catch構文でエラーを捉えられるようにしたい。

```
/* version1.0はこんな */
pgen_t* handle = pgen_open(filename, mode);
if (handle == NULL) {
	pgen_perror("oops");
	return -1;
}

/* これからはこうでしょ */
try { 
	pgen::pgen_t handle(filename, mode);
} catch(std::exception e) {
	printf("error: %s \n", e.what());
}
```




## 今のところの問題点(解決済み)




## 今のところの問題点(未解決)

### ヘッダの要素間の依存問題

長さなど、依存する場合の問題をどうしよう。。

### 好き勝手パケットをいじれる機能をどう残そう

前の問題と逆な問題だが、できればMalformed Packetも作れるようにしたい。
そういうパケットも好きだから。。。(個人的思想)


