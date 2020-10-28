#ifndef HTML_H
#define HTML_H
#include<stdio.h>
#include<string>
void setHtmlHeadString(std::string filename,std::string inspected_unit,std::string inspected_machine,std::string inspection_time,std::string inspection_name);
void setSearchKeywordString(std::string file_name,std::string search_keyword);
void setSearchDirString(std::string filename,std::string searchdir);
void setSecretFileDetails(std::string filename,std::string highsec,std::string commonsec ,std::string lowsec,std::string numberoffile);
void setSecretFileContent(int secID,std::string filename,std::string filesize,std::string modifytime,int secretpossibility,std::string filecontent,std::string file_path);
#endif // HTML_H
