#include "Keyboard.h"
#include <iostream>
#include <sstream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

// Create this controller
bool Keyboard::Create(Player* thePlayerInfo)
{
	Controller::Create(thePlayerInfo);
	KeyList[CONTROLLER_MOVEUP] = 'W';
	KeyList[CONTROLLER_MOVEDOWN] = 'S';
	KeyList[CONTROLLER_MOVELEFT] = 'A';
	KeyList[CONTROLLER_MOVERIGHT] = 'D';
	KeyList[CONTROLLER_SHOOT] = VK_SPACE;
	KeyList[CONTROLLER_RELOAD] = 'R';
	KeyList[CONTROLLER_CHANGE_WEAPON] = 'Q';
	return false;
}


bool Keyboard::Load(std::string _filePath)
{
    return false;
}

// Read from the controller
int Keyboard::Read(const float deltaTime)
{
	Controller::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
	return 0;

	for (int i = 0;i < NUM_CONRTOLLER;++i)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(KeyList[i]))
		{
			(this->*(controllerfunc[i]))(deltaTime);
		}
	}
}
