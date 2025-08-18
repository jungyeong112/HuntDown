#pragma once
#include "CScene.h"

class CMenu : public CScene
{
public:
	CMenu();
	~CMenu();

public:

	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
};

