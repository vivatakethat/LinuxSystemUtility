// std::regex是C++11的东西,如果是老的GCC版本要加 std=c++11选项

#include <iostream>
#include <fstream>
 
#include <regex>
#include<string>


using namespace std;
int main(int argc,char * argv[])
{
  
string str="2019-12-05T10:06:21+0800 localhost.localdomain kernel: usb usb1: New USB device found, idVendor=1d6b, idProduct=0002";
 cout<<"str.length()="<<str.length()<<endl;


 // const regex txt_regex("(?:]|:) usb (.*?): ");  //take:这个正则表达在centos7中文环境没用

 const char * c_regex="(?:]|:) usb (.*?): ";
 cout<<"c_regex="<<c_regex<<endl;
const regex txt_regex(c_regex);

cout<<"flag="<<txt_regex.flags()<<endl;

 // cout<<"txt_regex="<<txt
      bool isGetUSB= std::regex_search(str, txt_regex);//take:str_buffer USB记录信息
       if( isGetUSB )
        {  
                cout<<__LINE__<<endl;
        }

        return 0;

}


