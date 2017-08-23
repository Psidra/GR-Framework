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
		CONTROLLER_SHOOT,
		CONTROLLER_RELOAD,
		CONTROLLER_CHANGE_WEAPON,
		CONTROLLER_SPAWN_ENEMY,
		CONTROLLER_ENLARGE_MAP,
		CONTROLLER_PAUSE,
		CONTROLLER_BLANK,
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
	virtual bool Shoot(double dt);
	virtual bool Reload(double dt);
	virtual bool ChangeWeapon(double dt);
	virtual bool SpawnEnemy(double dt);
	virtual bool EnlargeMap(double dt);
	virtual bool Pause(double dt);
	virtual bool Blank(double dt); // It would be funny if this was left empty huh
};

