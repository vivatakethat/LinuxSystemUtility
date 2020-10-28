#include<iostream>
using namespace std;



//检查联网状态
int CheckIsNetWorking()
{
    //创建curl对象
    CURL *curl;
    //创建curlcode对象
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        //设置一个请求文件地址
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.xxxx.tv/release/remote.update");
        //执行请求操作,将返回值交给res
        res = curl_easy_perform(curl);
        //res=0的情况表示接收到信息,反之则没有收到信息
        if(res!=0)
        {
             //没有联网
             return false;
        }
        else
        {
             //已经联网
             return true;

        }
        /* 释放资源 */
        curl_easy_cleanup(curl);
    }
    return false;
}



int main()
{

            CheckIsNetWorking();

    return 0;
}