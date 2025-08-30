#pragma once
#include "CMuzzle_Flash.h"
class CMuzzle_Siege :public CMuzzle_Flash
{
public:
	CMuzzle_Siege();
	~CMuzzle_Siege();
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

