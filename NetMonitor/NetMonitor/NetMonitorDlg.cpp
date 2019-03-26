
// NetMonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetMonitor.h"
#include "NetMonitorDlg.h"
#include "afxdialogex.h"
#include "MsgFIFO.h"
#include "Cfg.h"
#include "Log.h"
#include "utility.h"
#include <stdio.h>
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetMonitorDlg �Ի���




CNetMonitorDlg::CNetMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_COMMAND(ID_32771, &CNetMonitorDlg::OnGetEtherNetList)
	ON_WM_DESTROY()
	ON_COMMAND(ID_32772, &CNetMonitorDlg::On32772)
	ON_COMMAND(ID_32773, &CNetMonitorDlg::On32773)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32774, &CNetMonitorDlg::On32774)
	ON_UPDATE_COMMAND_UI(ID_32775, &CNetMonitorDlg::OnUpdate32775)
	ON_UPDATE_COMMAND_UI(ID_32774, &CNetMonitorDlg::OnUpdate32774)
END_MESSAGE_MAP()


// CNetMonitorDlg ��Ϣ�������
int CNetMonitorDlg::PcapRun()
{
	int slidx =  Ccfg::GetInstance()->GetEtherNetID();
	netwrp.SetSelectedDevId(slidx);
	string exp = Ccfg::GetInstance()->GetFilterExpress();
	netwrp.SetFilterExpress(exp);
	int ret = netwrp.FilterLoop();
	return ret;
}

BOOL CNetMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CMenu menu;
	menu.LoadMenuA(IDR_MENU3);
	SetMenu(&menu);
	//CMenu* menu2 = GetMenu()->GetSubMenu(2); //��ò˵���ĵڶ����Ӳ˵�
	//menu2->CheckMenuRadioItem(ID_32774, ID_32775, ID_32775, MF_BYCOMMAND);  //���õ�ѡ״̬�����������������
	int n = menu.GetMenuItemCount();
	//CMenu* menu2 = menu.GetSubMenu(2);
	/*
	char diskPartition = 'D';
	CString directory = "NetMonitorLog";
	CLog::GetInstance()->SetLogFileInfo(diskPartition, directory);
	*/
	CString batpath = "";
	char szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strFileName = szPath;
	strFileName.Delete(strFileName.ReverseFind('\\')+1, strFileName.GetLength ()-strFileName.ReverseFind('\\')-1);
	batpath = strFileName;
	strFileName += "config.ini";
	Ccfg::GetInstance()->Load(strFileName.GetString());
	CString AppName = NULL;
	//new CString(szPath) + Ccfg::GetInstance()->GetFilterExpress();
	AppName.Format("%s ->%s", szPath, Ccfg::GetInstance()->GetFilterExpress());
	SetWindowText(AppName);

	string logpath = Ccfg::GetInstance()->GetLogPath();
	vector<string> tmppath;
	Split(logpath,tmppath, ":\\/");
	string dir = "";	
	for(auto it = tmppath.begin(); it != tmppath.end(); it++)
	{
		string p = *it;
		if(it == tmppath.begin())
		{
			dir += p + ":/";
		}
		else
		{
			dir += p + "/";
		}
		
		if(it!= tmppath.begin()){
			int r = _mkdir(dir.c_str());
			printf("%d\r\n", r);
		}else{
			
		}
	}
	CLog::GetInstance()->SetLogFileInfo(dir);
	CLog::GetInstance()->WriteLog(strFileName, logDay);

	m_editbox = make_shared<CEditBoxInfoShow>();
	m_editbox->SetThreadName("m_listbox");
	m_editbox->SetParent(this);
	m_editbox->SetCtrlID(IDC_LIST_MSG);
	m_editbox->SetRelease(true);

	CString cstr = "�����س������� ... ... \r\n";
	CFIFO::GetInstance()->Write(cstr.GetString());
	CLog::GetInstance()->WriteLog(cstr, logDay);
	
	int ret = netwrp.initial();
	if(ret<0)
	{
		cstr = "pCap��ʼ��ʧ��\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		CLog::GetInstance()->WriteLog(cstr, logDay);
		return TRUE;
	}
	else
	{
		int mode = Ccfg::GetInstance()->GetRunningMode();
		if(mode == 0)
		{
			cstr = "��ǰΪ�ֶ�ģʽ��������ץ������\r\n";
			CFIFO::GetInstance()->Write(cstr.GetString());
			CLog::GetInstance()->WriteLog(cstr, logDay);
			return TRUE;
		}
		else
		{		
			cstr = "��ǰΪ�Զ�ģʽ��ץ�����ܳɹ�\r\n";
			CFIFO::GetInstance()->Write(cstr.GetString());
			CLog::GetInstance()->WriteLog(cstr, logDay);
			_beginthread(Monitor, 0 ,this);
			msprint.SetRelease(true);
		}
	}
	int m = Ccfg::GetInstance()->GetCharMode();
	cstr.Format("CharMode = %d\r\n", m);
	CFIFO::GetInstance()->Write(cstr.GetString());
	CLog::GetInstance()->WriteLog(cstr, logDay);
	netwrp.SetCharMode(m);
	//cstr.Format("ret = %d");
	//
#if 0
	
#endif
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNetMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

 void CNetMonitorDlg::Monitor(LPVOID param)
 {
	CNetMonitorDlg* dlg = (CNetMonitorDlg*)param;
	int ret = dlg->PcapRun();

 }
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNetMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNetMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNetMonitorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	msprint.SetRunning(false);
	m_editbox->SetRunning(false);
}


void CNetMonitorDlg::On32772()
{
	// TODO: �ڴ���������������
	vector<string> devlist = netwrp.GetDevList();
	for(auto it = devlist.begin(); it != devlist.end(); it++)
	{
		CFIFO::GetInstance()->Write(*it);
	}
}


void CNetMonitorDlg::On32773()
{
	// TODO: �ڴ���������������
}


void CNetMonitorDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(IDOK == MessageBox("Sure to close?", "tips", MB_OKCANCEL))
		CDialogEx::OnClose();
	else
		return;
	//CDialogEx::OnClose();
}


void CNetMonitorDlg::On32774()
{
	// TODO: �ڴ���������������
	;
}


void CNetMonitorDlg::OnUpdate32775(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(true);
}


void CNetMonitorDlg::OnUpdate32774(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(true);
}
