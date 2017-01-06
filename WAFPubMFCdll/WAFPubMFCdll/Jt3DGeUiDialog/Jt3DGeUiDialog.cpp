#include "StdAfx.h"


IMPLEMENT_DYNAMIC(CJt3DGeUiDialog, CDialog)
CJt3DGeUiDialog::CJt3DGeUiDialog(UINT nIDTemplate, CWnd* pParentWnd/* = NULL*/)
:CDialog(nIDTemplate, pParentWnd), m_strDlgName(_T(""))
{

}
CJt3DGeUiDialog::~CJt3DGeUiDialog()
{
	
}


BEGIN_MESSAGE_MAP(CJt3DGeUiDialog, CDialog)

END_MESSAGE_MAP()

void    CJt3DGeUiDialog::BeginEditorCommand()
{
	CWnd* pWnd = GetParent();
	if (NULL != pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	ShowWindow(SW_HIDE);
	if (NULL != pWnd)
	{
		pWnd->SetFocus();
	}
}

void    CJt3DGeUiDialog::CompleteEditorCommand (BOOL restoreDialogs)
{
	if (restoreDialogs)
	{
		this->ShowWindow(SW_SHOW);
		SetFocus();
		CWnd* pWnd = GetParent();
		if (NULL != pWnd)
		{
			pWnd->EnableWindow(FALSE);
		}
		EnableWindow(TRUE);
	}
	else
	{
		this->OnOK();
	}
}

void    CJt3DGeUiDialog::CancelEditorCommand ()
{
	CWnd* pWnd = GetParent();
	if (NULL != pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	this->OnCancel();
	if (NULL != pWnd)
	{
		pWnd->SetFocus();
	}

}

void  CJt3DGeUiDialog::SetDialogName(LPCTSTR strDlgName)
{
	m_strDlgName = strDlgName;
}

CString CJt3DGeUiDialog::GetDialogName() const
{
	if (m_strDlgName.IsEmpty())
	{
		return this->GetRuntimeClass()->m_lpszClassName;
	}
	return m_strDlgName;
}

BOOL  CJt3DGeUiDialog::SetDialogData (LPCTSTR valueName, LPCTSTR data)
{
	return AfxGetApp()->WriteProfileString(GetDialogName(),valueName,data);
}

BOOL  CJt3DGeUiDialog::SetDialogData (LPCTSTR valueName, int data)
{
	return AfxGetApp()->WriteProfileInt(GetDialogName(),valueName,data);
}

void  CJt3DGeUiDialog::GetDialogData (LPCTSTR valueName, CString& data)
{
	data = AfxGetApp()->GetProfileString(GetDialogName(),valueName,data);
}

void  CJt3DGeUiDialog::GetDialogData (LPCTSTR valueName, int& data)
{
	data = AfxGetApp()->GetProfileInt(GetDialogName(),valueName,data);
}
