#pragma once
#pragma once
#include "pch.h"

class TimeManager
{
private:
	TimeManager() {}
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

public:
	static TimeManager* GetInstance()
	{
		static TimeManager s_instance;
		return &s_instance;
	}

public:
	void Init()
	{
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
	}

	void Update()
	{
		unsigned __int64 currentCount = 0;
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

		_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
		_prevCount = currentCount;

		_frameCount++;
		_frameTime += _deltaTime;

		if (_frameTime >= 1.f)
		{
			_fps = static_cast<unsigned int>(_frameCount / _frameTime);

			_frameTime = 0.f;
			_frameCount = 0;
		}
	}

	unsigned int GetFps() const { return _fps; }
	float GetDeltaTime() const { return _deltaTime; }

private:
	unsigned __int64 _frequency = 0;
	unsigned __int64 _prevCount = 0;
	float _deltaTime = 0.f;

private:
	float _frameTime = 0.f;
	unsigned int _frameCount = 0;
	unsigned int _fps = 0;
};