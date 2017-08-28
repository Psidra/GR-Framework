#include "DetectMemoryLeak.h"
#include "TextEntityManager.h"
#include "TextEntity.h"

TextEntityManager::TextEntityManager()
{
}

TextEntityManager::~TextEntityManager()
{
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
	return false;
}
