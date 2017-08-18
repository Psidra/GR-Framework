#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Collider\Collider.h"
#include "Mesh.h"

class UIElement : public Collision
{
public:
	UIElement();
	~UIElement();

	enum ELEMENT_TYPE {
		NONE = 0,
		CURSOR,
		START,
		RESUME,
		BACK,
		EXIT,
		VOL_UP,
		VOL_DOWN,
	}type;

	void Render();

private:
	Mesh* mesh;

};

#endif // !UIELEMENT_H