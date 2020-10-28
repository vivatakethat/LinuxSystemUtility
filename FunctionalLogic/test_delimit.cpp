

#include<iostream>
#include<string>
using namespace std;

 bool  recyRemveCR(string &str);  //配合while循环删除str中的回车
int main()
{

string a="aab\nb\ncc\n";
 

 while(!recyRemveCR(a));

 cout<<a<<endl;

    return 0;
}



 bool  recyRemveCR(string &str)
{

size_t n_delimit=str.find('\n');

if(n_delimit!=string::npos)
{
        string temp=str.substr(0,n_delimit);
        size_t n_end=str.size();
        string temp2=str.substr(n_delimit+1,n_end);//
        temp+=temp2;
        str=temp;

        return false;
}
else
{
    return true;
}


} 