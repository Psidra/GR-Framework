#pragma once

#include "Controller.h"
#include <fstream>
#include "KeyboardController.h"

class Keyboard :
	public Controller
{
private:
    unsigned int KeyList[NUM_CONRTOLLER];
	unsigned int pressMethod[NUM_CONRTOLLER];
	std::vector <unsigned char> Keys;

	unsigned int key;
public:
	Keyboard();
	virtual ~Keyboard();
	// Create this controller
	virtual bool Create(Player* thePlayerInfo = NULL);
	// Read from the controller
    virtual bool Load(std::string _filePath);
	virtual int Read(const float deltaTime);
	virtual void Write(std::string _filePath, unsigned int input_index, int input_key);
	virtual void MapKeys(std::string _filePath);
	virtual void ConvertInt();
	virtual unsigned int GetKey() { return this->key; }
};