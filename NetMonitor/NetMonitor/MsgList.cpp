#include "stdafx.h"
#include "MsgList.h"

shared_ptr<MsgList> MsgList::m_pInstance = NULL;


MsgList::MsgList():m_list()
{
	::InitializeCriticalSection( &m_csLock );
}


MsgList::~MsgList()
{
	::DeleteCriticalSection( &m_csLock );
}


void MsgList::Popfront(string& val)
{
	::EnterCriticalSection( &m_csLock );
	if(m_list.size()>0)
	{
		val = m_list.front();
		m_list.pop_front();
	}
	::LeaveCriticalSection( &m_csLock );
}


void MsgList::Pushback(string val)
{
	::EnterCriticalSection( &m_csLock );
	m_list.push_back(val);
	::LeaveCriticalSection( &m_csLock );
}

int  MsgList::Size()
{
	static int ret = 0;
	::EnterCriticalSection( &m_csLock );
	ret = m_list.size();
	::LeaveCriticalSection( &m_csLock );
	return ret;
}


void MsgList::Clear()
{
	::EnterCriticalSection( &m_csLock );
	m_list.clear();
	::LeaveCriticalSection( &m_csLock );
}

