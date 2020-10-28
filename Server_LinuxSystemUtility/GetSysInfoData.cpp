#include"GetSysInfoData.h"


std::shared_ptr< GetSysInfoImpl>  GetSYsInfoData::Open_GetSysInfo_Dynamic()
{
typedef std::shared_ptr<void>(*LPDLLCREATEFUNC)(void);
    LPDLLCREATEFUNC lpFunc;
    int iRet = 0;
    std::shared_ptr<void> lpObject;

    void* hLib = dlopen("./libGetsystemInfo.so", RTLD_LAZY);
        if (NULL == hLib) {
            printf("dlopen error is[%s]\n", dlerror());
        //	iRet = UNKNOW_ERROR;
        }
        else {
            int iRet = 0;
            char * perror;
            lpFunc = (LPDLLCREATEFUNC)dlsym(hLib, "CreateInstance");  //take:����ʵ��
            perror = dlerror();
            if (NULL == perror) {
                iRet = 0;
                lpObject = lpFunc();
            }
            else {
            //	iRet = UNKNOW_ERROR;
                printf("dlsym error :%s\n", perror);
                return  NULL;
            }
                std::shared_ptr<void> take = lpObject;



                std::shared_ptr< GetSysInfoImpl> m_GetSysInfo = std::static_pointer_cast< GetSysInfoImpl>(lpObject);


                if(m_GetSysInfo!=NULL)
                {
                    return     m_GetSysInfo;
                  //      cout<<__LINE__<<endl;
      //      vector<st_sysAccount> result_Account=m_GetSysInfo->getSysAccountSecurityCheck();
             //               cout<<__LINE__<<endl;
//            for(int i=0;i< result_Account.size();i++)
//            {
//                cout<<"Username:"<< result_Account[i].UserName<<endl;
//            }
            cout<<__LINE__<<endl;
                }
                else
                {
                    cout<<"m_GetSysInfo is NULL!!!!!"<<endl;
                }
}

}
