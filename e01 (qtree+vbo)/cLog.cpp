#include "cLog.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

cLog* cLog::_instance = 0;

cLog* cLog::Instance()
{
	FILE *f;

    if (_instance == 0)
	{
        _instance = new cLog;
		
		f=fopen("log.txt","w");
		fclose(f);
    }
    return _instance;
}

void cLog::Msg(char *msg)
{
	FILE *f;
	char s[128];

	ZeroMemory(s,sizeof(s));

	sprintf(s,"%s\n",msg);
	f=fopen("log.txt","a+");
	fwrite(s,sizeof(char),strlen(s),f);
	fclose(f);
}
void cLog::Msg(string str)
{
	FILE *f;

	f=fopen("log.txt","a+");
	fwrite(str.c_str(),sizeof(char),strlen(str.c_str()),f);
	fclose(f);
}
void cLog::Raw(unsigned char *bytes)
{
	FILE *f;
	
	f=fopen("log.raw","w");
	fwrite((const char *)bytes,sizeof(unsigned char),strlen((const char *)bytes),f);
	fclose(f);
}