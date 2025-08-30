#pragma once
#include "CScene.h"

class CMenu : public CScene
{
	enum INTRO {INTRO_FIRST,INTRO_SECOND,INTROEND};
public:
	CMenu();
	~CMenu();

public:

	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
public:
	void InsertBMP();
	void ChangeAnim(float fDeltaTime);

	float m_fElapsedTime{ 0.f };
	INTRO m_eState{INTROEND};
	
};

