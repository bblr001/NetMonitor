// MyListBox.cpp : ʵ���ļ�
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



// CMyListBox ��Ϣ�������




BOOL CMyListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CListBox::OnEraseBkgnd(pDC);
}


void CMyListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CListBox::OnPaint()
	CRect rect;
	GetClientRect(&rect);

	CDC MemDC;	//�ڴ�DC
	CBitmap MemBmp;
	MemDC.CreateCompatibleDC(&dc);
	MemBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//���Ǵ�����λͼֻ��һ�ź�ɫ��ֽ
	//�Ȼ��ϱ���
	MemDC.FillSolidRect(rect,RGB(255,255,255));
	//�ٻ���ͼ��
	//��һ��ʽ����Ĭ�ϵ�OnPaint(),��ͼ�λ����ڴ�DC����
	DefWindowProc(WM_PAINT, (WPARAM)MemDC.m_hDC, (LPARAM)0);

	//���
	dc.BitBlt(0,0,rect.Width(), rect.Height(), &MemDC, 0, 0,SRCCOPY);

	MemDC.DeleteDC();
	MemBmp.DeleteObject();
}


HBRUSH CMyListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	/*HBRUSH hbrEdit = ::CreateSolidBrush(RGB(0,0,0));
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,255));
	hbr =  hbrEdit;*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
