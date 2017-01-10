#include "StdAfx.h"
#include "GameS.h"

CGameS::CGameS()
{	
	m_bDrawBackDC = false;	
	m_hWnd = NULL;

	m_nLevel = 1;
	m_lScore = 0;
	m_bUpdateLevel = true;
	m_bUpdateScore = true;

	m_nItemHover = -1;
	m_nItemDown = -1;
	m_nState = 0;

	ZeroMemory(m_snakeItem, sizeof(m_snakeItem));
	m_pSankeHead = NULL;
	m_pSankeTail = NULL;
	m_nDirection = DIRECTION_R;
	m_nSpeed = 2;
	m_nTime = 0;
	m_nEatX = INVALID_ITEM;
	m_nEatY = INVALID_ITEM;
	m_nSnakeCount = 0;
	m_nEatCount = 0;
	m_nEatTpye = 0;
	m_bGameOver = false;
	m_nSingleTime = 75;
	m_bDrawSnake = false;
}

CGameS::~CGameS(void)
{
}

void CGameS::SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight, int nStartLevel, int nSingleTime)
{
	m_hWnd = hWnd;
	m_pImage = pImage;

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_nButtomSize = m_nWidth * 3 / 20;
	int nButtomFrame = m_nButtomSize / 2;

	m_rcPause.SetRect(0, 0, 0 + m_nButtomSize, 0 + m_nButtomSize);
	m_rcTop.SetRect(nButtomFrame + m_nButtomSize, nHeight - m_nButtomSize * 3, nButtomFrame + m_nButtomSize + m_nButtomSize, nHeight - m_nButtomSize * 3 + m_nButtomSize);
	m_rcBotton.SetRect(nButtomFrame + m_nButtomSize, nHeight - m_nButtomSize, nButtomFrame + m_nButtomSize + m_nButtomSize, nHeight);
	m_rcLeft.SetRect(nButtomFrame + 0, nHeight - m_nButtomSize * 2, nButtomFrame + 0 + m_nButtomSize, nHeight - m_nButtomSize * 2 + m_nButtomSize);
	m_rcRight.SetRect(m_nWidth - m_nButtomSize - nButtomFrame, nHeight - m_nButtomSize * 2, m_nWidth - nButtomFrame, nHeight - m_nButtomSize * 2 + m_nButtomSize);

	m_rcLineTop.SetRect(0, m_nButtomSize, m_nWidth, m_nButtomSize + m_nWidth * 35 / 800);
	m_rcLineBotton.SetRect(0, m_nButtomSize + m_nWidth - m_nWidth * 30 / 800, m_nWidth, m_nButtomSize + m_nWidth);
	m_rcLineLeft.SetRect(0, m_nButtomSize, 0 + m_nWidth * 30 / 800, m_nButtomSize + m_nWidth);
	m_rcLineRight.SetRect(m_nWidth -  m_nWidth * 32 / 800, m_nButtomSize, m_nWidth, m_nButtomSize + m_nWidth);

	m_rcLevel = m_rcPause;
	m_rcLevel.OffsetRect(m_nButtomSize + nButtomFrame, 0);
	m_rcScore = m_rcLevel;
	m_rcScore.right = m_nWidth;
	m_rcScore.left = m_rcLevel.right;

	int nSnakeItem = m_nWidth / SNAKE_SIZE;
	int nSankeSize = SNAKE_SIZE * nSnakeItem;
	int nSnakeLeft = (nWidth -  nSankeSize) / 2;
	int nSnakeTop = m_rcLineTop.top + (m_rcLineBotton.bottom - m_rcLineTop.top - nSankeSize) / 2;

	m_rcSnake.SetRect(nSnakeLeft, nSnakeTop, nSnakeLeft + nSankeSize, nSnakeTop + nSankeSize);

	m_nLevel = nStartLevel;
	m_nSingleTime = nSingleTime;
	m_nSpeed = 11 - m_nLevel;
	m_lScore = 0;
	m_bUpdateLevel = true;
	m_bUpdateScore = true;

	m_nItemHover = -1;
	m_nItemDown = -1;
	m_nState = 0;

	ZeroMemory(m_snakeItem, sizeof(m_snakeItem));
	m_pSankeHead = NULL;
	m_pSankeTail = NULL;
	m_nDirection = DIRECTION_R;

	ZeroMemory(m_snakeItem, sizeof(m_snakeItem));
	m_nSnakeCount = m_GameLogic.InitSnake(m_snakeItem, m_pSankeHead, m_pSankeTail, nSnakeItem);
	m_GameLogic.RandEat(m_snakeItem, m_nEatX, m_nEatY);
	m_bGameOver = false;
	m_bDrawSnake = false;
}

