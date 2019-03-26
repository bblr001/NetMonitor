#pragma once
#include "Thread.h"

class MsgPrint: public CThread
{
public:
	MsgPrint(void);
	virtual ~MsgPrint(void);
	virtual int Run();
};