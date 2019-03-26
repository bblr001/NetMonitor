
// NetMonitorDlg.h : ͷ�ļ�
//

#pragma once
#include "pcap.h"
//#include "netapp.h"
#include "CEditBoxInfoShow.h"
#include <memory>
#include "NetMonitorWrapper.h"
#include "MsgPrint.h"

// CNetMonitorDlg �Ի���
class CNetMonitorDlg : public CDialogEx
{
// ����
public:
	CNetMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NETMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
