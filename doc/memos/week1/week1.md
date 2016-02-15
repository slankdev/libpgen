
# Week1 Memos

 - Feb 3 (Wed) on Cybozu Lab
 - Feb 4 (Thu) 
 - Feb 5 (Fri) on Cybozu Lab





## 2/3

# 2/5

今日は「C++のためのAPIデザイン」をよんで必要になりそうなところをすこしまとめた。




# 2/5(金)のメモ

## 調べた内容

 - 例外処理
 - privateなメンバ変数は_から始る
 - 参照を返すときはconst 参照を返すようにする
 - constメンバ関数使う
 - exceptionクラスってのを使うぜ(ほんとに初めてしった)




## たまたま浮かんだアイディア

アドレスクラスで&&, ||の実装をしよう。
ネットワークアドレスとかの演算で便利になりそうだから実装しとこう。



## 感想(偏見多し)

 - try/catchは入門書でしか触ったことなかった。
 - exceptionクラス、ほーー
 - setter,getterのあらたな表現方法を知った。まあ、よく考えればわかるけど、多機能でいいすね


### 次にやること

 - [ ] 最小単位のクラスの詳細設計
 	- [ ] address class
		- [ ] ipaddr
		- [ ] macaddr
	- [ ] protocol header
		- [ ] ethenet
		- [ ] arp
		- [ ] ip 
		- [ ] icmp 
		- [ ] tcp 
		- [ ] udp



