#include "pch.h"
#include "CKeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{

}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	//if ((GetAsyncKeyState(_iKey) & 0x0001))
	//	return true;
	//
	//return false;

	if ((GetAsyncKeyState(_iKey) & 0x8000))
		return true;

	return false;
}

// 키를 누르자 마자
// 이전에 눌린 적이 없고, 현재 눌린 상태
bool CKeyMgr::Key_Down(int _iKey)
{
	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

// 키를 눌렀다가 뗏을 때
// 이전에 누른 적이 있고 현재 누르지 않았을 때
bool CKeyMgr::Key_Up(int _iKey)
{
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

bool CKeyMgr::Key_Pressing_Only(int i_key)
{
	if ((GetAsyncKeyState(i_key) & 0x0001))
		return true;

	return false;
}

void CKeyMgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];

		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}
