#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);		// 매개변수와 호환되는 dc를 할당

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,			// 프로그램 인스턴스 핸들
		pFilePath,		// 파일의 경로 전달
		IMAGE_BITMAP,	// 어떤 타입인지 알려주는 열거체
		0, 0,			// 가로, 세로
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);	// 
	// LR_LOADFROMFILE : 파일에서 이미지를 불러오는 옵션
	// LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환
	if (!m_hBitmap) {
		MessageBox(nullptr, L"비트맵 로드 실패", L"에러", MB_OK);
	}
// SelectObject : gdi 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
