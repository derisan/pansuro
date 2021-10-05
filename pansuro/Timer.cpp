#include "pch.h"
#include "Timer.h"

Timer* Timer::Instance()
{
	static Timer instance;
	return &instance;
}

void Timer::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mFrequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&mPrevCount));
}

void Timer::Update()
{
	UINT64 currentCount = 0;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	mDeltaTime = (currentCount - mPrevCount) / static_cast<float>(mFrequency);

	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	mPrevCount = currentCount;

	mFrameCount++;
	mFrameTime += mDeltaTime;

	if (mFrameTime > 1.0f)
	{
		mFPS = static_cast<UINT>(mFrameCount / mFrameTime);

		mFrameTime = 0.0f;
		mFrameCount = 0;
	}
}
