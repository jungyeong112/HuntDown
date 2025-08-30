#pragma once
#include "CMuzzle_Flash.h"
class CMuzzle_Shotgun : public CMuzzle_Flash
{
public:
	CMuzzle_Shotgun();
	~CMuzzle_Shotgun();
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	
};

