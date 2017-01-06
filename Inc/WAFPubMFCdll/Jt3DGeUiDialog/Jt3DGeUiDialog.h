#pragma once

// 三维窗体
class WAFPUBMFCDLL_API CJt3DGeUiDialog : public CDialog
{
	DECLARE_DYNAMIC(CJt3DGeUiDialog)
public:
	CJt3DGeUiDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CJt3DGeUiDialog();

	DECLARE_MESSAGE_MAP()
public:
	void     SetDialogName(LPCTSTR strDlgName);
	CString GetDialogName() const;
	BOOL  SetDialogData (LPCTSTR valueName, LPCTSTR data);
	BOOL  SetDialogData (LPCTSTR valueName, int data);
	void     GetDialogData (LPCTSTR valueName, CString& data);
	void     GetDialogData (LPCTSTR valueName, int& data);
protected:
	// -------------------------------------------------------------------------
	// Summary:
	// 		隐藏对话框，当用户有机会到图面点取数据
	// -------------------------------------------------------------------------
	virtual void    BeginEditorCommand ();

	// -------------------------------------------------------------------------
	// Summary:
	// 		恢复对话框显示
	// Parameters:
	// 		restoreDialogs - 变量为TRUE，显示对话框,为FALSE，调用对话框OnOK函数退出对话框
	// -------------------------------------------------------------------------
	virtual void    CompleteEditorCommand (BOOL restoreDialogs=TRUE);

	// -------------------------------------------------------------------------
	// Summary:
	// 		恢复隐藏状态，调用对话框OnCancel函数退出对话框
	// -------------------------------------------------------------------------
	virtual void    CancelEditorCommand ();
protected:
	CString m_strDlgName; // 用于保存和读取数据
};
