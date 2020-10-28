#include<iostream>
using namespace std;


int main()
{


  string a="最近一次密码修改时间\t\t\t\t\t：12月 27, 2019 ";//这里的`:`是中文的，转成hex：EFBC9A
  string b="最近一次密码修改时间\t\t\t\t\t: 12月 27, 2019 ";//这里的`:`是3A英文字符，转成hex是： 3A

 
 

const char *key=":";
const char *key_chinese="：";
 
cout<<a.find(key)<<endl;
cout<<a.find(key_chinese)<<endl;
 



    return 0;
}