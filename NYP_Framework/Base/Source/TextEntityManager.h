#pragma once

#include "SingletonTemplate.h"
#include <list>

class TextEntity;
class TextEntityManager : public Singleton<TextEntityManager>
{
	friend Singleton<TextEntityManager>;
private:

	TextEntityManager();
	virtual ~TextEntityManager();

	std::list<TextEntity*> TextEntityList;

public:
	void RenderUI();
	void AddTextEntity(TextEntity* _entity);
	bool RemoveTextEntity(TextEntity* _entity);

};