void CGameS::DrawSnake()
{
	int nSnakeItem = m_nWidth / SNAKE_SIZE;

	if(!m_bDrawSnake)
	{
		m_bDrawSnake = true;
	
 		m_memSnakeDC.BitBlt(0, 0, m_rcSnake.Width(), m_rcSnake.Height(), &m_memBackDC, m_rcSnake.left, m_rcSnake.top, SRCCOPY);
	 
 		for (int i = 0; i < SNAKE_SIZE; i++)
 		{
 			for (int j = 0; j < SNAKE_SIZE; j++)
 			{
 				if(m_snakeItem[i][j].bShow)
 				{
 					m_memSnakeDC.BitBlt(i * nSnakeItem, j * nSnakeItem, nSnakeItem, nSnakeItem, &m_memSnakeSrcDC,
 						m_GameLogic.GetImagePosX(m_snakeItem[i][j]), m_GameLogic.GetImagePosY(m_snakeItem[i][j]), SRCCOPY);
 				}
 			}
 		}
	}
	else
	{
		if(m_pSankeTail->pNextItem)
		{
			m_memSnakeDC.BitBlt(m_pSankeTail->pNextItem->nItemX * nSnakeItem, m_pSankeTail->pNextItem->nItemY * nSnakeItem, nSnakeItem, nSnakeItem, &m_memBackDC, 
				m_rcSnake.left + m_pSankeTail->pNextItem->nItemX * nSnakeItem, m_rcSnake.top + m_pSankeTail->pNextItem->nItemY * nSnakeItem, SRCCOPY);
		}

		m_memSnakeDC.BitBlt(m_pSankeHead->nItemX * nSnakeItem, m_pSankeHead->nItemY * nSnakeItem, nSnakeItem, nSnakeItem, &m_memSnakeSrcDC, 
			m_GameLogic.GetImagePosX(*m_pSankeHead), m_GameLogic.GetImagePosY(*m_pSankeHead), SRCCOPY);
		m_memSnakeDC.BitBlt(m_pSankeHead->pNextItem->nItemX * nSnakeItem, m_pSankeHead->pNextItem->nItemY * nSnakeItem, nSnakeItem, nSnakeItem, &m_memSnakeSrcDC, 
			m_GameLogic.GetImagePosX(*m_pSankeHead->pNextItem), m_GameLogic.GetImagePosY(*m_pSankeHead->pNextItem), SRCCOPY);

		m_memSnakeDC.BitBlt(m_pSankeTail->nItemX * nSnakeItem, m_pSankeTail->nItemY * nSnakeItem, nSnakeItem, nSnakeItem, &m_memSnakeSrcDC, 
			m_GameLogic.GetImagePosX(*m_pSankeTail), m_GameLogic.GetImagePosY(*m_pSankeTail), SRCCOPY);		
	}	

	if(m_nEatX != INVALID_ITEM && m_nEatY != INVALID_ITEM && !m_bGameOver)
	{
		m_memSnakeDC.BitBlt(m_nEatX * nSnakeItem, m_nEatY * nSnakeItem, nSnakeItem, nSnakeItem, &m_memSnakeSrcDC,
			10 * nSnakeItem, m_nEatTpye * nSnakeItem, SRCCOPY);
	}
}

