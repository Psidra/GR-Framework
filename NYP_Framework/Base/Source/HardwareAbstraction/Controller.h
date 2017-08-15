#pragma once

#include "../PlayerInfo/PlayerInfo.h"

class Controller
{
protected:
	Player* thePlayerInfo;
public:

	//Add more based on how many functions you have
    enum CONTROLLER_STATES
    {
        CONTROLLER_MOVEUP = 0,
        CONTROLLER_MOVEDOWN,
        CONTROLLER_MOVELEFT,
        CONTROLLER_MOVERIGHT,

        NUM_CONRTOLLER,
    };

	Controller();
	virtual ~Controller();

	// Create this controller
	virtual bool Create(Player* thePlayerInfo = NULL);
	// Read from the controller
	virtual int Read(const float deltaTime);

	bool(Controller::*controllerfunc[NUM_CONRTOLLER])(double dt);

	virtual bool MoveUp(double dt);
	virtual bool MoveDown(double dt);
	virtual bool MoveLeft(double dt);
	virtual bool MoveRight(double dt);


};

