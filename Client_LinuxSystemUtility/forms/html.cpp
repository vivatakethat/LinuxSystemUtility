#include<html.h>
#include<iostream>
#include<fstream>

void setHtmlHeadString(std::string filename,std::string inspected_unit,std::string inspected_machine,std::string inspection_time,std::string inspection_name){
    std::ofstream fileW(filename);
    if (fileW.is_open())
    {
    fileW<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
    fileW<<"<HTML><HEAD><TITLE>10.122.0.55(djy11096)涉密文件搜索结果</TITLE>";
    fileW<<"<META http-equiv=Content-Type content=\"text/html; charset=utf-8\">";
    fileW<<"<STYLE type=text/css>TD {FONT-SIZE: 9pt; LINE-HEIGHT: 200%}";
    fileW<<".dw {BACKGROUND-IMAGE: url(images/d5.gif); BACKGROUND-REPEAT: no-repeat; TEXT-ALIGN: left}";
    fileW<<".ztitle {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; mso-style-parent: style0}";
    fileW<<".z1 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ebebeb; BORDER-BOTTOM: windowtext 0.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}";
    fileW<<".z2 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}";
    fileW<<".z3 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #c8c840; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}";
    fileW<<"</STYLE>";
    fileW<<"<META content=\"MSHTML 6.00.2600.0\" name=GENERATOR></HEAD>";
    fileW<<"<BODY bgColor=#000000 leftMargin=0 topMargin=0 marginheight=\"0\" marginwidth=\"0\">";
    fileW<<"<TABLE cellSpacing=0 cellPadding=0 width=\"90%\" align=center border=0>";
    fileW<<"<TBODY>";
    fileW<<"<TR>";
    fileW<<"<TD>";
    fileW<<"<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>";
    fileW<<"<TBODY>";
    fileW<<"<TR>";
    fileW<<"<TD class=ztitle align=left bgColor=#a86a2d><FONTcolor=#ffffff><STRONG>&nbsp;&nbsp;&nbsp;</STRONG></FONT><STRONG><FONT size=2>";
    fileW<<"涉密文件搜索结果: </FONT></STRONG>";
    fileW<<"</TD></TR></TBODY></TABLE><BR>";
    fileW<<  "<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>";
    fileW<<     "<TBODY>";
    fileW<<   "<TR>";
    fileW<<    "<TD class=dw><FONT color=#ffffff><STRONG>&nbsp;&nbsp;&nbsp;本次检查信息：</STRONG></FONT> ";
    fileW<<   "</TD></TR></TBODY></TABLE>";
    fileW<<    "<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>";
    fileW<<  "<TBODY>";
    fileW<<  "<TR>";
    fileW<<  "<TD class=z1 align=middle bgColor=#464646>";
    fileW<<  "<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>";
    fileW<<  "<TBODY>";
    fileW<< "<TR>";

    std::string unit = "<TD align=middle width=\"30%\"><FONT color=#ffffff>" + inspected_unit + "</FONT></TD>";
    std::string machine = "<TD align=middle width=\"30%\"><FONT color=#ffffff>" + inspected_machine + "</FONT></TD></TR>";
    std::string time = "<TD align=middle width=\"30%\"><FONT color=#ffffff>" + inspection_time + "</FONT></TD>";
    std::string name = "<TD align=middle width=\"30%\"><FONT color=#ffffff>" + inspection_name + "</FONT></TD></TR></TBODY></TABLE><TBODY>";
    fileW<< "<TD align=middle width=\"20%\"><FONT color=#fff000>被检查单位：</FONT></TD>";
    fileW<<  unit;
    fileW<<"<TD align=middle width=\"20%\"><FONT color=#fff000>被检查主机：</FONT></TD>";
    fileW<<  machine;
    fileW<<  "<TR><TD align=middle width=\"20%\"><FONT color=#fff000>检查时间：</FONT></TD>";
    fileW<<  time;
    fileW<< "<TD align=middle width=\"20%\"><FONT color=#fff000>检查人：</FONT></TD>";
    fileW<<  name;
    fileW<<"<TR><TD class=z1 width=\"25%\">";
    fileW<<  "</TD></TR>";
    fileW<< "<TD class=z1 align=middle bgColor=#464646>";
    fileW<< "<TABLE border=1 height=1 cellSpacing=0 cellPadding=0 width=\"100%\">";
    fileW<< "<TR bgcolor=#fefff7><TD class=dw width=\"20%\"><FONT color=Black><STRONG><center>搜索关键词: </STRONG></FONT></TD>";
    fileW<< "<TD class=dw><FONT color=blue width=\"80%\"><center>";

}
    fileW.close();
}

