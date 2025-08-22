#pragma once

#include "CUI.h"

class CUI_HPBar : public CUI
{
public:
	CUI_HPBar();
	~CUI_HPBar();

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Update_BGRect();
private:
	INFO m_tBgInfo{};
	RECT m_tBgRECT{};
};

