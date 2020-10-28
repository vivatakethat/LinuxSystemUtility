

#include<iostream>

#include<string>
#include<regex>

using namespace std;

int main()
{


    string test="2019-10-08T10:45:54+0800 t470p kernel: usb 1-3.4.2: USB disconnect, device number 9";
  if(std::string::npos!=test.find("disconnect"))
  {
    cout<<"find disconnect!\n"<<endl;
  }
 
 
  return 0;

}