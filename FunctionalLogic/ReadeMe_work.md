git lab
地址:  http://192.168.6.2/tq/linuxsecuritychecksystem
密码:  123456789
User: liaoxi

[simeServerExample_YuanZhenYue](https://github.com/eidheim/Simple-WebSocket-Server/blob/master/ws_examples.cpp)


# 目标操作系统
1. 银河麒麟 :192.168.6.231 
>sftp://root@192.168.6.231/ 
密码15个1+1个a+!
>如果连不上可能因为输入错误密码太多次数,导致锁定要等30分钟

处理器:(arm64)    4  phytium FT1500A  ( FT-1500A is a ARM64 SoC designed by Phytium, which includes 16 cores of ARMv8 processor, a 32-lane PCIe host, 2 GMAC on-chip ethernet controller and a GICv3 interrupt controller with ITS support.[12])




+ 给目录添加白名单:
 kysec_set -n exectl  -v verified -r ./
 >要在secadm用户下

 
2. 中标麒麟:192.168.6.230
>密码:6个1
>`bios`,帐号:`secadm`  密码: 15个a+1+!

CPU: MIPS64 CPUs 
```
使用内建 specs。
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/libexec/gcc/mips64el-neokylin-linux/4.9.3/lto-wrapper
目标：mips64el-neokylin-linux
配置为：../configure --prefix=/usr --mandir=/usr/share/man --infodir=/usr/share/info --with-bugurl=http://bugzilla.cs2c.com/bugzilla --enable-bootstrap --enable-shared --enable-threads=posix --enable-checking=release --enable-multilib --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-gnu-unique-object --enable-linker-build-id --with-arch=loongson3a --enable-languages=c,c++,objc,obj-c++,fortran,go,lto --enable-plugin --disable-libgcj --with-isl=/builddir/build/BUILD/gcc-4.9.3/obj-mips64el-neokylin-linux/isl-install --with-cloog=/builddir/build/BUILD/gcc-4.9.3/obj-mips64el-neokylin-linux/cloog-install --enable-gnu-indirect-function --with-long-double-128 --build=mips64el-neokylin-linux
线程模型：posix
gcc 版本 4.9.3 20150626 (NeoKylin 4.9.3-3) (GCC) 
[root@localhost ~]# 
```




3. NFS64
>中科方德(方德桌面),对应的VMOS:`/run/media/takethat/2t_t5/VM_OS/nfs64/`
ID: secadm  密码: 123456



# bug
 + 在VM_Ubuntu14获取硬盘信息失败，在manjaro18KDE读取USB移动硬盘也报同样的错误
```
ioctl(...,/dev/sda...) has some proble:
ERROR: HDIO_GET_IDENTITY: Invalid argument


+ HDD_FILE获取硬盘信息会崩溃，看注释代码：takethat：




>`root@Kylin:~/takethat_worksapce/portable-system-info-cpp/parted/parted-3.2# ./configure --prefix=/root/takethat_worksapce/portable-system-info-cpp/parted/parted-3.2/OutPut/  --without-readline --host=aarch64-linux-gnu`







```
# 待优化的问题
+ 如何获得硬盘使用时间?
+ 
+ 每次执行程序都会写入.json文件，是接着上次一次写，如何处理？
+ 

Current：
USBRIP：转成C++代码，并且发觉额外插入USB功能!



# INFO

[如何在LINUX下实现硬件的自动检测（上）]( https://www.ibm.com/developerworks/cn/linux/hardware/auto-detecting/part1/index.html)
[如何在LINUX下实现硬件的自动检测（下）](  https://www.ibm.com/developerworks/cn/linux/hardware/auto-detecting/part2/index.html )
 
 一般而言，在Linux下进行设备自动检测是根据设备的总线类型进行的。现在的微机系统上最常见的总线类型有PCI、SERIAL、USB、PCMCIA、PARPORT、ISA、SCSI等。对于检测过程，一般不是通过c语言的库函数直接对设备进行访问，并读取设备的信息，而是通过内核的/proc文件系统进行。

 ------------
[forensicswiki.org](https://forensicswiki.org/wiki/Main_Page)
 
Linux系统中常见的日志记录器是 Sysklogd 
 
[How to List Users in Linux](https://linuxize.com/post/how-to-list-users-in-linux/)
 
[linux系统日志以及分析](https://blog.csdn.net/dubendi/article/details/78782691 )

[Linux: HowTo Check User Password Expiration Date and Time](https://www.cyberciti.biz/faq/linux-howto-check-user-password-expiration-date-and-time/ )
[Device driver] ( https://en.wikipedia.org/wiki/Device_driver )  


[How to display or show information about a Linux Kernel module or drivers]( https://www.cyberciti.biz/tips/how-to-display-or-show-information-about-a-linux-kernel-module-or-drivers.html  )


[Linux Partition HOWTO]( http://tldp.org/HOWTO/Partition/devices.html#numbers )

[linux 命令C实现 ls pwd cd](https://www.kancloud.cn/sugar-salmon/test2/812793)
[检查系统和硬件信息的命令](https://zhuanlan.zhihu.com/p/32822864  )


[5 UNIX / Linux objdump Command Examples](https://linux.101hacks.com/unix/objdump/)

[Linux下获取CPUID、硬盘序列号与MAC地址 ](https://blog.csdn.net/eroswang/article/details/4725121)

[Howto get hardware information in Linux using C++](https://stackoverflow.com/questions/5193625/howto-get-hardware-information-in-linux-using-c)
>[如何在Linux中使用C++获取硬件信息](http://cn.voidcc.com/question/p-vmqciyrz-re.html )

[如何通过C库获取Linux硬件和系统信息？](http://cn.voidcc.com/question/p-cxynnnzk-sd.html )



+ 文件状态函数: stat


+ json

[JSON for Modern C++ ](https://github.com/nlohmann/json)
[json for modern c++的使用](https://blog.csdn.net/fengxinlinux/article/details/71037244)

+ sed：stream  editor
[三十分钟学会 SED](https://aicode.cc/san-shi-fen-zhong-xue-huised.html#toc_81)
[掘金]( https://juejin.im/entry/586360f3570c3500695501d6 )
[sed, a stream editor](https://www.gnu.org/software/sed/manual/sed.html#index-Copy-hold-space-into-pattern-space-168   )
+ 正则表达式
[正则表达式30分钟入门教程]( http://deerchao.net/tutorials/regex/regex.htm)



[读取scsi磁盘serial number](http://blog.chinaunix.net/uid-8462735-id-1641638.html )

[Hard Disk Metadata](http://blog.fpmurphy.com/2010/05/hard-disk-metadata.html)


------------------------------------------
# work


## 计算机基本信息

+ 操作系统信息
什么操作系统？
多少位？


[How To Find Out My Linux Distribution Name and Version]( https://www.cyberciti.biz/faq/find-linux-distribution-name-version-number/)

+ 硬件信息
CPU
内存
硬盘

+ 系统驱动

[How do I find out what hard disks are in the system?]( https://unix.stackexchange.com/questions/4561/how-do-i-find-out-what-hard-disks-are-in-the-system)


[Basic Steps in Forensic Analysis of Unix Systems](https://staff.washington.edu/dittrich/misc/forensics/)

ls -lat /etc/tm     
 
[IDE (Integrated Drive Electronics)](  https://searchstorage.techtarget.com/definition/IDE  )

## Preparationf for analysis


+ /etc/fstab
 the file system layout from 


+ /etc/hosts 
the host name and IP address



+  /var/log/dmesg

he devices on the system
 
/var/log/messages

system log files


## HDD
硬盘信息包括硬盘的序列号、硬盘型号、使用时间等信息，并可以发现曾经接入从盘的痕迹。分区信息包括卷标、卷名称、容量，盘符等信息，具备 识别隐藏分区功能。

接口分为:  IDE, SCSI and SATA
>IDE接口硬盘：表示为/dev/hda1、/dev/hdb …；
>SCSI(The Small Computer System Interface) 接口的硬盘、SATA接口的硬盘表示为/dev/sda、/dev/sdb … … ；

/proc/scsi


+ SCSI
[scsi_io.c](https://ftp.samba.org/pub/unpacked/ctdb/utils/scsi_io/scsi_io.c)

[sg3_utils](https://github.com/hreinecke/sg3_utils)



+ 获取硬盘序列号 /型号

1. 系统自带命令
fdisk –l  
序列号:
hdparm -i /dev/sda

2. 编程方法:获取HDD序列号/型号
```C
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>

int 
main(int argc, 
     char *argv[])
{
    static struct hd_driveid hd;
    int fd;

    if (geteuid() >  0) {
        printf("ERROR: Must be root to use\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY|O_NONBLOCK)) < 0) {
        printf("ERROR: Cannot open device %s\n", argv[1]);
        exit(1);
    }

    if (!ioctl(fd, HDIO_GET_IDENTITY, &hd)) {
        printf("Hard Disk Model: %.40s\n", hd.model);
        printf("  Serial Number: %.20s\n", hd.serial_no);
    } else if (errno == -ENOMSG) {
        printf("No hard disk identification information available\n");
    } else {
        perror("ERROR: HDIO_GET_IDENTITY");
        exit(1);
    }
        exit(0);
}
```
>但是这个获取nvme1n1p会出错(T470p电脑上报错)



+ 硬盘使用时间

1. 额外安装smartctl获取硬盘使用时间
d
[获得硬盘使用时间 smartctl --all /dev/sdc | grep Power_On_Hours](https://superuser.com/questions/686570/how-to-get-total-running-time-of-a-hard-disk)

2. 没有找到编程的方法获取硬盘时间




相关命令
```

lsblk

sfdisk -l /dev/sda
```
+ 识别隐藏分区功能:
需要额外软件testdisk可以实现

[Linux 系统结构详解](http://www.codeceo.com/article/linux-system.html)

## USB外接设备使用记录
USB设备检查，显示出本计算机曾经使用过的U盘及移动硬盘留下的痕迹，例如：U盘芯片的生产厂家，产品系列，型号及序列号等。如果在U盘列表框中输入了涉密U盘，就会用红色标示出来，同时也会统计出不同类别的记录。
  USB设备检查中，会把非常规USB设备的使用记录检查出来。例如手机、平板、数码相机机、视频设备、USB打印机等的连接记录。
 

 [USB Device Tracking Artifacts on Linux](http://forensic-proof.com/wp-content/uploads/2012/06/USB-Device-Tracking-Artifacts-on-Linux.pdf)


 [usbrip: forensics tool for tracking USB device artifacts (history of USB events) on GNU/Linux](https://firmwaresecurity.com/2019/04/28/usbrip-forensics-tool-for-tracking-usb-device-artifacts-history-of-usb-events-on-gnu-linux/)


 
Track and Log USB Device Usage/Activity with USBLogView
 + journalctl 看近期日志
>ubuntu14没有这个工具


+ 命令:列出当前系统的USB设备
lsusb



[What's the difference of dmesg output and /var/log/messages?](https://unix.stackexchange.com/questions/35851/whats-the-difference-of-dmesg-output-and-var-log-messages)


+ /var/log/messages 读应的dmesg
>manjaro没有这个文件？




# 网络上网记录


[Getting internet browsing history from shell]( https://askubuntu.com/questions/631631/getting-internet-browsing-history-from-shell )


[wiki/Google_Chrome](https://forensicswiki.org/wiki/Google_Chrome)




# 下载软件检查
+ Internet Download Manager
+ analysis of Free Download Manager for Forensic Artefacts



# 11.系统的服务
[how to list all running services in linux from command line](https://www.lostsaloon.com/technology/how-to-list-all-services-in-linux/  )





---------

# memo


16. 标密文件




----------
21880小时




# 系统依赖问题

在manjaro编译运行可以，放入到Enterprise Linux Server 7.5 (Maipo)/CentOS Linux 7 (Core) 则报错
```linux
[root@localhost portable-system-info-cpp]# ./getsystem 
./getsystem: /lib64/libstdc++.so.6: version `CXXABI_1.3.9' not found (required by ./getsystem)
./getsystem: /lib64/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by ./getsystem)
```












# log


+ 2019-10-25 FRI:
银河麒麟  编译parted3.2在执行配置文件,报错
```linux
checking for sigaction... yes
checking for getuid... yes
checking for canonicalize_file_name... (cached) yes
checking for library containing blkid_probe_get_topology... no
checking for blkid_probe_get_topology... no
checking that generated files are newer than configure... done
configure: creating ./config.status
/bin/bash: ./config.status: Permission denied
```
在root手动执行./config.status生成了makefile即可 &&在另外一个终端执行loop.sh文件


```shell
#!/bin/bash 打印时间
while true
do
kysec_set -n exectl  -v verified  ./*
done
```
>loop.sh

