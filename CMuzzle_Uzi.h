#pragma once
#include "CMuzzle_Flash.h"
class CMuzzle_Uzi :public CMuzzle_Flash
{
public:
	CMuzzle_Uzi();
	~CMuzzle_Uzi();
	int Update() override;
};

