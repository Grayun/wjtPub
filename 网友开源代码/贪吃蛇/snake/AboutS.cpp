#include "StdAfx.h"
#include "AboutS.h"

CAboutS::CAboutS()
{	
	m_bDrawBackDC = false;	
	m_hWnd = NULL;

	m_nItemHover = -1;
	m_nItemDown = -1;
	m_nState = 0;
}

CAboutS::~CAboutS(void)
{
}

void CAboutS::SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight)
{
	m_hWnd = hWnd;
	m_pImage = pImage;

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_nButtomSize = m_nWidth * 3 / 20;
	int nButtomFrame = m_nButtomSize / 2;

	m_rcLineTop.SetRect(0, m_nButtomSize, m_nWidth, m_nButtomSize + m_nWidth * 35 / 800);
	m_rcTitle.SetRect(0, 0, m_nWidth / 2, 0 + m_nHeight * 170 / 1280);
}

void CAboutS::Draw(CDC &dc, CRect rcUpdate)
{
	if(!m_bDrawBackDC)
	{
		m_bDrawBackDC = true;

		m_memBackDC.CreateCompatibleDC(&dc);
		m_memBackBit.CreateCompatibleBitmap(&dc, m_nWidth * 3, m_nHeight);
		CBitmap *pOldBitmap = m_memBackDC.SelectObject(&m_memBackBit);

		m_memBackDC.BitBlt(0, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);
		m_memBackDC.BitBlt(m_nWidth, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);
		m_memBackDC.BitBlt(m_nWidth * 2, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);


		Graphics graphics(m_memBackDC);

		//上线
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 240, 800, 35, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left + m_nWidth, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 240, 800, 35, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left + m_nWidth * 2, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 240, 800, 35, UnitPixel);

		//标题
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 580, 400, 170, UnitPixel);

		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 580, 400, 170, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 0, 1562, 420, 170, UnitPixel);

		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth * 2, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 580, 400, 170, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth * 2, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 420, 1562, 420, 170, UnitPixel);

		//详情
		graphics.DrawImage(m_pImage, Rect(m_nWidth * 1.5 / 8, m_nHeight * 2 / 5, m_nWidth * 5 / 8, m_nHeight / 5), 700, 1732, 500, 250, UnitPixel);

	}

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &m_memBackDC, 0, 0, SRCCOPY);

	if(m_nItemHover == 0)
	{
		dc.BitBlt(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height(), 
			&m_memBackDC, m_rcTitle.left + m_nWidth * m_nState, m_rcTitle.top, SRCCOPY);
	}

}

bool CAboutS::OnMouseMove(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}
	return false;
}

bool CAboutS::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 2;
		m_nItemDown = m_nItemHover;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	return m_nItemHover != -1;
}

bool CAboutS::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(m_nItemHover != -1 && m_nItemDown == m_nItemHover)
	{
		::SendMessage(m_hWnd, MSG_ABOUT, m_nItemDown, 0);
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	m_nItemDown = -1;

	return false;
}

bool CAboutS::OnTimer(UINT_PTR nIDEvent)
{
	return false;
}
