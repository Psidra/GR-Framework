#include "Enemy.h"
#include <iostream>

/********************************************************************************
Constructor
********************************************************************************/
CEnemy::CEnemy(Mesh* modelMesh) : modelMesh(modelMesh)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

/********************************************************************************
Strategy
********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
}

void CEnemy::Update(double dt)
{

}
void CEnemy::Render(void)
{

}