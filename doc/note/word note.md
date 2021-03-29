# 工作笔记



[TOC]

## 一、u-boot

### 1.1、相关命令

​		联咏 u-boot 相关命令：

```sh
setenv ipaddr 192.168.1.57
setenv serverip 192.168.1.167


fdt:
  mw.b 0x0 ff 0x40000; tftpboot 0x0 nvt-na51068-evb.bin
  nand erase 0x40000 0x40000; nand write 0x0 0x40000 0x40000
uboot:
  mw.b 0x0 ff 0x200000; tftpboot 0x0 u-boot.bin
  nand erase 0xc0000 0x200000; nand write 0x0 0xc0000 0x200000
kernel:
  mw.b 0x0 ff 0x460000;tftpboot 0x0 uImage.bin
  nand erase 0x300000 0x460000; nand write 0x0 0x300000 0x460000
rootfs0:
  mw.b 0x0 ff 0x3200000;tftpboot 0x0 rootfs.ramdisk.bin.raw
  nand erase 0x760000 0x3200000; nand write 0x0 0x760000 0x3200000
rootfs1:
  mw.b 0x0 ff 0x2500000;tftpboot 0x0 rootfs_1.rw.ubifs.bin.raw
  nand erase 0x3960000 0x2500000; nand write 0x0 0x3960000 0x2500000  
app:
  mw.b 0x0 ff 0x21a0000;tftpboot 0x0 DVR_8CH.nand.ubifs.raw
  nand erase 0x5e60000 0x2000000; nand write 0x0 0x5e60000 0x2190000
  
  mw.b 1800000 ff 500000;tftp 1800000  rootfs.squash.bin.raw  
  sf probe 0;sf erase 460000 500000;sf write 1800000 460000 500000
  
mw.b 1800000 ff 8000000;tftp 0x1800000 FW98323A.bin
nvt_update_all 0x1800000 0
```

## 二、底层UI交叉编译

### 2.1、qt

