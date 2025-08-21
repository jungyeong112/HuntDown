#pragma once
#include "CScene.h"

class CStage : public CScene
{
public:
	CStage();
	~CStage();

public:

	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void CreateMap();
	void CreateUI();
};

