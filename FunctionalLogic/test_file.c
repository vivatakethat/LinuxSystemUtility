#include<stdio.h>
#include <stdio.h>
#include<error.h>
#include<errno.h>
int main()
{
	FILE * m_pFile; 


    		//打开文件
 m_pFile= fopen("test_file.json", "a+b");

if(m_pFile==NULL)
{

	fprintf(stderr,"Failed to oopen, reason:%s\n",strerror(errno));
}
fwrite("this is is text", 1, strlen("this is is text"), m_pFile);

return 0;

}