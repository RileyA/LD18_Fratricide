#include <iostream>
#include "Engine.h"
#include "QEngine.h"
#include "TimeManager.h"
#include "Logger.h"

#include "LD18MenuState.h"
#include "LD18PlayState.h"
#include "UGLYGLOBALS.h"

namespace QE
{
	float difficulty = 1.f;
	int moveControl = 0;// 0 = arrows, 1 = mouse, 2 = WASD, 3 = ,AOE
	int actControl = 0;// 0 = ZX, 1 = LMB/RMB, 2 = right shift/ctrl, 3 = ;/Q
	long long score = 0;
}

#ifndef WIN32A
int main(int argc, char* argv[])
#else
int CALLBACK WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
int nCmdShow)
#endif
{
	QE::LD18MenuState* ld18ms = new QE::LD18MenuState();

	QE::TimeManager::getSingletonPtr();
	QE::QEngine* eng = new QE::QEngine();
	eng->addState(ld18ms);
	eng->initialize();
	eng->start();


	return 0;
}