void CGameS::Draw(CDC &dc, CRect rcUpdate)
{
	if(!m_bDrawBackDC)
	{
		m_bDrawBackDC = true;

		m_memBackDC.CreateCompatibleDC(&dc);
		m_memBackBit.CreateCompatibleBitmap(&dc, m_nWidth * 2, m_nHeight);
		m_memBackDC.SelectObject(&m_memBackBit);

		m_memBackDC.BitBlt(0, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);
		m_memBackDC.BitBlt(m_nWidth, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);


		//蛇
		int nSnakeItem = m_nWidth / SNAKE_SIZE;
		m_memSnakeSrcDC.CreateCompatibleDC(&dc);
		m_memSnakeSrcBit.CreateCompatibleBitmap(&dc, nSnakeItem * 11, nSnakeItem * 2);
		m_memSnakeSrcDC.SelectObject(&m_memSnakeSrcBit);

		for(int i = 0; i < 10; i++)
		{
			m_memSnakeSrcDC.BitBlt(i * nSnakeItem, 0, nSnakeItem, nSnakeItem * 2, &dc, 0, 0, SRCCOPY);
		}

		//苹果
		m_memSnakeSrcDC.BitBlt(10 * nSnakeItem, 0, nSnakeItem, nSnakeItem * 2, &dc, 0, 0, SRCCOPY);

		Graphics graphics0(m_memSnakeSrcDC);

		graphics0.DrawImage(m_pImage, Rect(0, 0, nSnakeItem * 10, nSnakeItem * 2), 0, 0, 1200, 240, UnitPixel);
		graphics0.DrawImage(m_pImage, Rect(10 * nSnakeItem, 0, nSnakeItem, nSnakeItem * 2), 900, 1085, 120, 240, UnitPixel);



		Graphics graphics(m_memBackDC);

		//上
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 240, 800, 35, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left + m_nWidth, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 240, 800, 35, UnitPixel);

		//下
		graphics.DrawImage(m_pImage, Rect(m_rcLineBotton.left, m_rcLineBotton.top, m_rcLineBotton.Width(), m_rcLineBotton.Height()), 0, 275, 800, 30, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineBotton.left + m_nWidth, m_rcLineBotton.top, m_rcLineBotton.Width(), m_rcLineBotton.Height()), 0, 275, 800, 30, UnitPixel);

		//左
		graphics.DrawImage(m_pImage, Rect(m_rcLineLeft.left, m_rcLineLeft.top, m_rcLineLeft.Width(), m_rcLineLeft.Height()), 1138, 750, 30, 800, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineLeft.left + m_nWidth, m_rcLineLeft.top, m_rcLineLeft.Width(), m_rcLineLeft.Height()), 1138, 750, 30, 800, UnitPixel);

		//右
		graphics.DrawImage(m_pImage, Rect(m_rcLineRight.left, m_rcLineRight.top, m_rcLineRight.Width(), m_rcLineRight.Height()), 1168, 750, 32, 800, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineRight.left + m_nWidth, m_rcLineRight.top, m_rcLineRight.Width(), m_rcLineRight.Height()), 1168, 750, 32, 800, UnitPixel);

		//暂停
		graphics.DrawImage(m_pImage, Rect(m_rcPause.left, m_rcPause.top, m_rcPause.Width(), m_rcPause.Height()), 820, 750, 120, 120, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcPause.left + m_nWidth, m_rcPause.top, m_rcPause.Width(), m_rcPause.Height()), 940, 750, 120, 120, UnitPixel);
		
		//上
		graphics.DrawImage(m_pImage, Rect(m_rcTop.left, m_rcTop.top, m_rcTop.Width(), m_rcTop.Height()), 420, 1085, 120, 120, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcTop.left + m_nWidth, m_rcTop.top, m_rcTop.Width(), m_rcTop.Height()), 420, 1205, 120, 120, UnitPixel);

		//下
		graphics.DrawImage(m_pImage, Rect(m_rcBotton.left, m_rcBotton.top, m_rcBotton.Width(), m_rcBotton.Height()), 660, 1085, 120, 120, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcBotton.left + m_nWidth, m_rcBotton.top, m_rcBotton.Width(), m_rcBotton.Height()), 660, 1205, 120, 120, UnitPixel);

		//左
		graphics.DrawImage(m_pImage, Rect(m_rcLeft.left, m_rcLeft.top, m_rcLeft.Width(), m_rcLeft.Height()), 540, 1085, 120, 120, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLeft.left + m_nWidth, m_rcLeft.top, m_rcLeft.Width(), m_rcLeft.Height()), 540, 1205, 120, 120, UnitPixel);

		//右
		graphics.DrawImage(m_pImage, Rect(m_rcRight.left, m_rcRight.top, m_rcRight.Width(), m_rcRight.Height()), 780, 1085, 120, 120, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcRight.left + m_nWidth, m_rcRight.top, m_rcRight.Width(), m_rcRight.Height()), 780, 1205, 120, 120, UnitPixel);

		m_memInfoDC.CreateCompatibleDC(&dc);
		m_memInfoBit.CreateCompatibleBitmap(&dc, m_nButtomSize * 5, m_nButtomSize * 10);
		m_memInfoDC.SelectObject(&m_memInfoBit);

		Graphics graphics1(m_memInfoDC);

		int nNumberWidth = m_nButtomSize / 2;
		for (int i = -1; i < 9; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				//级别
				if(i == -1)
				{
					m_memInfoDC.BitBlt(0, (j + 1) * m_nButtomSize, m_rcLevel.Width(), m_rcLevel.Height(), &m_memBackDC, m_rcLevel.left, m_rcLevel.top, SRCCOPY);
					graphics1.DrawImage(m_pImage, Rect(0, (j + 1) * m_nButtomSize, m_rcLevel.Width(), m_rcLevel.Height()), j * 120, 1325, 120, 120, UnitPixel);
				}
 				else
 				{
 					//数字
 					m_memInfoDC.BitBlt(m_nButtomSize + i * nNumberWidth, j * m_nButtomSize, nNumberWidth, m_nButtomSize, &m_memBackDC, m_nWidth - (i + 1) * nNumberWidth, m_rcScore.top, SRCCOPY);
 					graphics1.DrawImage(m_pImage, Rect(m_nButtomSize + i * nNumberWidth, j * m_nButtomSize, nNumberWidth, m_nButtomSize), j * 60, 1445, 60, 120, UnitPixel);
 				}				
			}
		}

		//蛇
		m_memSnakeDC.CreateCompatibleDC(&dc);
		m_memSnakeBit.CreateCompatibleBitmap(&dc, m_rcSnake.Width(), m_rcSnake.Height());
		m_memSnakeDC.SelectObject(&m_memSnakeBit);

		m_memSnakeDC.BitBlt(0, 0, m_rcSnake.Width(), m_rcSnake.Height(), &m_memBackDC, m_rcSnake.left, m_rcSnake.top, SRCCOPY);

		//m_memSnakeDC.FillSolidRect(0, 0, m_rcSnake.Width(), m_rcSnake.Height(), RGB(128, 128, 128));

		DrawSnake();
	}

	if(m_bUpdateLevel)
	{
		m_bUpdateLevel = false;
		
		m_memBackDC.BitBlt(m_rcLevel.left, m_rcLevel.top, m_rcLevel.Width(), m_rcLevel.Height(), &m_memInfoDC, 0, m_nLevel * m_nButtomSize, SRCCOPY);
	}

	if(m_bUpdateScore)
	{
		m_memBackDC.BitBlt(m_rcScore.left, m_rcScore.top, m_rcScore.Width(), m_rcScore.Height(), &dc, m_rcScore.left, m_rcScore.top, SRCCOPY);
		m_bUpdateScore = false;
		long lScore = m_lScore;
		int nNumberWidth = m_nButtomSize / 2;
		int nIndex = 1;
		do 
		{			
			m_memBackDC.BitBlt(m_nWidth - nNumberWidth * nIndex, m_rcScore.top, m_nButtomSize / 2, m_rcScore.Height(), &m_memInfoDC, m_nButtomSize + nNumberWidth * nIndex , (lScore % 10) * m_nButtomSize, SRCCOPY);
			lScore /= 10;
			nIndex++;
		} while (lScore != 0);		
	}

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &m_memBackDC, 0, 0, SRCCOPY);

	//蛇
	DrawSnake();
	dc.BitBlt(m_rcSnake.left, m_rcSnake.top, m_rcSnake.Width(), m_rcSnake.Height(), &m_memSnakeDC, 0, 0, SRCCOPY);

	if(m_nItemHover == 0)
	{
		dc.BitBlt(m_rcPause.left, m_rcPause.top, m_rcPause.Width(), m_rcPause.Height(), 
			&m_memBackDC, m_rcPause.left + m_nWidth * m_nState, m_rcPause.top, SRCCOPY);
	}
	else if(m_nItemHover == 1)
	{
		dc.BitBlt(m_rcTop.left, m_rcTop.top, m_rcTop.Width(), m_rcTop.Height(), 
			&m_memBackDC, m_rcTop.left + m_nWidth * m_nState, m_rcTop.top, SRCCOPY);
	}
	else if(m_nItemHover == 2)
	{
		dc.BitBlt(m_rcBotton.left, m_rcBotton.top, m_rcBotton.Width(), m_rcBotton.Height(), 
			&m_memBackDC, m_rcBotton.left + m_nWidth * m_nState, m_rcBotton.top, SRCCOPY);
	}
	else if(m_nItemHover == 3)
	{
		dc.BitBlt(m_rcLeft.left, m_rcLeft.top, m_rcLeft.Width(), m_rcLeft.Height(), 
			&m_memBackDC, m_rcLeft.left + m_nWidth * m_nState, m_rcLeft.top, SRCCOPY);
	}
	else if(m_nItemHover == 4)
	{
		dc.BitBlt(m_rcRight.left, m_rcRight.top, m_rcRight.Width(), m_rcRight.Height(), 
			&m_memBackDC, m_rcRight.left + m_nWidth * m_nState, m_rcRight.top, SRCCOPY);
	}
}

