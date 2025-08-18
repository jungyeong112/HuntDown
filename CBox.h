#pragma once
#include "CObj.h"


enum BOXTYPE
{
	WOOD_BOX, STONE_BOX,  GAS_BARREL, STEEL_BOX, BOX_END
};
class CBox : public CObj
{
public:
	CBox();
	~CBox();

public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;
	void Release() override;

	void OnCollision(FCollision _pCollision);
	void Set_Box(BOXTYPE eType, int iMaxHp, int iEndFrame);

public:
	BOXTYPE Get_Type() { return m_eBoxType;}
	void Set_Type(BOXTYPE eType) { m_eBoxType = eType; }
	void Change_Anim();
	
private:
	BOXTYPE m_eBoxType = BOX_END;
};

