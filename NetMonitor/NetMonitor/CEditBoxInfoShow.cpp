#include "stdafx.h"
#include "CEditBoxInfoShow.h"
#include "MsgFIFO.h"
#include <atltrace.h>
#include "MyListBox.h"

CEditBoxInfoShow::CEditBoxInfoShow():CThread(false),
m_dlg(NULL)
{
	
}

CEditBoxInfoShow::~CEditBoxInfoShow()
{
	TRACE("%s, exit\r\n", this->GetThreadName().c_str());
}

int CEditBoxInfoShow::Run()
{
	string str;
	int c = 0;
	
	while(this->m_running)
	{
		if(!CFIFO::GetInstance()->isEmpty())
		{
#if 0
			c = ((CListBox*)m_dlg->GetDlgItem(m_id))->GetCount();
			if(c>10000)
				((CListBox*)m_dlg->GetDlgItem(m_id))->ResetContent();
			str = CFIFO::GetInstance()->Read();
			
			((CListBox*)m_dlg->GetDlgItem(m_id))->AddString(str.c_str());
			
			c = ((CListBox*)m_dlg->GetDlgItem(m_id))->GetCount();
			if(c<=0)c =1;
			((CListBox*)m_dlg->GetDlgItem(m_id))->SetCurSel(c-1);
			//((CListBox*)m_dlg->GetDlgItem(m_id))->del
#else
			c = ((CMyListBox*)m_dlg->GetDlgItem(m_id))->GetCount();
			if(c>1000)
				((CMyListBox*)m_dlg->GetDlgItem(m_id))->ResetContent();
			str = CFIFO::GetInstance()->Read();
			((CMyListBox*)m_dlg->GetDlgItem(m_id))->SetRedraw(FALSE);
			int line_cnt = ((CMyListBox*)m_dlg->GetDlgItem(m_id))->AddString(str.c_str());
			((CMyListBox*)m_dlg->GetDlgItem(m_id))->SetCurSel(line_cnt);
			((CMyListBox*)m_dlg->GetDlgItem(m_id))->SetRedraw(TRUE);
			
#endif
		}
		Sleep(100);
	}
	TRACE("CEditBoxInfoShow thread exit ... ...\r\n");
	return 0;
}