void setSecretFileDetails(std::string filename,std::string highsec,std::string commonsec ,std::string lowsec,std::string numberoffile){
    std::ofstream fileW(filename,std::ios::app);
    if (fileW.is_open())
    {
    std::string number = "<br><bgcolor=#d5daed><FONT color=RED  SIZE=2pt >共找到 " + numberoffile + " 个文件,其中:<br>";
    std::string all = "未经人工核查  涉密概率较高:" +highsec + ",涉密概率一般:" + commonsec + ",涉密概率较低:" + lowsec + ",<br>";
    fileW<<number;
    fileW<<all;
    fileW<<"</body></html>";
    }
    fileW.close();
}
void setSearchDirString(std::string filename ,std::string searchdir){
    std::ofstream fileW(filename,std::ios::app);
    if (fileW.is_open())
    {
    fileW<< "</TD>";
    fileW<<  "<TR bgcolor=#d5daed><<TD class=dw width=\"20%\"><FONT color=Black><STRONG><center>搜索目录: </STRONG>";

    std::string dir = "<TD class=dw><FONT color=Black width=\"80%\"><STRONG><center>" + searchdir + "</STRONG></TD</TR>";
    fileW<<   dir;

    fileW<<   "</TBODY>";
    fileW<<  "</TABLE>";

    fileW<< "<FONT face=宋体 size=4>";
    fileW<<"<TABLE border=1 height=1 width=100% cellspacing=0 cellpadding=0 bordercolordark=#ffffff bordercolorlight=#666666>";
    fileW<<"<TR bgcolor=#d5daed><TD align=middle width=\"5%\">编号</TD><TD align=middle width=\"65%\">文件名称</TD><TD align=middle width=\"8%\">文件大小</TD><TD align=middle width=\"12%\">修改时间</TD><TD align=middle width=\"10%\">涉密概率</TD>";


    }
    fileW.close();
}

void setSecretFileContent(int secID,std::string filename,std::string filesize,std::string modifytime,int secretpossibility,std::string filecontent,std::string file_path){
    int nNumber = 0;
   std::ofstream fileW(filename,std::ios::app);
    if (fileW.is_open())
   {
    std::string id = std::to_string(secID);
    std::string font_color;
    if(secretpossibility>70){
        if(secretpossibility>80)
            font_color = "#FFC0CB";
         else
            font_color = "yellow";
    }
    else
        font_color = "#fefff7";


    std::string all2;
    std::string all1 = "<TR bgcolor=" + font_color  + "><TD><center>" + id + "</TD><TD><font color=Red><a href=\"" + file_path + "\" target=banner>" + file_path + "</TD><TD><left>" + filesize +"</TD><TD><left>" + modifytime + "</TD><TD><center>" + std::to_string(secretpossibility) +"%</TD></TR>";

    fileW<<all1;
     int nIndex =  filecontent.find("^^");
     while(nIndex != -1){
         std::string s1 = filecontent.substr(0,nIndex);
         filecontent = filecontent.substr(nIndex+2);
         if(nNumber==0){
             all2 = "<TR bgcolor=" + font_color  + "> <TD COLSPAN=5><font color=Black><center>" + s1 + "<br>";
         }
         else{
            all2 = "<center>" + s1 + "<br>";
         }
         nNumber++;
         fileW<<all2;
         nIndex = filecontent.find("^^");
     }

     if(nNumber ==0){
         all2 = "<TR bgcolor=" + font_color  + "> <TD COLSPAN=5><font color=Black><center>" + filecontent + "<br>";
         fileW<<all2;

     }



//    fileW<<"<TR bgcolor=#FFC0CB> <TD COLSPAN=5><font color=Black><center>，抓好本单位保密工作。3引用文件《中华人民共和国保守国家<font color=RED>秘密<font color=Black>法》《中华人民共和国保守国家秘密法实施办法》《公司保密管理制 <br>";
//    fileW<<"<center>秘密的标志由密级、“★”（标识）、保密期限三部分组成（例如“<font color=RED>机密<font color=Black>★１０年”）。一般标于文件或资料的首页显眼位置，公文的秘密等级， <br>";
//    fileW<<"<center>明保密期限的，按基本保密期限认可：秘密级十年，机密级二十年，<font color=RED>绝密<font color=Black>级三十年。5.2.5公司产生的国家秘密的定密、密级变更和解密 <br>";
//    fileW<<"<center>物等。5.2.3公司内部产生的国家秘密的具体内容界定按照《<font color=RED>公司秘密<font color=Black>事项界定》执行。5.2.4国家秘密的密级标志与保密期限5.2.4 <br>";
//    fileW<<"<center>定范围内的人员知悉的事项。5.3.2公司秘密包括商业秘密和<font color=RED>工作秘密<font color=Black>。（1）商业秘密是指公司在经营活动中产生的、不为公众知悉、能对公司 <br>";
    fileW<<"</center></TD></TR>";
   }
   fileW.close();
}
void setSearchKeywordString(std::string filename,std::string search_keyword){
    std::ofstream fileW(filename,std::ios::app);
    if (fileW.is_open())
    {
        std::string keyword = search_keyword + "<br>";
        fileW<<keyword;
    }
       fileW.close();
}
