#include "StdAfx.h"

namespace HP3D
{

	//����ȫ�� SendMessage ��CAD�������� ESC
	bool ClearAcadCmdLine()
	{
		CFrameWnd* pFrame = (CFrameWnd*)(AfxGetApp()->m_pMainWnd);
		if (NULL != pFrame)
		{
			CView* pView = pFrame->GetActiveView();
			if (NULL == pView)
			{
				pView = pFrame->GetActiveFrame()->GetActiveView();
			}

			if (pView)
			{
				::SendMessage(pView->m_hWnd, WM_CHAR, VK_ESCAPE, 0);
				::SendMessage(pView->m_hWnd, WM_CHAR, VK_ESCAPE, 0);			
			}
		}

		return true;
	}

	// �ƶ����嵽 ���������Ͻ�
	void MoveWindowToViewTopLeft(CWnd* pWindow)
	{
		if (NULL == pWindow)
		{
			return;
		}
		// �������嵽�ӿ����Ͻ�
		CFrameWnd* pFrame = (CFrameWnd*)(AfxGetApp()->m_pMainWnd);
		CView* pView = pFrame->GetActiveView();
		if (NULL == pView)
		{
			pView = pFrame->GetActiveFrame()->GetActiveView();
		}

		if (NULL != pView)
		{
			CRect rc3D;
			pView->GetWindowRect(&rc3D);
			CPoint ptWin = rc3D.TopLeft();
			pFrame->ScreenToClient(&ptWin);
			pFrame->ClientToScreen(&ptWin);
			pWindow->SetWindowPos(NULL, ptWin.x, ptWin.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
		}

	}
}
