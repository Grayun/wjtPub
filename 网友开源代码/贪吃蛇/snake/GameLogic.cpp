#include "StdAfx.h"
#include "GameLogic.h"

CGameLogic::CGameLogic(void)
{
}

CGameLogic::~CGameLogic(void)
{
}

int CGameLogic::GetImagePosX(SnakeItem &snakeItem)
{
	if(snakeItem.nSnakeType >= HEAD_U && snakeItem.nSnakeType <= BODY_UR)
	{
		switch(snakeItem.nSnakeType)
		{
		case HEAD_U:
			{
				return 3 * m_nSnakeWidth;
			}
		case HEAD_D:
			{
				return 1 * m_nSnakeWidth;	
			}
		case HEAD_L:
			{
				return 2 * m_nSnakeWidth;	
			}
		case HEAD_R:
			{
				return 0 * m_nSnakeWidth;	
			}
		case TAIL_U	:
			{
				return 9 * m_nSnakeWidth;	
			}
		case TAIL_D	:
			{
				return 9 * m_nSnakeWidth;	
			}
		case TAIL_L	:
			{
				return 8 * m_nSnakeWidth;	
			}
		case TAIL_R	:
			{
				return 8 * m_nSnakeWidth;	
			}
		case BODY_U:
			{
				return 7 * m_nSnakeWidth;	
			}
		case BODY_D:
			{
				return 5 * m_nSnakeWidth;	
			}
		case BODY_L:
			{
				return 6 * m_nSnakeWidth;
			}
		case BODY_R:
			{
				return 4 * m_nSnakeWidth;	
			}
		case BODY_RD:
			{
				return 0 * m_nSnakeWidth;	
			}
		case BODY_RU:
			{
				return 4 * m_nSnakeWidth;	
			}
		case BODY_LD:
			{
				return 6 * m_nSnakeWidth;	
			}
		case BODY_LU:
			{
				return 2 * m_nSnakeWidth;	
			}
		case BODY_DL:
			{
				return 1 * m_nSnakeWidth;	
			}
		case BODY_DR:
			{
				return 5 * m_nSnakeWidth;	
			}
		case BODY_UL:
			{
				return 7 * m_nSnakeWidth;
			}
		case BODY_UR:
			{
				return 3 * m_nSnakeWidth;	
			}
		}
	}
	return 0;
}

int CGameLogic::GetImagePosY(SnakeItem &snakeItem)
{
	if(snakeItem.nSnakeType >= HEAD_U && snakeItem.nSnakeType <= BODY_UR)
	{
		switch(snakeItem.nSnakeType)
		{
		case HEAD_U:
			{
				return 0;
			}
		case HEAD_D:
			{
				return 0;
			}
		case HEAD_L:
			{
				return 0;
			}
		case HEAD_R:
			{
				return 0;
			}
		case TAIL_U	:
			{
				return 0;
			}
		case TAIL_D	:
			{
				return m_nSnakeWidth;
			}
		case TAIL_L	:
			{
				return 0;
			}
		case TAIL_R	:
			{
				return m_nSnakeWidth;
			}
		case BODY_U:
			{
				return 0;
			}
		case BODY_D:
			{
				return 0;
			}
		case BODY_L:
			{
				return 0;
			}
		case BODY_R:
			{
				return 0;
			}
		case BODY_RD:
			{
				return m_nSnakeWidth;
			}
		case BODY_RU:
			{
				return m_nSnakeWidth;
			}
		case BODY_LD:
			{
				return m_nSnakeWidth;
			}
		case BODY_LU:
			{
				return m_nSnakeWidth;
			}
		case BODY_DL:
			{
				return m_nSnakeWidth;
			}
		case BODY_DR:
			{
				return m_nSnakeWidth;
			}
		case BODY_UL:
			{
				return m_nSnakeWidth;
			}
		case BODY_UR:
			{
				return m_nSnakeWidth;
			}
		}
	}
	return 0;
}

