#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
       #include <arpa/inet.h>
       #include <sys/socket.h>
       #include <netdb.h>
       #include <ifaddrs.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <linux/if_link.h>
       #include<string.h>


#include<iostream>



using namespace std;

       string GetLocalMac();

       int main(int argc, char *argv[])
       {
           struct ifaddrs *ifaddr, *ifa;
           int family, s, n;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

           /* Walk through linked list, maintaining head pointer so we
              can free list later */

           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               /* Display interface name and family (including symbolic
                  form of the latter for the common families) */

               printf("%-8s %s (%d)\n",
                      ifa->ifa_name,
                      (family == AF_PACKET) ? "AF_PACKET" :
                      (family == AF_INET) ? "AF_INET" :
                      (family == AF_INET6) ? "AF_INET6" : "???",
                      family);

               /* For an AF_INET* interface address, display the address */

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }

                   printf("\t\taddress: <%s>\n", host);

               } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = ifa->ifa_data;

                   printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
           }


         //  cout<< GetLocalMac()<<endl;

           freeifaddrs(ifaddr);
           exit(EXIT_SUCCESS);
       }



// string GetLocalMac()  
// {  
//     int sock_mac;  
      
//     struct ifreq ifr_mac;  
//     char mac_addr[30];     
      
//     sock_mac = socket( AF_INET, SOCK_STREAM, 0 );  
//     if( sock_mac == -1)  
//     {  
//         perror("create socket falise...mac/n");  
//         return "";  
//     }  
      
//     memset(&ifr_mac,0,sizeof(ifr_mac));     
//     strncpy(ifr_mac.ifr_name, "eth0", sizeof(ifr_mac.ifr_name)-1);     
  
//     if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)  
//     {  
//         printf("mac ioctl error/n");  
//         return "";  
//     }  
      
//     sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],  
//             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);  
  
//     printf("local mac:%s /n",mac_addr);      
      
//     close( sock_mac );  
//     return string( mac_addr );  
// }  


