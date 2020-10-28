/*
https://stackoverflow.com/questions/2916675/programmatically-obtain-dns-servers-of-host

http://blog.kankanan.com/article/linux-4e0b7f167a0b83b753d6-etc-resolv.conf-4e2d768457df540d89e36790670d52a15668.html


*/


//  https://baike.baidu.com/item/%E4%BD%8D%E6%AE%B5/10979511?fromtitle=%E4%BD%8D%E5%9F%9F&fromid=9215688

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>

int main(int argc, char *argv[])
{
    struct __res_state res;
    res.options &= ~ RES_INIT;

    int err = res_ninit(&res);
    if (err) {
        fprintf(stderr, "res_init error: %d\n", err);
        return err;
    }

    char ip[16];
    for(int i = 0 ; i < res.nscount; ++i) {
        ip[0] = '\0';
        if (! inet_ntop(AF_INET, &res.nsaddr_list[i].sin_addr, ip, sizeof(ip))) {
            perror("inet_ntop");
            continue;
        }
        printf("ip: %s\n", ip);
    }

    res_nclose(&res);

    return 0;
}






