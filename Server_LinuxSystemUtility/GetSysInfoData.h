#ifndef SPECIALTROJANPROXY_LIUNX_GETSYSINFODATA_H
#define SPECIALTROJANPROXY_LIUNX_GETSYSINFODATA_H

#endif // SPECIALTROJANPROXY_LIUNX_GETSYSINFODATA_H


//#include"CAGMDLL.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include<iostream>
#include<memory>
#include<string>
#include"GetSysInfoImpl.h"

using namespace  std;
class GetSYsInfoData
       {
public:
      std::shared_ptr< GetSysInfoImpl>   Open_GetSysInfo_Dynamic();


private:
            std::shared_ptr< GetSysInfoImpl> ptr_GetSysInfo;

};


