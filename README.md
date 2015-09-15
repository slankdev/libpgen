# libpgen (LIBrary Packet GENerator)
詳しいドキュメントは開発者サイトを参照してください。 http://slankdev.net/libpgen

## 概要、コンセプト
li　bpgenはC++で書かれたパケット解析、送受信などを簡単にできるようにすることを目標としているライブラリです。

コンセプトは以下のとうりです。
* 簡単で高度にパケットを設定して送ることができる
* ネットワークプログラミングのめんどくさいこと（ややこしく絡み合った構造体、
	バイトオーダ操作、アドレス操作）をしなくていい
* プログラミング素人でも、ネットワークの最低限の知識があれば高度なネットワークツールを作成することができる。


## 利用可能環境
Ubuntu-Gnome14.04LTS, ArchLinuxで動作確認ができています。確認していませんんが、他のlinuxでも動作するはずです。
(2015/09/16) BSDをサポートしました。OSXでも動きます。
 
## 追加予定機能
* 別のOS対応　
* 様々なパケットに対応



## 開発者
Handlename  : Slank  
Fullname    : Hiroki Shirokura  
Country     : Japan  
E-mail      : slank.dev@gmail.com  

　
## 対応プロトコル
Ethernet, ARP, IP, ICMP, TCP, UDP, DNS, DHCP, ar_drone
　
## サンプルコード
- 　DNS query 		libpgen/sample/DNS/
- MITM attack		libpgen/sample/MITM/

more infomation see http://slankdev.net/libpgen  


## Install
	　
	$ cd libpgen
	$ make
	$ sudo make install

## Uninstall 
　
	$ cd libpgen
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
