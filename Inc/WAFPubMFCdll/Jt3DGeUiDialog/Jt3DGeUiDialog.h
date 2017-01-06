#pragma once

// ��ά����
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
	// 		���ضԻ��򣬵��û��л��ᵽͼ���ȡ����
	// -------------------------------------------------------------------------
	virtual void    BeginEditorCommand ();

	// -------------------------------------------------------------------------
	// Summary:
	// 		�ָ��Ի�����ʾ
	// Parameters:
	// 		restoreDialogs - ����ΪTRUE����ʾ�Ի���,ΪFALSE�����öԻ���OnOK�����˳��Ի���
	// -------------------------------------------------------------------------
	virtual void    CompleteEditorCommand (BOOL restoreDialogs=TRUE);

	// -------------------------------------------------------------------------
	// Summary:
	// 		�ָ�����״̬�����öԻ���OnCancel�����˳��Ի���
	// -------------------------------------------------------------------------
	virtual void    CancelEditorCommand ();
protected:
	CString m_strDlgName; // ���ڱ���Ͷ�ȡ����
};