int CGameLogic::InitSnake(SnakeItem snakeItem[][SNAKE_SIZE], SnakeItem *&pSankeHead, SnakeItem *&pSankeTail, int nSnakeWidth)
{
	m_nSnakeWidth = nSnakeWidth;
	int nTailX = (SNAKE_SIZE - 4) / 2;
	int nTailY = (SNAKE_SIZE - 1) / 2;
	int nHeadY = nTailY;
	int nHeadX = nTailX + 3;

	pSankeHead = &snakeItem[nHeadX][nHeadY];
	pSankeTail = &snakeItem[nTailX][nTailY];

	snakeItem[nHeadX][nHeadY].bShow = true;
	snakeItem[nHeadX][nHeadY].nSnakeType = HEAD_R;
	snakeItem[nHeadX][nHeadY].pNextItem = &snakeItem[nHeadX - 1][nHeadY];
	snakeItem[nHeadX][nHeadY].pFrontItem = NULL;
	snakeItem[nHeadX][nHeadY].nItemX = nHeadX;
	snakeItem[nHeadX][nHeadY].nItemY = nHeadY;

	snakeItem[nHeadX - 1][nHeadY].bShow = true;
	snakeItem[nHeadX - 1][nHeadY].nSnakeType = BODY_R;
	snakeItem[nHeadX - 1][nHeadY].pNextItem = &snakeItem[nHeadX - 2][nHeadY];
	snakeItem[nHeadX - 1][nHeadY].pFrontItem = &snakeItem[nHeadX][nHeadY];
	snakeItem[nHeadX - 1][nHeadY].nItemX = nHeadX - 1;
	snakeItem[nHeadX - 1][nHeadY].nItemY = nHeadY;
	
	snakeItem[nHeadX - 2][nHeadY].bShow = true;
	snakeItem[nHeadX - 2][nHeadY].nSnakeType = BODY_R;
	snakeItem[nHeadX - 2][nHeadY].pNextItem = &snakeItem[nHeadX - 3][nHeadY];
	snakeItem[nHeadX - 2][nHeadY].pFrontItem = &snakeItem[nHeadX - 1][nHeadY];
	snakeItem[nHeadX - 2][nHeadY].nItemX = nHeadX - 2;
	snakeItem[nHeadX - 2][nHeadY].nItemY = nHeadY;
	
	snakeItem[nHeadX - 3][nHeadY].bShow = true;
	snakeItem[nHeadX - 3][nHeadY].nSnakeType = TAIL_R;
	snakeItem[nHeadX - 3][nHeadY].pNextItem = NULL;
	snakeItem[nHeadX - 3][nHeadY].pFrontItem = &snakeItem[nHeadX - 2][nHeadY];
	snakeItem[nHeadX - 3][nHeadY].nItemX = nHeadX - 3;
	snakeItem[nHeadX - 3][nHeadY].nItemY = nHeadY;

	return 4;
}

