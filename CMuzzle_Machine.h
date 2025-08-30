#pragma once
#include "CMuzzle_Flash.h"
class CMuzzle_Machine : public CMuzzle_Flash
{
public:
	CMuzzle_Machine();
	~CMuzzle_Machine();
public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
};

