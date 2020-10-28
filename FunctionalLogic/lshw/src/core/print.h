#ifndef _PRINT_H_
#define _PRINT_H_

#include "hw.h"
#include "../../../PublicData.h"


std::vector<st_HardWare>  print(hwNode & node, bool html=true, int level = 0);

bool desotry();
void printhwpath(hwNode & node);
void printbusinfo(hwNode & node);

void status(const char *);
#endif
