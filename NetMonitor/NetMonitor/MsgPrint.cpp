#include "stdafx.h"
#include "MsgPrint.h"
#include "MsgList.h"
#include "MsgFIFO.h"
#include "Log.h"

MsgPrint::MsgPrint()
{
	m_release = false;
}

MsgPrint::~MsgPrint()
{

}

int MsgPrint::Run()
{
	string str = "";
	str = "进入消息打印\n";
	CFIFO::GetInstance()->Write(str);
	CLog::GetInstance()->WriteLog(str.c_str(), logDay);
	while(this->m_running)
	{
		if(MsgList::GetInstance()->Size()>0)
		{
			MsgList::GetInstance()->Popfront(str);
			CFIFO::GetInstance()->Write(str);
			CLog::GetInstance()->WriteLog(str.c_str(), logDay);
			Sleep(20);
		}
		Sleep(100);
	}
	return 0;
}