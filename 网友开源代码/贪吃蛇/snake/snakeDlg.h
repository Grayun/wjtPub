
// SnakeDlg.h : ͷ�ļ�
//

#pragma once

#include "MenuS.h"
#include "GameS.h"
#include "SettingS.h"
#include "TopS.h"
#include "AboutS.h"

// CSnakeDlg �Ի���
class CSnakeDlg : public CDialog
{
// ����
public:
	CSnakeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SNAKE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

//����
	int m_nWidth;
	int m_nHeight;
	int m_nStartLevel;
	int m_nSingleTime;

// ����
private:
	CPoint m_point;
	BOOL m_bLButtonDown;
	CRect m_rcLastUpdate;
	BOOL m_bTracking;
	BOOL m_bSetCapture;

// ʵ��
protected:
	HICON m_hIcon;
	Image *m_pImage;
	CDC m_memBackDC;
	CBitmap m_memBackBit;
	bool m_bDrawBackDC;

	int m_nType;
	CMenuS m_menu;
	CGameS m_game;
	CTopS m_top;
	CSettingS m_setting;
	CAboutS m_About;

	// ����ͼƬ
	bool ImageFromIDResource(UINT nID, CString strType, Image * & pImg);

	BOOL UpdateControl();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnMessageMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageGame(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageTop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageSetting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageAbout(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
