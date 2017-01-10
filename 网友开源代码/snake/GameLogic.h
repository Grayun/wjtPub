#pragma once

#define				SNAKE_SIZE					12
#define				HEAD_U						1
#define				HEAD_D						2   
#define				HEAD_L						3   
#define				HEAD_R						4 

#define				TAIL_U						5
#define				TAIL_D						6   
#define				TAIL_L						7   
#define				TAIL_R						8  

#define				BODY_U						9
#define				BODY_D						10   
#define				BODY_L						11   
#define				BODY_R						12  

#define				BODY_RD						13
#define				BODY_RU						14   
#define				BODY_LD						15
#define				BODY_LU						16  

#define				BODY_DL						17
#define				BODY_DR						18   
#define				BODY_UL						19  
#define				BODY_UR						20

#define				DIRECTION_U					1
#define				DIRECTION_D					2
#define				DIRECTION_L					3
#define				DIRECTION_R					4

#define				INVALID_ITEM				0xFFFFFFFF
#define				MOVE_TIME					75
struct SnakeItem
{
	int nItemX;
	int nItemY;
	int nSnakeType;
	int nEatType;
	bool bShow;
	SnakeItem *pNextItem;
	SnakeItem *pFrontItem;
};

class CGameLogic
{
	int m_nSnakeWidth;
public:
	CGameLogic(void);
	~CGameLogic(void);

	int GetImagePosX(SnakeItem &snakeItem);
	int GetImagePosY(SnakeItem &snakeItem);
	int InitSnake(SnakeItem snakeItem[][SNAKE_SIZE], SnakeItem *&pSankeHead, SnakeItem *&pSankeTail, int nSnakeWidth);
	int MoveSnake(SnakeItem snakeItem[][SNAKE_SIZE], SnakeItem *&pSankeHead, SnakeItem *&pSankeTail, int nDirection, bool bMoveTail);
	bool RandEat(SnakeItem snakeItem[][SNAKE_SIZE], int &nEatX, int &nEatY);
};
