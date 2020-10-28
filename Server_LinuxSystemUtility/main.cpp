//#include "server_ws.hpp"


#include <iostream>
#include <thread>
#include "SpecialTrojanServer.h"
#include<sys/file.h>

//#include <qtextcodec.h>

using namespace std;
static int g_single_proc_inst_lock_fd = -1;
static void single_proc_inst_lockfile_cleanup(void);
bool is_single_proc_inst_running(const char *process_name);

int main(int argc, char *argv[])
{


//home/takethat/Templates/SpecialTrojanProxy_liunx_centos/SpecialTrojanProxy_liunx

    if(is_single_proc_inst_running("SpecialTrojanProxy_liunx")==false)
    {
cout<<"Only has `SpecialTrojanProxy_liunx` process in this time,StarUp Faile! "<<endl;
return -1;
    }
            //QTextCode* codec=QTextCode::codeForName("GBK");
    WsServer server;
   // server.config.port = 8889;
server.config.port = 8889;
    auto &SpecialTrojanEp = server.endpoint["^/SpecialTrojan/?$"];//这个对应client进程名字
    Init(SpecialTrojanEp);

    thread server_thread([&server]() {
        // Start WS-server
    server.start();
    cout<<__LINE__<<endl;//debug
    });
    server_thread.join();
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
