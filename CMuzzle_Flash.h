#pragma once
#include "CEffect.h"

class CMuzzle_Flash : public CEffect
{

public:
	CMuzzle_Flash();
	~CMuzzle_Flash();

public:
	// CEffect을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	virtual void Set_FireOffset(Vector2 vPos) { m_vFireOffset = vPos; }

protected:
	VECTOR2 m_vFireOffset{ 0,0 };
};

