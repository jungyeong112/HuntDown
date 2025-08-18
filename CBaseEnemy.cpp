#include "pch.h"
#include "CBaseEnemy.h"

CBaseEnemy::CBaseEnemy()
{
}

CBaseEnemy::~CBaseEnemy()
{
}

void CBaseEnemy::Initialize()
{
	
}

int CBaseEnemy::Update()
{
	CObj::Update_Rect();

	return 0;
}

void CBaseEnemy::LateUpdate()
{
	Move_BodyFrame();
}

void CBaseEnemy::Render(HDC hDC)
{
}

void CBaseEnemy::Release()
{
}
