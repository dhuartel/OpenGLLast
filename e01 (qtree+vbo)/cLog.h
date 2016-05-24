
#pragma once

#include <string>
using namespace std;

class cLog
{
public: 
	
    static cLog* Instance();
	static void Msg(char *msg);
	static void Msg(string str);
	static void Raw(unsigned char *bytes);

private:
    static cLog* _instance;
};