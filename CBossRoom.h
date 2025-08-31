#pragma once
#include "CBossIntroAnim.h"
class CBossRoom :public CBossIntroAnim
{
public:
	CBossRoom();
	~CBossRoom();
	void Render(HDC hDC) override;

};

