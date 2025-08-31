#pragma once
#include "CFrontLayer.h"
class CFrontLayer1_2 :public CFrontLayer
{
public:
	CFrontLayer1_2();
	~CFrontLayer1_2();

	void Render(HDC hDC) override;
};

