#include <iostream>
#include <fstream>
 
#include <regex>
#include<string>


using namespace std;
int main(int argc,char * argv[])
{
  
string str="2019-12-05T10:06:21+0800 localhost.localdomain kernel: usb usb1: New USB device found, idVendor=1d6b, idProduct=0002";
 
 
  const std::regex txt_regex("(?:]|:) usb (.*?): ");  //take:这个正则表达在centos7中文环境没用
      bool isGetUSB= std::regex_search(str, txt_regex);//take:str_buffer USB记录信息
       if( isGetUSB )
        {  
                cout<<__LINE__<<endl;
        }

        return 0;
}

