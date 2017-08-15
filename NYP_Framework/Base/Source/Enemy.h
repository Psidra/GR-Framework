#ifndef Enemies
#define Enemies

#include "GenericEntity.h"
#include "Collider\Collider.h"

#include "Strategy.h"
#include "Animation.h"

#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

class Mesh;

class CEnemy :public Collision, public EntityBase, public CAnimation
{
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget;
	Vector3 target;
	
	CStrategy* theStrategy;

	double m_dSpeed;
	double m_dAcceleration;
	Vector3 position, scale;
	Vector3 maxBoundary, minBoundary;
	
	
public:
	CEnemy(Mesh* modelMesh);
	virtual ~CEnemy();

	void Update(double dt = 0.0333f);
	void Render(void);

	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);

	GenericEntity* enemyType;
};

namespace Create
{
	CEnemy* Enemy(const std::string&_meshName, const Vector3& position, const Vector3& scale, const Vector3& maxAABB = Vector3(5, 5, 0), const Vector3& minAABB = Vector3(-5, -5, 0));
};

#endif // !Enemy