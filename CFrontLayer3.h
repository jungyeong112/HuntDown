#pragma once
#include "CFrontLayer.h"
class CFrontLayer3 : public CFrontLayer
{
public:
	CFrontLayer3();
	~CFrontLayer3();

	void Initialize() override;
	void Render(HDC hDC) override;
};

