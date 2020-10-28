#include<iostream>
#include<vector>
using namespace std;

std::wstring GBKToUnicode( const std::string& src );
 bool hasChinese(const std::string& src);
int main()
{


  string a="最近一次密码修改时间\t\t\t\t\t：12月 27, 2019 ";//这里的`:`是中文的，转成hex：EFBC9A
  string b="最近一次密码修改时间\t\t\t\t\t: 12月 27, 2019 ";//这里的`:`是3A英文字符，转成hex是： 3A

 
 

const char *key=":";
const char *key_chinese="：";
 
cout<<a.find(key)<<endl;
cout<<a.find(key_chinese)<<endl;
 

cout<<hasChinese(a)<<endl;
cout<<hasChinese(b)<<endl;
    return 0;
}









 
/*convert mbcs to unicode string*/
 std::wstring GBKToUnicode( const std::string& src ) 
{ 
    setlocale( LC_ALL, "chs" );// 设置为中文环境，不然可能会转换失败
    std::vector<wchar_t> dst( src.size() + 1, L'\0' ); 
    size_t count = std::mbstowcs( &dst[0], src.c_str(), dst.size() - 1 );
    setlocale( LC_ALL, "C" ); 
    return std::wstring( &dst[0] );
 }
 bool hasChinese(const std::string& src)
 {
     auto w = GBKToUnicode(src);
     for(auto c:w) 
     { if(c >= 0x4E00 && c <= 0x9FCB 
         || c>= 0x3400 && c <= 0x4DB5 )// 自己再添加中文编码范围
          return true;
     }
     return false;
 }