bool CGameS::OnMouseMove(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcPause.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 1;
	}
	else if(m_rcTop.PtInRect(point))
	{
		m_nItemHover = 1;
		m_nState = 1;
	}
	else if(m_rcBotton.PtInRect(point))
	{
		m_nItemHover = 2;
		m_nState = 1;
	}
	else if(m_rcLeft.PtInRect(point))
	{
		m_nItemHover = 3;
		m_nState = 1;
	}
	else if(m_rcRight.PtInRect(point))
	{
		m_nItemHover = 4;
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

bool CGameS::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcPause.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nItemDown = 0;
		m_nState = 1;
		::SendMessage(m_hWnd, MSG_GAME, m_nItemHover, 0);
	}
	else if(m_rcTop.PtInRect(point))
	{
		m_nItemHover = 1;
		m_nItemDown = 1;
		m_nState = 1;
		::SendMessage(m_hWnd, MSG_GAME, VK_UP, 0);
	}
	else if(m_rcBotton.PtInRect(point))
	{
		m_nItemHover = 2;
		m_nItemDown = 2;
		m_nState = 1;
		::SendMessage(m_hWnd, MSG_GAME, VK_DOWN, 0);
	}
	else if(m_rcLeft.PtInRect(point))
	{
		m_nItemHover = 3;
		m_nItemDown = 3;
		m_nState = 1;
		::SendMessage(m_hWnd, MSG_GAME, VK_LEFT, 0);
	}
	else if(m_rcRight.PtInRect(point))
	{
		m_nItemHover = 4;
		m_nItemDown = 4;
		m_nState = 1;
		::SendMessage(m_hWnd, MSG_GAME, VK_RIGHT, 0);
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

bool CGameS::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcPause.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 1;
	}
	else if(m_rcTop.PtInRect(point))
	{
		m_nItemHover = 1;
		m_nState = 1;
	}
	else if(m_rcBotton.PtInRect(point))
	{
		m_nItemHover = 2;
		m_nState = 1;
	}
	else if(m_rcLeft.PtInRect(point))
	{
		m_nItemHover = 3;
		m_nState = 1;
	}
	else if(m_rcRight.PtInRect(point))
	{
		m_nItemHover = 4;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(m_nItemHover == 0 && m_nItemDown == m_nItemHover)
	{
		::SendMessage(m_hWnd, MSG_GAME, m_nItemDown, 0);
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	m_nItemDown = -1;

	return false;
}

bool CGameS::OnTimer(UINT_PTR nIDEvent)
{	
	if(++m_nTime >= m_nSpeed)
	{
		m_nTime = 0;
		if (!m_bGameOver)
		{
			if(0 == m_GameLogic.MoveSnake(m_snakeItem, m_pSankeHead, m_pSankeTail, m_nDirection, m_nEatCount == 0))
			{
				DrawSnake();
				EatSanke();
			}		
			else
			{
				m_bGameOver = true;
			}
		}
		
	}
	::InvalidateRect(m_hWnd, NULL, true);
	return false;
}

bool CGameS::OnKeyDown(UINT nChar)
{
	bool bMove = false;
	if(VK_UP == nChar)
	{
		if(m_nDirection != DIRECTION_D)
		{
			m_nDirection = DIRECTION_U;
			bMove = true;			
		}
	//	m_nItemHover = 1;
	//	m_nItemDown = 1;
	//	m_nState = 1;
	}
	else if(VK_DOWN == nChar)
	{
		if(m_nDirection != DIRECTION_U)
		{
			m_nDirection = DIRECTION_D;
			bMove = true;			
		}
	//	m_nItemHover = 2;
	//	m_nItemDown = 2;
	//	m_nState = 1;
	}
	else if(VK_LEFT == nChar)
	{
		if(m_nDirection != DIRECTION_R)
		{
			m_nDirection = DIRECTION_L;
			bMove = true;			
		}
		//m_nItemHover = 3;
		//m_nItemDown = 3;
	//	m_nState = 1;
	}
	else if(VK_RIGHT == nChar)
	{
		if(m_nDirection != DIRECTION_L)
		{
			m_nDirection = DIRECTION_R;
			bMove = true;
		}
		
		//m_nItemHover = 4;
		//m_nItemDown = 4;
		//m_nState = 1;
	}

	if(bMove && !m_bGameOver)
	{
		KillTimer(m_hWnd, 1);
		m_nTime = 0;
		SetTimer(m_hWnd, 1, m_nSingleTime, NULL);
		if(0 == m_GameLogic.MoveSnake(m_snakeItem, m_pSankeHead, m_pSankeTail, m_nDirection, m_nEatCount == 0 && !m_bGameOver))
		{
			m_bGameOver = false;
			DrawSnake();
			EatSanke();
			::InvalidateRect(m_hWnd, NULL, true);
			return true;
		}		
		else
		{
			m_bGameOver = true;
		}		
	}

	return false;
}

bool CGameS::OnKeyUp(UINT nChar)
{
	//m_nItemHover = -1;
	//m_nState = 0;
	//::InvalidateRect(m_hWnd, NULL, true);
	return false;
}

int CGameS::EatSanke()
{
	m_nEatCount = 0;
	if(m_pSankeHead->nItemX == m_nEatX && m_pSankeHead->nItemY == m_nEatY)
	{
		m_nSnakeCount++;
		m_nEatCount = 1;
		m_nEatTpye = rand() % 2;
		m_GameLogic.RandEat(m_snakeItem, m_nEatX, m_nEatY);

		m_lScore++;
		m_bUpdateScore = true;

		if(m_lScore % 10 == 0)
		{	
			if(m_nLevel < 9)
			{
				m_bUpdateLevel = true;
				m_nLevel++;
				m_nSpeed = 11 - m_nLevel;
			}
		}
	}

	return m_nSnakeCount;
}