- 参考网址
  - [[ARM版QT交叉编译——嵌入式平台QT开发环境搭建](https://www.iemblog.com/?p=1340&lang=zh)]

## 三、ad芯片

### 3.1、基础概念

### 3.2、bt656

#### 3.2.1、BT.656标准简介	-	[*转载*]

​		凡是做模拟信号采集的，很少不涉及BT.656标准的，因为常见的模拟视频信号采集芯片都支持输出BT.656的数字信号，那么，BT.656到底是何种格式呢？本文将主要介绍 标准的 8bit BT656（4:2:2）YCbCr SDTV（标清） 数字视频信号格式，主要针对刚刚入门模拟视频采集的初学者入门之用。

**1. 帧的概念（Frame）**

​		一个视频序列是由N个帧组成的，采集图像的时候一般有2种扫描方式，一种是[逐行扫描](http://baike.baidu.com/view/84623.htm#sub84623)（progressive scanning），一种是[隔行扫描](http://baike.baidu.com/view/115294.htm#sub115294)（interlaced scanning）。对于隔行扫描，每一帧一般有2个场（field），一个叫顶场（top field），一个叫底场（bottom field）。假设一帧图像是720行，那么，顶场就**包含**其中所有的偶数行，而底场则**包含**其中所有的奇数行。

**2. 场的概念（field）**  

​		注意，上面提到顶场和底场，用的是“包含”二字，而不是说完全由后者组成，因为在BT.656标准中，一个场是由三个部分组成的：

​			场 = 垂直消隐顶场（First Vertical Blanking） + 有效数据行（Active Video） + 垂直消隐底场（Second Vertical Blanking）

​		对于顶场，有效数据行就是一帧图像的所有偶数行，而底场，有效数据行就是一帧图像的所有奇数行。顶场和底场的空白行的个数也有所不同，那么，对于一个标准的 8bit BT656（4:2:2）SDTV（标清）的视频而言，对于一帧图像，其格式定义如下：



​		由上图可以知道，对于PAL制式，每一帧有625行，其中，顶场有效数据288行，底场有效数据也是288行，其余行即为垂直消隐信号。为什么是288行？因为PAL制式的SDTV或者D1的分辨率为 720*576，即一帧有576行，故一场为288行。

​		由上图我们还可以知道，顶场有效数据的起始行为第23行，底场有效数据的起始行为第335行。

​		另外，上图中的 F 标记奇偶场，V标记 是否为垂直消隐信号。

**3. 每一行的组成（Lines）**

 		下面说明每一行的组成，一行是由4个部分组成：

  				行 = 结束码（EAV） + 水平消隐（Horizontal Vertical Blanking） + 起始码（SAV） + 有效数据（Active Video）

  		典型的一行数据组成如下图所示：

[![img](http://img1.51cto.com/attachment/201104/213307428.png)](http://img1.51cto.com/attachment/201104/213307428.png)

​		起始码（SAV）和结束码（EAV），它是标志着一行的开始结束的重要标记，也包含了其他的一些重要的信息，后面将会讲到。

​		为什么水平消隐 是280字节，这个我暂时还没搞清楚，不知道是不是标准定义的。

​		为什么一行中的有效数据是 1440 字节？ 因为PAL制式的SDTV或者D1的分辨率为 720*576，即一行有720个有效点，由于采集的是彩色图像，那么一行就是由亮度信息（Y）和色差信息（CbCr）组成的，由于是 YCbCr422格式，故一行中有720列Y，720列CbCr，这样，一行的有效字节数就自然为 720 x 2 = 1440 字节了。

**4. EAV和SAV**

​		EAV和SAV都是4个字节（Bytes），由上面的图可以知道，SAV后面跟着的就是有效的视频数据了。那么，EAV和SAV的格式是怎么样的呢？

​		EAV和SAV的4个字节的格式规定如下（下面以16进制表示）：

  				FF 00 00 XY

​		其中，前三个字节为固定的，必须是FF 00 00，而第4个字节（XY）是根据场、消隐信息而定的，其8个bit含义如下： 1 F V H P3 P2 P1 P0

​		其中，F：标记场信息，传输顶场时为0，传输底场时为1

​					V：标记消隐信息，传输消隐数据时为1，传输有效视频数据时为0

​					H：标记EAV还是SAV，SAV为0，EAV为1

​		而 P0~P3为保护比特，其值取决于F、H、V，起到校验的作用，计算方法如下：

[![img](http://img1.51cto.com/attachment/201104/200501427.jpg)](http://img1.51cto.com/attachment/201104/200501427.jpg)

**5. 总结**

​		这篇文章仅仅讲了BT.656格式中最基本的一些概念，权当入门，许多视频采集芯片都会涉及这方面的知识。如有疑问，欢迎留言讨论。最后，感谢[《A Brief Introduction to Digital Video》](http://www.spacewire.co.uk/video_standard.html)这篇英文文章对BT.656的介绍，写得很不错，我基本上根据它了解BT656和写这篇文章的，有兴趣的童鞋可以看看。

—————————————————————————————————————————————————————————

以下转自特权博客http://www.eefocus.com/ilove314/blog/11-09/231422_89195.html

***\**\*ITU-RBT.656\*\*视频标准接口\****

ITU-RBT.656视频标准接口PAL制式（720*576）每场由四部分组成。

● 有效视频数据，分为奇场和偶场，均由288行组成。每行有1440个字节，其中720个字节为Y分量，360个字节为Cb分量，360个字节为Cr分量。Y分量的取值为16~235；Cb和Cr分量的取值为16~240。

● 水平消隐，有280个字节。

● 垂直消隐。

● 控制字。

对于有效数据行，其格式如图1所示。EAV和SAV为嵌入式控制字，分别表示有效视频的终点和起点。EAV和SAV均为4个字节构成，前3个字节FF、00、00为固定头，“XY”为控制字。“XY”的8个bit含义如下：

● Bit7（Const），常数，总为1。

● Bit6（F），场同步信号，表示该行数据处于奇场还是偶场。

● Bit5（V），垂直同步信号，表示处于场消隐区间还是正程区间（有效数据行）。

● Bit4（H），水平同步信号，表示是“SAV”还是“EAV”。

● Bit3-0（P3P2P1P0），纠错位。P3=V(XOR)H；P2=F(XOR)H；P1=F(XOR)V；P0=F(XOR)V(XOR)H。

EAV与SAV的详细定义如表1所示。

表1

| ***\*Bit7\**** | ***\*Bit6\**** | ***\*Bit5\**** | ***\*Bit4\**** | ***\*Bit3-0(P3P2P1P0)\**** | ***\*Hex\**** | ***\*Description\**** |
| -------------- | -------------- | -------------- | -------------- | -------------------------- | ------------- | --------------------- |
| 1              | 0              | 0              | 0              | 0000                       | 0x80          | Even,Active,SAV       |
| 1              | 0              | 0              | 1              | 1101                       | 0x9d          | Even, Active,EAV      |
| 1              | 0              | 1              | 0              | 1011                       | 0xab          | Even,Blank, SAV       |
| 1              | 0              | 1              | 1              | 0110                       | 0xb6          | Even, Blank, EAV      |
| 1              | 1              | 0              | 0              | 0111                       | 0xc7          | Odd, Active, SAV      |
| 1              | 1              | 0              | 1              | 1010                       | 0xda          | Odd, Active, EAV      |
| 1              | 1              | 1              | 0              | 1100                       | 0xec          | Odd, Blank, SAV       |
| 1              | 1              | 1              | 1              | 0001                       | 0xf1          | Odd, Blank, EAV       |

 

Blanking为水平消隐区，通常由80H/10H来填充。

![img](https://img-blog.csdn.net/20171117114255521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpeW9uZ3l1ZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 

![img](http://pic002.cnblogs.com/images/2012/125746/2012040817190474.jpg) ![img](http://pic002.cnblogs.com/images/2012/125746/2012040817190474.jpg)

图1

​     对于图1中的Valid data（有效数据）区，其数据排列顺序如图2所示。即Y : Cb : Cr="4" : 2 : 2。从图像的像素点上来理解，就是每个像素点有一个单独的Y值，而相邻的两个像素点的Cb和Cr数据是一样的。

 ![img](https://img-blog.csdn.net/20171117114343397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpeW9uZ3l1ZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](http://pic002.cnblogs.com/images/2012/125746/2012040817223280.jpg)

 图2

​     PAL一场的数据行格式如图3所示。 

![img](http://pic002.cnblogs.com/images/2012/125746/2012040817204554.jpg)

 ![img](https://img-blog.csdn.net/20171117114358264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpeW9uZ3l1ZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图3

​     工程实践中就是通过EAV和SAV对行、场信息进行检测，分离出有效数据。而EAV和SAV的差别只在控制字XY数据有别。因此表2对控制字XY和行、场之间的关系做了映射，并且理论上控制字XY数据流也是按照表2的上到下、左到右的顺序出现的。但是在实际工程中，特权同学发现奇场和偶场的顺序好像和这里的定义刚好反了。

表2

| **行数** | ***\*F\**** | ***\*V\**** | ***\*EAV\**** | ***\*SAV\**** |
| -------- | ----------- | ----------- | ------------- | ------------- |
| 1~22     | 0           | 1           | 0xb6          | 0xab          |
| 23~310   | 0           | 0           | 0x9d          | 0x80          |
| 311~312  | 0           | 1           | 0xb6          | 0xab          |
| 313~335  | 1           | 1           | 0xf1          | 0xec          |
| 336~623  | 1           | 0           | 0xda          | 0xc7          |
| 624~625  | 1           | 1           | 0xf1          | 0xec          |