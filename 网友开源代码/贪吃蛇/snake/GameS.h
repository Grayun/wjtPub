#pragma once
#include "GameLogic.h"

#define MSG_GAME	(WM_USER + 2)

class CGameS
{
	CGameLogic m_GameLogic;
	SnakeItem m_snakeItem[SNAKE_SIZE][SNAKE_SIZE];
	SnakeItem *m_pSankeHead;
	SnakeItem *m_pSankeTail;
	int m_nDirection;
	int m_nSpeed;
	int m_nTime;
	int m_nEatX;
	int m_nEatY;
	int m_nSnakeCount;
	int m_nEatCount;
	int m_nEatTpye;
	bool m_bGameOver;
	int m_nSingleTime;
	bool m_bDrawSnake;

	CRect m_rcSnake;
	int m_nLevel;
	long m_lScore; 
	bool m_bUpdateLevel;
	bool m_bUpdateScore;

	HWND m_hWnd;
	int m_nItemHover;
	int m_nItemDown;
	int m_nState;

	CRect m_rcPause;
	CRect m_rcTop;
	CRect m_rcBotton;
	CRect m_rcLeft;
	CRect m_rcRight;

	CRect m_rcLineTop;
	CRect m_rcLineBotton;
	CRect m_rcLineLeft;
	CRect m_rcLineRight;

	CRect m_rcLevel;
	CRect m_rcScore;

	int m_nWidth;
	int m_nHeight;
	int m_nButtomSize;


	Image *m_pImage;
	CDC m_memBackDC;
	CBitmap m_memBackBit;
	bool m_bDrawBackDC;
	CDC m_memInfoDC;
	CBitmap m_memInfoBit;

	CDC m_memSnakeDC;
	CBitmap m_memSnakeBit;

	CDC m_memSnakeSrcDC;
	CBitmap m_memSnakeSrcBit;

public:
	CGameS();
	~CGameS(void);

	void SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight, int nStartLevel, int nSingleTime);
	void Draw(CDC &dc, CRect rcUpdate);
	void DrawSnake();
	int EatSanke();

	bool OnMouseMove(UINT nFlags, CPoint point);
	bool OnLButtonDown(UINT nFlags, CPoint point);
	bool OnLButtonUp(UINT nFlags, CPoint point);
	bool OnTimer(UINT_PTR nIDEvent);

	bool OnKeyDown(UINT nChar);
	bool OnKeyUp(UINT nChar);
};
