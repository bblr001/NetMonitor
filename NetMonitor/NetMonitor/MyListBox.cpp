// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
//#include "MsgTransfer.h"
#include "MyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	this->SetRedraw(FALSE);
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CMyListBox 消息处理程序




BOOL CMyListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CListBox::OnEraseBkgnd(pDC);
}


void CMyListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListBox::OnPaint()
	CRect rect;
	GetClientRect(&rect);

	CDC MemDC;	//内存DC
	CBitmap MemBmp;
	MemDC.CreateCompatibleDC(&dc);
	MemBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//这是创建的位图只是一张黑色的纸
	//先画上背景
	MemDC.FillSolidRect(rect,RGB(255,255,255));
	//再画上图形
	//这一句式调用默认的OnPaint(),把图形画在内存DC表上
	DefWindowProc(WM_PAINT, (WPARAM)MemDC.m_hDC, (LPARAM)0);

	//输出
	dc.BitBlt(0,0,rect.Width(), rect.Height(), &MemDC, 0, 0,SRCCOPY);

	MemDC.DeleteDC();
	MemBmp.DeleteObject();
}


HBRUSH CMyListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	/*HBRUSH hbrEdit = ::CreateSolidBrush(RGB(0,0,0));
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,255));
	hbr =  hbrEdit;*/
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
