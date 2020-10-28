#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <net/if.h>
  
struct ethtool_value 
{
 __uint32_t cmd;
 __uint32_t data;
};
  
/*return 1:has cable; return 0:no cable*/
int detect_eth_cable(char *ifname) 
{
 struct ethtool_value edata;
 struct ifreq ifr;
 int fd = -1, err = 0;
  
 memset(&ifr, 0, sizeof(ifr));
 strcpy(ifr.ifr_name, ifname);
  
 fd = socket(AF_INET, SOCK_DGRAM, 0);
 if (fd < 0) {
  //perror("Cannot get control socket");
  return -1;
  }
 edata.cmd = 0x0000000A;
 ifr.ifr_data = (caddr_t)&edata;
 err = ioctl(fd, 0x8946, &ifr);
 if (err == 0) {
  fprintf(stdout, "Link detected: %s\n", edata.data ? "yes":"no");
 } else if (errno != EOPNOTSUPP) {
  perror("Cannot get link status");
  }
 return(edata.data==1 ? 1:0);
}
int main(int argc, char**argv)
{
 detect_eth_cable("enp0s31f6");
 return 0;
}

