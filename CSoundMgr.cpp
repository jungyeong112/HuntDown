#include "pch.h"
#include "CSoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD::System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	//FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}
void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_mapSound.clear();

	m_pSystem->close();
	m_pSystem->release();
	m_pSystem = nullptr;
}


void CSoundMgr::PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	if (eID != MAXCHANNEL)
		StopSound(eID);

	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	bool bPlay = FALSE;

	if (m_pChannelArr[eID]->isPlaying(&bPlay))
	{
		m_pSystem->playSound(iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}

	m_pChannelArr[eID]->setVolume(fVolume);
	m_pSystem->update();
}

void CSoundMgr::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{

	StopSound(SOUND_BGM);
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, FALSE, &m_pChannelArr[SOUND_BGM]);

	m_pChannelArr[SOUND_BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[SOUND_BGM]->setVolume(fVolume);
	m_pSystem->update();
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	m_pChannelArr[eID]->stop();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
	{
		m_pChannelArr[i]->stop();
	}
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pChannelArr[eID]->setVolume(fVolume);

	m_pSystem->update();
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst("../Sounds/*.*", &fd); //long

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sounds/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../ Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);
}
