#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct st_str
{
    string a="null";
    string b="null";

};

int main()
{

std::vector<st_str>  vec_str;//vec_str[2];

st_str one;
one.a="11111aaaaaa";
one.b="11111111bbbbbb";

st_str two;
two.a="22222aaaaaa";
two.b="222222bbbbbb";


vec_str.push_back(two);
        vec_str.push_back(one);

  cout<<vec_str[1].a<<endl;
//   vec_str[0].b="0000bbbb";
//    vec_str[1].a="111111aaaa";
//    vec_str[1].b="111111bbbbb";
// for(std::vector<st_str>::iterator it=vec_str.begin();it!=vec_str.end();it++)
// {
//                 cout<<it->a<<endl;
// }
return 0;
}