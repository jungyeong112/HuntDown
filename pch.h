// pch.h
#pragma once

//  시스템 헤더 먼저
#define NOMINMAX 
#include <Windows.h>

#include <list>
#include <vector>      // 'Vector' → 'vector'로 수정 필요
#include <map>
#include <algorithm>
#include <memory>
#include <string>
#include <io.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

//  GDI+ 네임스페이스
using namespace Gdiplus;

//  디버그 관련
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Debug.h"
#pragma comment(lib, "Gdi32.lib")
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

// 프로젝트 헤더
#include "framework.h"
#include "CalculateUtil.h"
#include "CScreenManager.h"

//  FMOD 관련
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

//  STL 네임스페이스는 cpp 파일에서 사용하는 게 안전하지만, 필요 시 아래처럼
using namespace std;