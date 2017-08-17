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
	this->controllerfunc[CONTROLLER_SHOOT] = &Controller::Shoot;
	this->controllerfunc[CONTROLLER_RELOAD] = &Controller::Reload;
	this->controllerfunc[CONTROLLER_CHANGE_WEAPON] = &Controller::ChangeWeapon;
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
	if (!Player::GetInstance()->isDodging())
		Player::GetInstance()->MoveUp();
	std::cout << "Front" << std::endl;
	return false;
}

bool Controller::MoveDown(double dt)
{
	if (!Player::GetInstance()->isDodging())
		Player::GetInstance()->MoveDown();
	std::cout << "Back" << std::endl;
	return false;
}

bool Controller::MoveLeft(double dt)
{
	if (!Player::GetInstance()->isDodging())
		Player::GetInstance()->MoveLeft();
	std::cout << "Left" << std::endl;
	return false;
}

bool Controller::MoveRight(double dt)
{
	if (!Player::GetInstance()->isDodging())
		Player::GetInstance()->MoveRight();
	std::cout << "Right" << std::endl;
	return false;
}

bool Controller::Shoot(double dt)
{
	if (!Player::GetInstance()->isDodging())
		Player::GetInstance()->Shoot(dt);
	std::cout << "Shoot" << std::endl;
	return false;
}

bool Controller::Reload(double dt)
{
	Player::GetInstance()->Reload(dt);
	std::cout << "Reload" << std::endl;
	return false;
}

bool Controller::ChangeWeapon(double dt)
{
	Player::GetInstance()->ChangeWeapon(dt);
	std::cout << "ChangeWeapon" << std::endl;
	return false;
}