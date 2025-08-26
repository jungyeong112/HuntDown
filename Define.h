#pragma once

extern	HWND	g_hWnd;


#define WINCX 800
#define WINCY 600
#define WINFPS 60

#define PURE = 0

#define PI  3.14f

#define OBJ_DIE 1
#define OBJ_NO_EVENT 0 

#define VK_MAX 0xff



enum OBJID { ITEM, ENEMY, GROUND, FLAT_GROUND, HIDE_AREA, WALL, PLAYER, BULLET, ENEMYBULLET, BOX, EFFECT, EXPLOSION, KICK, ENEMY_MELEE, BOXBREAKER, OBJ_END };
enum CHANNELID { SOUND_EFFECT, SOUND_BGM, MAXCHANNEL };

typedef struct tagInfo
{
	float fX, fY;
	float fCX, fCY;
}INFO;

typedef struct VECTOR2
{
	VECTOR2(float _x = 0, float _y = 0) : fx(_x), fy(_y) {}
	float fx, fy;

}Vector2;


template<typename T>
void  Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

struct tagFinder
{
	tagFinder(const TCHAR* pTag) : m_pTag(pTag) {}

	template<typename T>
	bool	operator()(T& MyPair)
	{
		return !lstrcmp(m_pTag, MyPair.first);
	}

	const TCHAR* m_pTag;
};

struct DeleteMap
{
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};
typedef struct tagFrame
{
	int		iStart;
	int		iEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

}FRAME;