
// NetMonitorDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNetMonitorDlg 对话框




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


// CNetMonitorDlg 消息处理程序
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

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CMenu menu;
	menu.LoadMenuA(IDR_MENU3);
	SetMenu(&menu);
	//CMenu* menu2 = GetMenu()->GetSubMenu(2); //获得菜单项的第二个子菜单
	//menu2->CheckMenuRadioItem(ID_32774, ID_32775, ID_32775, MF_BYCOMMAND);  //设置单选状态，函数具体详解如下
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

	CString cstr = "网络监控程序启动 ... ... \r\n";
	CFIFO::GetInstance()->Write(cstr.GetString());
	CLog::GetInstance()->WriteLog(cstr, logDay);
	
	int ret = netwrp.initial();
	if(ret<0)
	{
		cstr = "pCap初始化失败\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		CLog::GetInstance()->WriteLog(cstr, logDay);
		return TRUE;
	}
	else
	{
		int mode = Ccfg::GetInstance()->GetRunningMode();
		if(mode == 0)
		{
			cstr = "当前为手动模式，不启动抓包功能\r\n";
			CFIFO::GetInstance()->Write(cstr.GetString());
			CLog::GetInstance()->WriteLog(cstr, logDay);
			return TRUE;
		}
		else
		{		
			cstr = "当前为自动模式，抓包功能成功\r\n";
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNetMonitorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	msprint.SetRunning(false);
	m_editbox->SetRunning(false);
}


void CNetMonitorDlg::On32772()
{
	// TODO: 在此添加命令处理程序代码
	vector<string> devlist = netwrp.GetDevList();
	for(auto it = devlist.begin(); it != devlist.end(); it++)
	{
		CFIFO::GetInstance()->Write(*it);
	}
}


void CNetMonitorDlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
}


void CNetMonitorDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDOK == MessageBox("Sure to close?", "tips", MB_OKCANCEL))
		CDialogEx::OnClose();
	else
		return;
	//CDialogEx::OnClose();
}


void CNetMonitorDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	;
}


void CNetMonitorDlg::OnUpdate32775(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(true);
}


void CNetMonitorDlg::OnUpdate32774(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(true);
}
