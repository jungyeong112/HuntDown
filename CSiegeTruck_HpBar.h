#pragma once
#include "CUI_HPBar.h"
class CSiegeTruck_HpBar : public CUI_HPBar
{
public:
	CSiegeTruck_HpBar();
	~CSiegeTruck_HpBar();

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	virtual void Update_BGRect();

	float m_fRatio{ 0.f };
};

