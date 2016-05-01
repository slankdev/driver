# キャラクタ型デバイスドライバ

## 感想

 - 2.6の解説書と全然ちげーじゃねーかよ！！！

## 注意

作業を簡単に進めたいので、以下の点を最初に決めて作業を進めることとする。

 - major番号の割り当ては動的に行う
 - デバイス構造体の中にcdev構造体を入れる
 - scull_trim見たいなやつは実装してない。


## デバイスドライバプログラミングで必要なこと

 1. init関数
	 1. デバイス番号の割り当て
	 1. キャラクタデバイスの登録
	 1.
	 1.
 1.
 1. exit関数
 	 1. デバイス番号の開放
	 1. 
	 1. 
	 1. 

## 実装しなければならない関数

 - 


## 重要な関数

 - linux/fs.h
	 - alloc_chrdev_region() デバイス番号の動的割り当て
	 - unregister_chrdev_region() デバイス番号の解放
 - linux/cdev.h
 	 - cdev_init() 割当済みのcdev構造体を初期化
	 - cdev_add() 初期化されたcdevにデバイスを追加
	 - cdev_del() 初期化されたcdevからデバイスを消去
 - asm/uaccess.h
	 - copy_to_user() カーネルランド -> ユーザランド
	 - copy_from_user() ユーザランド -> カーネルランド
 - linux/slab.h
	 - kmalloc()
	 - kfree()

## 重要なデータ構造

 - linux/fs.h
	 - struct file_operations デバイス処理メソッドの関数ポインタをもつ
	 - struct file オープンされいているファイルに対して1つ容易され管理する
	 - struct inode ファイルを内部的に表現
 - linux/cdev.h
	 - struct cdev キャラクタデバイスを表現
	 -
 -



## openメソッドがやること

 - デバイス固有のエラーをチェック
 - デバイスが初めて開かれた場合は初期化
 - f_opポインタを更新
 - filp->private_dataにメモリを確保する

## releaseメソッドがやること

 - filp->private_dataの割り当てを解放する
 - 最後にクローズする場合はデバイスをシャットダウンする


## read,writeメソッドがやること

 - copy_to_user(), copy_from_user()を使ってユーザランドとカーネルランドをつなげる
 -
 -
