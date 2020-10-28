/*
https://blog.csdn.net/mrpre/article/details/84144448

*/

#include <resolv.h>
#include<iostream>
using namespace std;
int main()
{
    int i;
    struct sockaddr_in *sin;
    /*判断是否初始化过。
     *例如进程调用过gethostname，就会在底层默认调用一次res_init。
     *此时不需要再次初始化了。
     */
    if (_res.nscount <= 0) {
        res_init();
        if (_res.nscount <= 0) {
            /*可能是/etc/resolv.conf没有配置nameserver*/
            printf("No any namserver");
            return -1;
        }
    }
    for (i = 0; i < _res.nscount; i++) {
        sin = &_res.nsaddr_list[i];
        /*每个sin 都是DNS 服务器的地址
        *对应的就是 /etc/resolv.conf 中 nameserver 指定 的ip
        */  

       cout<<"sin->sin_addr.s_addr="<<sin->sin_addr.s_addr<<endl;


    }

  //  cout<<"res_init()="<<res_init()<<endl;

    return 0;
}








