# libpgen (LIBrary Packet GENerator)
Packet generator library for C++  
About this library, please watch http://slankdev.net/libpgen  

## 概要、コンセプト

!!!使用するときはライセンスを読んでの使用をしてください!!!  
  
コンセプトは以下のとうりです。
* 簡単で高度にパケットを設定して送ることができる
* 色々とヘッダを調べたり、する必要がない
* グチャグチャに依存しあった型にとらわれなくていい
* ネットワークプログラミングのめんどくさいこと（ややこしく絡み合った構造体、
	バイトオーダ操作、アドレス操作）をしなくていい
* プログラミング素人でも、ネットワークの最低限の知識があれば使いこなせる
  
  
ネットワークプログラミング用のC/C++のライブラリです。  
libpcapのようにOSの型にとらわれず、簡単に高度な処理をできるようなライブラリにしていきたいです。
libpgenは低レイヤから高レイヤまで様々なパケットを作成して送信することを助ける関数を含んでいます。
ネットワークプログラミングのめんどくさいところ（アドレス操作、バイトオーダ操作、ややこしく絡み合った構造体）
を全てライブラリに処理をさせてシンプルなコードだけで細かくパケットをいじりたおせるようにしています。
scapyを少し参考にしながら作っていますが、C/C++でpython並の超高級処理ができればプログラミングをもっと楽しく
できると思います。
現在はlinux専用でコードを書いていますが、いずれはOS X、Windows 向けのコードも追加していけたらいいと思います。
似たようなものとしては、hpingやpython のscapyなどがありますが、
hpingやscapyなどより細かくパケットを編集できるようにしていこうと思います。
  

 

## 利用可能環境
os   : Ubuntu-Gnome 14.04  
build: make g++11  

 
## 追加予定機能
* 別のOS対応
* .pgenファイルでのパケットフォーマットの取り込み
* 様々なパケットに対応



## Developer

Handlename  : Slank  
Fullname    : Hiroki Shirokura  
Country     : Japan  
E-mail      : slank.dev@gmail.com  


## Support Protocols
Ethernet, ARP, IP, ICMP, TCP, UDP, DNS(over UDP), ar_drone


## Install 
	
	$ cd libpgen/src
	$ make
	$ sudo make install

## Uninstall 

	$ cd libpgen/src
	$ sudo make uninstall


## LICENCE1
	
	本ライブラリを使用して作成されたプログラムを使用したことによって
	起きた問題などに関しては、ライブラリ製作者は責任を持たず、
	すべてライブラリ使用者の自己責任とさせていただきます。
	
	システムに影響などが起こってしまったり、
	その他の問題が起きた場合は、ライブラリ製作者に連絡をしてください。
	責任は負いませんが、協力は致します。

## LICENCE2
GNU GPL v2.0 https://www.gnu.org/licenses/gpl-2.0.html 
