// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net

#if !defined(AFX_COPTIONTREEDEMODLG_H__244E5C67_47F0_4263_844D_26B88306B113__INCLUDED_)
#define AFX_COPTIONTREEDEMODLG_H__244E5C67_47F0_4263_844D_26B88306B113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include <src\OptionTree.h>

/////////////////////////////////////////////////////////////////////////////
// CCOptionTreeDemoDlg dialog

class CCOptionTreeDemoDlg : public CDialog
{
// Construction
public:
	CCOptionTreeDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCOptionTreeDemoDlg)
	enum { IDD = IDD_COPTIONTREEDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCOptionTreeDemoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnApplyFont(LPARAM lParam);
	LRESULT WM_ApplyButton(WPARAM wParam, LPARAM lParam);
	void OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
  void OnTreeItemClicked(NMHDR* pNotifyStruct, LRESULT* plResult);
	HICON m_hIcon;
	COptionTree m_otTree;
  COptionTreeItem* _pSearchOffset;
	UINT m_uApplyFontID;
  BOOL  m_bMindAttribute;
	// Generated message map functions
	//{{AFX_MSG(CCOptionTreeDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedBtTest();
  CString _Text;
  CString _SearchText;
  afx_msg void OnBnClickedBtExpandall();
  afx_msg void OnBnClickedBtClearInfo();
  afx_msg void OnBnClickedBtSearchItem();
  afx_msg void OnBnClickedBtResetSearch();
  afx_msg void OnBnClickedCbMindAttribute();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPTIONTREEDEMODLG_H__244E5C67_47F0_4263_844D_26B88306B113__INCLUDED_)
