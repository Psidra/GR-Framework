#include "Controller.h"
#include <iostream>
using namespace std;

const bool _CONTROLLER_DEBUG = false;

Controller::Controller()
	: thePlayerInfo(NULL)
{
}


Controller::~Controller()
{
	// We just set thePlayerInfo to NULL without deleting. SceneText will delete this.
	if (thePlayerInfo)
		thePlayerInfo = NULL;
}


// Create this controller
bool Controller::Create(Player* thePlayerInfo)
{
	if (_CONTROLLER_DEBUG)
		cout << "Controller::Create()" << endl;
	this->thePlayerInfo = thePlayerInfo;
	this->controllerfunc[CONTROLLER_MOVEUP] = &Controller::MoveUp;
	this->controllerfunc[CONTROLLER_MOVEDOWN] = &Controller::MoveDown;
	this->controllerfunc[CONTROLLER_MOVELEFT] = &Controller::MoveLeft;
	this->controllerfunc[CONTROLLER_MOVERIGHT] = &Controller::MoveRight;

	return false;
}

// Read from the controller
int Controller::Read(const const float deltaTime)
{
	if (_CONTROLLER_DEBUG)
		cout << "Controller::Read()" << endl;
	return 0;
}

bool Controller::MoveUp(double dt)
{
	Player::GetInstance()->MoveUp(dt);
	std::cout << "Front" << std::endl;
	return false;
}

bool Controller::MoveDown(double dt)
{
	Player::GetInstance()->MoveDown(dt);
	std::cout << "Back" << std::endl;
	return false;
}

bool Controller::MoveLeft(double dt)
{
	Player::GetInstance()->MoveLeft(dt);
	std::cout << "Left" << std::endl;
	return false;
}

bool Controller::MoveRight(double dt)
{
	Player::GetInstance()->MoveRight(dt);
	std::cout << "Right" << std::endl;
	return false;
}
