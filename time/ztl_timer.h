#pragma once
#include <Windows.h>
/*��ʱ����*/
namespace ztl
{
	class Timer
	{
	public:
		Timer() :StartCycles(0), EndCycles(0), SumCycles(0)
		{
		}
	public:
		/*��ʼ��ʱ*/
		void Start()
		{
			StartCycles = GetCurrentCycles();
		}
		/*��ֹ��ʱ*/
		void Stop()
		{
			EndCycles = GetCurrentCycles();
			SumCycles += EndCycles - StartCycles;
			EndCycles = StartCycles = 0;
		}
		/*���ü�ʱ��*/
		void Reset()
		{
			SumCycles = EndCycles = StartCycles = 0;
		}
		/*��ʼ���ò��¼�ʱ*/
		void ReStart()
		{
			Reset();
			Start();
		}
		/*�жϼ�ʱ���Ƿ�������*/
		bool IsRunning()
		{
			return StartCycles != 0;
		}
		/*��������*/
		long double GetElapsedTime()
		{
			return double(SumCycles) / GetTimerFrequency();
		}
		/*������ü�ʱ��������*/
		LONGLONG GetElapsedCyCles()
		{
			return SumCycles;
		}
		/*��ȡ��ʱ��Ƶ��*/
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
		LONGLONG SumCycles;//�ܵ�ʱ��������
		LONGLONG StartCycles;//��ʼ��ʱ��������
		LONGLONG EndCycles;//������ʱ��������
	};
}