#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Collider\Collider.h"
#include "EntityBase.h"
#include "TextEntity.h"

class Mesh;

class UIElement : public EntityBase, public Collision // Not inheriting generic entity or entity base as I wanted UI element to be separate
{
public:
	UIElement();
	UIElement(Mesh* _mesh);
	~UIElement();

	enum ELEMENT_STATE {
		MAIN_MENU = 0,
		PLAYING,
		PAUSE,
		OPTIONS,
		ALL,
		VICTORY,
		DEFEAT,
	}elestate;

	enum ELEMENT_TYPE {
		NONE = 0,
		CURSOR,
		START,
		RESUME,
		OPTION,
		BACK,
		EXIT,
		VOL_UP,
		VOL_DOWN,

		INPUT_RELOAD,
		INPUT_MOVE_UP,
		INPUT_MOVE_DOWN,
		INPUT_MOVE_LEFT,
		INPUT_MOVE_RIGHT,
		INPUT_BLANK,
		INPUT_MAP,
		INPUT_NEXT_GUN,
		INPUT_PAUSE,
		CANCEL,
		CONFIRM

	}type;

	//void SetPosition(const Vector3& _value);
	//Vector3 GetPosition();

	//void SetScale(const Vector3& _value);
	//Vector3 GetScale();

	//bool HasCollider(void) const;
	//void SetCollider(const bool _value);

	void Update();
	void Render();
	void CollisionResponse(UIElement * ThatElement);

private:
	Mesh* mesh;
	//TextEntity* textObj;
	//Vector3 position;
	//Vector3 scale;

	//bool m_bCollider;
};

namespace Create
{
	UIElement* UIEntity(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		bool _collision = false);
};

#endif // !UIELEMENT_H