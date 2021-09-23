#pragma once

#define TIMER Timer::Instance()

class Timer
{
public:
	static Timer* Instance();

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	void OnInit();
	void OnUpdate();

	UINT GetFPS() const { return mFPS; }
	float GetDeltaTime() const { return mDeltaTime; }

private:
	Timer() = default;

private:
	UINT64 mFrequency;
	UINT64 mPrevCount;
	UINT mFrameCount;
	UINT mFPS;
	float mDeltaTime;
	float mFrameTime;
};

