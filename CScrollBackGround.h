#pragma once

class CScrollBackGround
{
public:
	CScrollBackGround();
	~CScrollBackGround();

public:

	void Initialize(HDC backDC, int backW, int backH, float parallax, int yWorld );
	void Render(HDC hDC, int camX);

	
public:
	int SafeMod(int a, int b)
	{
		if (b <= 0)return 0;
		int m = a % b;
		if (m < 0) m += b;
		return m;
	}

private:

	HDC m_backDC = nullptr;
	int m_ibackW = 0;
	int m_ibackH = 0;
	float m_fparallax = 0.25f;
	int m_yWorld = 0;

};

