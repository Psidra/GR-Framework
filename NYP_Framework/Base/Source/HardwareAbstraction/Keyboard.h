#pragma once

#include "Controller.h"
//#include <map>
#include <fstream>

class Keyboard :
	public Controller
{
private:
    unsigned int KeyList[NUM_CONRTOLLER];
	unsigned int pressMethod[NUM_CONRTOLLER];
public:
	Keyboard();
	virtual ~Keyboard();
	// Create this controller
	virtual bool Create(Player* thePlayerInfo = NULL);
	// Read from the controller
    virtual bool Load(std::string _filePath);
	virtual int Read(const float deltaTime);
	virtual void Write(std::string _filePath);
	//virtual void MappingKeys(std::string _filePath);
};

