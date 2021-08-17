//
// Created by huhua on 2021/8/16.
//

#ifndef ZUMA_BOT_NTINFO_H
#define ZUMA_BOT_NTINFO_H
#include <WIndows.h>

typedef LONG NTSTATUS;
typedef DWORD KPRIORITY;
typedef WORD UWORD;

void* GetThreadStackTopAddress_x86(HANDLE hProcess, HANDLE hThread);
#endif //ZUMA_BOT_NTINFO_H