int CGameLogic::MoveSnake(SnakeItem snakeItem[][SNAKE_SIZE], SnakeItem *&pSankeHead, SnakeItem *&pSankeTail, int nDirection, bool bMoveTail)
{
	int nHeadX = pSankeHead->nItemX;
	int nHeadY = pSankeHead->nItemY;
	int nTailX = pSankeTail->nItemX;
	int nTailY = pSankeTail->nItemY;
	
	int nNextX;
	int nNextY;
	int nHeadNextX;
	int nHeadNextY;

	if(nDirection == DIRECTION_U)
	{
		nNextX = nHeadX ;
		nNextY = nHeadY - 1;
		if(nNextY < 0)
		{
			nNextY = SNAKE_SIZE - 1;
		}
	}
	else if(nDirection == DIRECTION_D)
	{
		nNextX = nHeadX ;
		nNextY = nHeadY + 1;
		if(nNextY >= SNAKE_SIZE)
		{
			nNextY = 0;
		}
	}
	else if(nDirection == DIRECTION_L)
	{
		nNextX = nHeadX - 1;
		nNextY = nHeadY;
		if(nNextX < 0)
		{
			nNextX = SNAKE_SIZE - 1;
		}
	}
	else if(nDirection == DIRECTION_R)
	{
		nNextX = nHeadX + 1;
		nNextY = nHeadY;
		if(nNextX >= SNAKE_SIZE)
		{
			nNextX = 0;
		}
	}
	else
	{
		return 2;
	}

	SnakeItem &snakeNewHeadItem = snakeItem[nNextX][nNextY];
	SnakeItem &snakeHeadItem = snakeItem[nHeadX][nHeadY];
	SnakeItem &snakeHeadNextItem = *snakeHeadItem.pNextItem;

	nHeadNextX = snakeHeadNextItem.nItemX;
	nHeadNextY = snakeHeadNextItem.nItemY;

	if(snakeNewHeadItem.bShow)
	{
		if(&snakeNewHeadItem != pSankeTail || &snakeNewHeadItem == pSankeTail && !bMoveTail)
		{
			//不能掉头
			if(&snakeNewHeadItem == &snakeHeadNextItem)
			{
				return 0;
			}
			//撞上
			return 1;
		}
	}

	if(bMoveTail)
	{
		//处理蛇尾
		SnakeItem &snakeTailItem = snakeItem[nTailX][nTailY];
		SnakeItem &snakeFrontItem = *snakeTailItem.pFrontItem;
		SnakeItem &snakeFrontFrontItem = *snakeFrontItem.pFrontItem;

		int nFrontTailX = snakeFrontItem.nItemX;
		int nFrontTailY = snakeFrontItem.nItemY;
		int nFrontFrontTailX = snakeFrontFrontItem.nItemX;
		int nFrontFrontTailY = snakeFrontFrontItem.nItemY;

		snakeTailItem.bShow = false;
		snakeTailItem.nEatType = 0;
		pSankeTail = &snakeFrontItem;

		if(nTailY == nFrontTailY)
		{	
			//向左
			if(snakeTailItem.nSnakeType == TAIL_L)
			{
				if(snakeFrontItem.nSnakeType == BODY_L)
				{
					snakeFrontItem.nSnakeType = TAIL_L;
				}
				else
				{
					if(snakeFrontItem.nSnakeType == BODY_LU)
					{
						snakeFrontItem.nSnakeType = TAIL_U;
					}
					else
					{
						snakeFrontItem.nSnakeType = TAIL_D;
					}
				}
			}
			else
			{
				if(snakeFrontItem.nSnakeType == BODY_R)
				{
					snakeFrontItem.nSnakeType = TAIL_R;
				}
				else
				{
					if(snakeFrontItem.nSnakeType == BODY_RU)
					{
						snakeFrontItem.nSnakeType = TAIL_U;
					}
					else
					{
						snakeFrontItem.nSnakeType = TAIL_D;
					}
				}
			}
		}
		else
		{
			//向上
			if(snakeTailItem.nSnakeType == TAIL_U)
			{
				if(snakeFrontItem.nSnakeType == BODY_U)
				{
					snakeFrontItem.nSnakeType = TAIL_U;
				}
				else
				{
					if(snakeFrontItem.nSnakeType == BODY_UL)
					{
						snakeFrontItem.nSnakeType = TAIL_L;
					}
					else
					{
						snakeFrontItem.nSnakeType = TAIL_R;
					}
				}
			}
			else
			{
				if(snakeFrontItem.nSnakeType == BODY_D)
				{
					snakeFrontItem.nSnakeType = TAIL_D;
				}
				else
				{
					if(snakeFrontItem.nSnakeType == BODY_DL)
					{
						snakeFrontItem.nSnakeType = TAIL_L;
					}
					else
					{
						snakeFrontItem.nSnakeType = TAIL_R;
					}
				}
			}
		}
	}

	if(nDirection == DIRECTION_U)
	{
		//处理蛇头
		if(nHeadX == nHeadNextX)
		{
			snakeHeadItem.nSnakeType = BODY_U;
		}
		else
		{
			if(snakeHeadNextItem.nSnakeType == BODY_L || snakeHeadNextItem.nSnakeType == BODY_UL  || snakeHeadNextItem.nSnakeType == BODY_DL)
			{
				snakeHeadItem.nSnakeType = BODY_LU;
			}
			else
			{
				snakeHeadItem.nSnakeType = BODY_RU;
			}
		}

		snakeNewHeadItem.nSnakeType = HEAD_U;
		snakeNewHeadItem.bShow = true;
		snakeNewHeadItem.pNextItem = &snakeHeadItem;
		snakeNewHeadItem.pFrontItem = NULL;

		snakeHeadItem.pFrontItem = &snakeNewHeadItem;		
		snakeNewHeadItem.nItemX = nNextX;
		snakeNewHeadItem.nItemY = nNextY;

		pSankeHead = &snakeNewHeadItem;
	}
	else if(nDirection == DIRECTION_D)
	{
		//处理蛇头
		if(nHeadX == nHeadNextX)
		{
			snakeHeadItem.nSnakeType = BODY_D;
		}
		else
		{
			if(snakeHeadNextItem.nSnakeType == BODY_L || snakeHeadNextItem.nSnakeType == BODY_UL  || snakeHeadNextItem.nSnakeType == BODY_DL)
			{
				snakeHeadItem.nSnakeType = BODY_LD;
			}
			else
			{
				snakeHeadItem.nSnakeType = BODY_RD;
			}
		}

		snakeNewHeadItem.nSnakeType = HEAD_D;
		snakeNewHeadItem.bShow = true;
		snakeNewHeadItem.pNextItem = &snakeHeadItem;
		snakeNewHeadItem.pFrontItem = NULL;

		snakeHeadItem.pFrontItem = &snakeNewHeadItem;		
		snakeNewHeadItem.nItemX = nNextX;
		snakeNewHeadItem.nItemY = nNextY;

		pSankeHead = &snakeNewHeadItem;
	}
	else if(nDirection == DIRECTION_L)
	{
		//处理蛇头
		if(nHeadY == nHeadNextY)
		{
			snakeHeadItem.nSnakeType = BODY_L;
		}
		else
		{
			if(snakeHeadNextItem.nSnakeType == BODY_D || snakeHeadNextItem.nSnakeType == BODY_LD  || snakeHeadNextItem.nSnakeType == BODY_RD)
			{
				snakeHeadItem.nSnakeType = BODY_DL;
			}
			else
			{
				snakeHeadItem.nSnakeType = BODY_UL;
			}
		}

		snakeNewHeadItem.nSnakeType = HEAD_L;
		snakeNewHeadItem.bShow = true;
		snakeNewHeadItem.pNextItem = &snakeHeadItem;
		snakeNewHeadItem.pFrontItem = NULL;

		snakeHeadItem.pFrontItem = &snakeNewHeadItem;		
		snakeNewHeadItem.nItemX = nNextX;
		snakeNewHeadItem.nItemY = nNextY;

		pSankeHead = &snakeNewHeadItem;
	}
	else if(nDirection == DIRECTION_R)
	{
		//处理蛇头
		if(nHeadY == nHeadNextY)
		{
			snakeHeadItem.nSnakeType = BODY_R;
		}
		else
		{
			if(snakeHeadNextItem.nSnakeType == BODY_D || snakeHeadNextItem.nSnakeType == BODY_LD  || snakeHeadNextItem.nSnakeType == BODY_RD)
			{
				snakeHeadItem.nSnakeType = BODY_DR;
			}
			else
			{
				snakeHeadItem.nSnakeType = BODY_UR;
			}
		}

		snakeNewHeadItem.nSnakeType = HEAD_R;
		snakeNewHeadItem.bShow = true;
		snakeNewHeadItem.pNextItem = &snakeHeadItem;
		snakeNewHeadItem.pFrontItem = NULL;

		snakeHeadItem.pFrontItem = &snakeNewHeadItem;		
		snakeNewHeadItem.nItemX = nNextX;
		snakeNewHeadItem.nItemY = nNextY;
		
		pSankeHead = &snakeNewHeadItem;
	}

	return 0;
}

bool CGameLogic::RandEat(SnakeItem snakeItem[][SNAKE_SIZE], int &nEatX, int &nEatY)
{
	nEatX = rand() % SNAKE_SIZE;
	nEatY = rand() % SNAKE_SIZE;

	if(snakeItem[nEatX][nEatY].bShow)
	{
		for(int i = 0; i < SNAKE_SIZE; i++)
		{
			for(int j = 0; j < SNAKE_SIZE; j++)
			{
				nEatX++;
				if(nEatX >= SNAKE_SIZE)
				{
					nEatX = 0;
					nEatY++;
					if(nEatY >= SNAKE_SIZE)
					{
						nEatY = 0;
					}
				}
				if(!snakeItem[nEatX][nEatY].bShow)
				{
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}

	nEatX = INVALID_ITEM;
	nEatY = INVALID_ITEM;

	return false;
}