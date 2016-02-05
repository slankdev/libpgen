
# 知らなかったことまとめ

## メンバ関数のconstコレクトネス

メソッドをconstで定義すると、そのメソッドないでは、全てのメンバ変数がconstになる。

```
class A {
	int a;
	int b;
	A(int _a, int _b) : a(_a), b(_b) {}
	int func() const {
		return a+b;
	}
};
```


