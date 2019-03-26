
// NetMonitorDlg.h : 头文件
//

#pragma once
#include "pcap.h"
//#include "netapp.h"
#include "CEditBoxInfoShow.h"
#include <memory>
#include "NetMonitorWrapper.h"
#include "MsgPrint.h"

// CNetMonitorDlg 对话框
class CNetMonitorDlg : public CDialogEx
{
// 构造
public:
	CNetMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetEtherNetList();
	static void Monitor(LPVOID param);
	int PcapRun();
private:
	shared_ptr<CEditBoxInfoShow> m_editbox;
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fcode;

	NetMonitorWrapper netwrp;
	MsgPrint msprint;
public:
	afx_msg void OnDestroy();
	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void OnClose();
	afx_msg void On32774();
	afx_msg void OnUpdate32775(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32774(CCmdUI *pCmdUI);
};
