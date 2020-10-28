//#include "windows.h"

#include <sys/socket.h>
#include <unistd.h>

#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QProcess>
#include <QFileInfo>
#include <QLibrary>
#include<sys/file.h>
#include "mainwindows.h"
#include "utils.h"
#include<stdio.h>
#include "SysemAccountForms.h"
#include "tipmessagebox.h"
#define PATH_MAX 4096
static int g_single_proc_inst_lock_fd = -1;
typedef bool (*Fun)();
string GetStdoutFromCommand(string cmd,int& count_Line);
bool is_single_proc_inst_running(const char *process_name);
static void single_proc_inst_lockfile_cleanup(void);
string pwdProcess();
//bool verifyKey()
//{
//    bool ret = false;

//    try {
//        QString dllPath = QApplication::applicationDirPath() + "./UsbKey.dll";
//        QLibrary mylib(dllPath);
//        char *str1 = NULL;
//        str1 = (char *)malloc(1024);
//        memset(str1, 0, 1024);
//        if (mylib.load())
//        {
//            Fun open=(Fun)mylib.resolve("InitKeyOn");
//            ret = open();
//        }
//    } catch(...) {

//    }

//    return ret;
//}

int main(int argc, char *argv[])
{
    //SecurityCheckSystem
   if( is_single_proc_inst_running("SecurityCheckSystem")==false)
   {
       cout<<"StartUp faile,Only has one `SecurityCheckSystem` in this OS!"<<endl;
       return -1;

   }


string clientPath=pwdProcess();
 cout<<"clientPath="<<clientPath<<endl;
string cd_CMD="cd "+clientPath+" && cd securitychecksystem && ./SpecialTrojanProxy_liunx > /dev/null 2>&1 &";

cout<<"cd_CMD="<<cd_CMD<<endl;

int t_a;
system(cd_CMD.c_str());
//cout<<GetStdoutFromCommand(cd_CMD,t_a);





   int t_demo=0;

// string do_recoginition=clientPath+"recoginition/recoginition &";
string do_recoginition="cd "+clientPath+" && cd recoginition && ./recoginition > /dev/null 2>&1 &";
system(do_recoginition.c_str());
int t_b;
//cout<<GetStdoutFromCommand(do_recoginition,t_b);

cout<<"do_recoginition="<<do_recoginition<<endl;
// GetStdoutFromCommand(do_recoginition,t_demo);
string currPath=pwdProcess();
if(currPath!=clientPath)
{
    string cdCMD;
cdCMD+="cd "+clientPath;
int t_CMD;
system(cdCMD.c_str());
//GetStdoutFromCommand(cdCMD,t_CMD);

}
//string do_SpecialTrojanProxyliunx=clientPath+"securitychecksystem/SpecialTrojanProxy_liunx &";
//system(do_SpecialTrojanProxyliunx.c_str());
 //GetStdoutFromCommand(do_SpecialTrojanProxyliunx,t_demo);

//    HANDLE hMutex = CreateMutex(NULL, true, QString("Global\\{902780AA-DBA6-4478-A512-5D6B086440C4}").toStdWString().c_str());
//    if (GetLastError() == ERROR_ALREADY_EXISTS)
//    {
//        CloseHandle(hMutex);
//        return 0;
//    }

    QApplication a(argc, argv);

  // QTextCodec *codec = QTextCodec::codecForName("utf8");
  //  QTextCodec::setCodecForLocale(codec);

    QFile file(":/qss/style.css");
    if (file.open(QFile::Text | QFile::ReadOnly)) {
        a.setStyleSheet(file.readAll().constData());
        file.close();
    }

    a.setFont(QFont("Microsoft Yahei"));
  //  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

//    if (!verifyKey()) {
//        qDebug() << "start show message box!";
//        TipMessageBox msg;
//        msg.SetTipInfo("不存在用户锁，请插入用户锁！");
//        msg.activateWindow();
//        msg.raise();
//        msg.exec();
//        qDebug() << "no find key!";
//        return 0;
//    }

    MainWindows w;
    w.activateWindow();
    w.raise();
    w.show();


//    QString path;
//    if (IsWow64()) {
//        path = QString("%1/%2X64.exe").arg(QApplication::applicationDirPath()).arg(SERVER_PROCESS_NAME);
//    } else {
//        path = QString("%1/%2.exe").arg(QApplication::applicationDirPath()).arg(SERVER_PROCESS_NAME);
//    }

//    qDebug() << "path:" << path;
//    QFileInfo file_info(path);
//    if (file_info.isFile()) {
//        QProcess process;
//        process.startDetached(path);
//    }

//    setAutoStart();

    return a.exec();
}




static void single_proc_inst_lockfile_cleanup(void)
{
    if (g_single_proc_inst_lock_fd != -1) {
        close(g_single_proc_inst_lock_fd);
        g_single_proc_inst_lock_fd = -1;
    }
}

bool is_single_proc_inst_running(const char *process_name)
{
    char lock_file[128];
    snprintf(lock_file, sizeof(lock_file), "/var/tmp/%s.lock", process_name);

 //   g_single_proc_inst_lock_fd = open(lock_file, O_CREAT|O_RDWR, 0644);
     g_single_proc_inst_lock_fd = open(lock_file, O_CREAT|O_RDWR|O_CLOEXEC, 0644);   //take_add:在fork进程前清理fd(O_CLOEXEC标记在linu2.6.23才开始支持此标记)
    if (-1 == g_single_proc_inst_lock_fd) {
        fprintf(stderr, "Fail to open lock file(%s). Error: %s\n",
                lock_file, strerror(errno));
        return false;
    }

    if (0 == flock(g_single_proc_inst_lock_fd, LOCK_EX | LOCK_NB)) {
        atexit(single_proc_inst_lockfile_cleanup);

//        int flags=fcntl(g_single_proc_inst_lock_fd,F_GETFD);
//flags|=FD_CLOEXEC;
//fcntl(g_single_proc_inst_lock_fd,F_SETFD,flags);
        return true;
    }

    close(g_single_proc_inst_lock_fd);
    g_single_proc_inst_lock_fd = -1;
    return false;

}


string GetStdoutFromCommand(string cmd,int& count_Line) {

count_Line=0;
  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  //cmd.append(" 2>dev/null \" ");


 //cout<<"cmd.c_str()="<<cmd.c_str()<<endl; getchar();
  stream = popen(cmd.c_str(), "r");

  if (stream== NULL)
{
 printf("Error command  Failed: %s  line=%d\n  popen() is faile!",strerror(errno),__LINE__);
        data="\npopen is Faile\n";

}
else
{
        data="\npopen is susses~\n";

 pclose(stream);

  }


    return data;


 }


string pwdProcess()
 {
     char dir[PATH_MAX] = {0};
         int n = readlink("/proc/self/exe", dir, PATH_MAX);
         //printf("PATH_MAX: %d\n", PATH_MAX);
         //printf("readlink return: %d\n", n);
     //	printf("dir: %s\n", dir);

      for(;n>0;n--)
      {
          if(dir[n]=='/')
          {
              n++;
              break;
          }
      }
         char resultDir[PATH_MAX] = {0};
     strncpy(resultDir, dir, n);

string str_dir(resultDir,resultDir+strlen(resultDir));
//cout<<"str_dir="<<str_dir<<endl;
     return str_dir;

 }
