#include "DetectMemoryLeak.h"
#include "TextEntityManager.h"
#include "TextEntity.h"

TextEntityManager::TextEntityManager()
{
}

TextEntityManager::~TextEntityManager()
{
	for (std::list<TextEntity*>::iterator it = TextEntityList.begin(); it != TextEntityList.end();++it)
	{
		//delete all entity in list
		EntityBase* temp = *it;
		delete temp;
		temp = NULL;
	}
}

void TextEntityManager::RenderUI()
{
	for (std::list<TextEntity*>::iterator it = TextEntityList.begin();
		it != TextEntityList.end(); ++it)
	{
		if (*it == NULL)
			continue;

		(*it)->RenderUI();
	}
}

void TextEntityManager::AddTextEntity(TextEntity * _entity)
{
	TextEntityList.push_back(_entity);
}

bool TextEntityManager::RemoveTextEntity(TextEntity * _entity)
{
	// Find the entity's iterator
	std::list<TextEntity*>::iterator findIter = std::find(TextEntityList.begin(), TextEntityList.end(), _entity);

	// Delete the entity if found
	if (findIter != TextEntityList.end())
	{
		delete *findIter;
		findIter = TextEntityList.erase(findIter);
		*findIter = NULL;
		return true;
	}

	// Return false if not found
	std::cout << "this text entity is not found!\n";
	return false;
}
