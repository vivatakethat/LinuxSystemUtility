/*

g++ -ggdb take_network.cpp hw.o main.o print.o mem.o dmi.o device-tree.o cpuinfo.o osutils.o pci.o version.o cpuid.o ide.o cdrom.o pcmcia-legacy.o scsi.o s390.o disk.o spd.o network.o isapnp.o pnp.o fb.o options.o usb.o sysfs.o display.o heuristics.o parisc.o cpufreq.o partitions.o blockio.o lvm.o ideraid.o pcmcia.o volumes.o mounts.o smp.o abi.o jedec.o dump.o fat.o virtio.o vio.o ../lshw.o -lresolv    -o take




*/


/*

g++ -c take_network.cpp  -o take_network.o


g++ take_network.o  ../lshw.o -L. -llshw -lresolv -o static


*/
#include"mounts.h"
#include "options.h"
#include<stdio.h>
#include "network.h"
#include "hw.h"
#include"disk.h"
#include"main.h"
      #include <unistd.h>
      #include"partitions.h"
 
      #include "print.h"
  //    #include<set>
 
// #include <vector>
// #include <string>
// #include <map>

// using namespace std;
// static set < string > disabled_tests;
// string lowercase(const string & s)
// {
//   string result;

//   for (unsigned int i = 0; i < s.length(); i++)
//     result += tolower(s[i]);

//   return result;
// }

// bool disabled(const char *option)
// {
//   return disabled_tests.find(lowercase(option)) != disabled_tests.end();
// }
// bool enabled(const char *option)
// {
//   return !(disabled(lowercase(option).c_str()));
// }

 
int main(int argc,char **argv)
{
 
printf("----\n");
   hwNode computer("computer",
      hw::system);

    scan_system(computer);



  if (!parse_options(argc, argv))
  {
   printf("error\n");
    exit(1);
  }


 disable("output:html");
        print(computer, enabled("output:html"));
    return 0;
}