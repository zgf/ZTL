#pragma once
#include <Windows.h>
/*计时器类*/
namespace ztl
{
	class Timer
	{
	public:
		Timer() :StartCycles(0), EndCycles(0), SumCycles(0)
		{
		}
	public:
		/*开始计时*/
		void Start()
		{
			StartCycles = GetCurrentCycles();
		}
		/*终止计时*/
		void Stop()
		{
			EndCycles = GetCurrentCycles();
			SumCycles += EndCycles - StartCycles;
			EndCycles = StartCycles = 0;
		}
		/*重置计时器*/
		void Reset()
		{
			SumCycles = EndCycles = StartCycles = 0;
		}
		/*开始重置并新计时*/
		void ReStart()
		{
			Reset();
			Start();
		}
		/*判断计时器是否在运行*/
		bool IsRunning()
		{
			return StartCycles != 0;
		}
		/*已用秒数*/
		long double GetElapsedTime()
		{
			return double(SumCycles) / GetTimerFrequency();
		}
		/*获得已用计时器周期数*/
		LONGLONG GetElapsedCyCles()
		{
			return SumCycles;
		}
		/*获取计时器频率*/
		LONGLONG GetTimerFrequency()
		{
			LARGE_INTEGER FrequencyTemp;
			QueryPerformanceFrequency(&FrequencyTemp);
			return FrequencyTemp.QuadPart;
		}
	private:
		LONGLONG GetCurrentCycles()
		{
			LARGE_INTEGER Temp;
			QueryPerformanceCounter(&Temp);
			return Temp.QuadPart;
		}
	private:
		LONGLONG SumCycles;//总的时钟周期数
		LONGLONG StartCycles;//开始计时的周期数
		LONGLONG EndCycles;//结束计时的周期数
	};
}