#include "EngineCore.h"
#include "EngineTime.h"
#include"NetworkEngine.h"
void Time::Initialize()
{
	beginTime = std::chrono::system_clock().now();
	endTime = std::chrono::system_clock().now();

	deltaTime = std::chrono::duration<float>(0);
	totalTime = std::chrono::duration<float>(0);
	frameCount = 0;
}

void Time::Destroy()
{
}

void Time::Update()
{
	endTime = std::chrono::system_clock().now();
	deltaTime = endTime - beginTime;

	if (NetworkEngine::Instance().IsServer() && deltaTime.count() < 0.01666f)
	{
		int sleepTime=(0.01666f-deltaTime.count())*1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

		endTime = std::chrono::system_clock().now();
		deltaTime = endTime - beginTime;
	}
	beginTime = endTime;
	totalTime += deltaTime;
	frameCount++;
}
