///////////////////////////////////////////////////////////////////////////////
/*!
\file   ParticleEffect.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for ParticleEffect class.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#include "ParticleEffect.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"

ParticleEffect::ParticleEffect(Vector3 pos, Vector3 vel, EFFECT_TYPE type, double effect_life_span, double particle_life_span, bool follow, GenericEntity* targetEnt)
{
	this->m_emitter = new Emitter(type, pos, vel, effect_life_span, particle_life_span, follow, targetEnt);
	position = pos;
}

void ParticleEffect::Update(double _dt)
{
	if (this->getEmitter()->getIsDone())
		this->SetIsDone(true);

	this->m_emitter->update(_dt);
	/*std::cout << "TILEID PARTILE: " << tile_ID << std::endl;
	std::cout << "TILEID PLAYER: " << Player::GetInstance()->GetTileID() << std::endl;*/

}

void ParticleEffect::Render()
{
	this->m_emitter->render();
}

void ParticleEffect::setMesh(Mesh * meshName)
{
	this->m_modelMesh = meshName;
}

Mesh * ParticleEffect::getMesh()
{
	return this->m_modelMesh;
}

Emitter * ParticleEffect::getEmitter()
{
	return this->m_emitter;
}

ParticleEffect * Create::Particle(
	const std::string& _meshName,
	const Vector3& pos,
	const Vector3& vel,
	const EFFECT_TYPE& type,
	const double& effect_life_span,
	const double& particle_life_span,
	const bool& follow,
	GenericEntity* targetEnt)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ParticleEffect* result = new ParticleEffect(pos, vel, type, effect_life_span, particle_life_span, follow, targetEnt);
	result->getEmitter()->setMesh(modelMesh);
	result->SetCollider(false); //Disable collision for this
	result->SetPhysic(false);

	//Adds it to entityManager
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}
