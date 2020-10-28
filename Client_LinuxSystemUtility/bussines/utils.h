#ifndef UTILS_H
#define UTILS_H

//#include "windows.h"
#include <sys/socket.h>
#include <unistd.h>
//#include "tlhelp32.h"

#include <QString>
#include <QTableWidget>

#define SERVER_PROCESS_NAME "SpecialTrojanProxy"
#define TABLELINEHIGHT 38

extern bool auto_roll;
#define BOOL int

bool is64BitOs();
BOOL IsWow64();
BOOL enableShutdownPrivilege();
void killProcess(const QString &proccess_name);
void reboot();
void setAutoStart();
void canCelAutoStart();
void setRowContentColor(const QString content_color, QTableWidget *table, int set_row);
void openFileDirFromFullPath(const QString &full_path);
void pushData(const QString &data);
QString popData();

BOOL DisableWow64FsRedirection(BOOL disable);

#endif // UTILS_H
