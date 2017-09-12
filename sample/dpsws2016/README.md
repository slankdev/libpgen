
# これは何

DPSWS2016田沢湖で発案した
アプリケーションのコードです

テキストで指定したフォーマットでパケットを解析するdpsws2016-analyzeと、
テキストで指定したデータメッセージのパケットを作成するdpsws2016-genereteのプロトタイプなプログラムです。

## Usage

```
$ make clean
$ make
$ sudo ./dpsws2016-analyze ICMP-captured.pcap < dpsws2016-fmt-ICMP.txt
$ sudo ./dpsws2016-generate ICMP-generated.pcap < dpsws2016-msg-ICMP.txt
```

### パケット指定用フォーマット

# フォーマット書式

description:dig|hex:bytes[:text]
.............

# フォーマット書式の解説

description = 任意のASCIIテキスト
dig|hex = データの表記方法の選択、dig=10進数表記、hex=16進数表記
bytes = データのバイト数、0または-1を指定すると無制限を指定
[text] = パケット生成時に投入するデータ自体、前記のデータ指定方法(dig|hex)に則って、1バイトずつ、スペース区切りで記載する

# サンプル

dpsws2016-fmt-ICMP.txt = ICMPパケットを解析するためのフォーマット書式例(データ無し)
dpsws2016-msg-ICMP.txt = ICMPパケットを作成するためのフォーマット書式例(データ有り)

#### 注意事項

現在のプロトタイプなプログラムでは、解析(dpsws2016-analyze)も作成(dpsws2016-generate)も、1パケットしか取り扱えません。
解析時に複数パケットが含まれるpcapファイルをしていた場合、先頭の1パケットだけを解析します。

 

