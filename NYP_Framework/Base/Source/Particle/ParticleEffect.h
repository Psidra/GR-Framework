///////////////////////////////////////////////////////////////////////////////
/*!
\file   ParticleEffect.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for particle effect. This class contains the entire particle effect
system.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#pragma once

#include "EntityBase.h"
#include "Emitter.h"
#include "Particle.h"

class GenericEntity;

class ParticleEffect : public EntityBase
{
public:
	ParticleEffect() {};
	ParticleEffect(Vector3 pos, Vector3 vel, EFFECT_TYPE type, double effect_life_span, double particle_life_span, bool follow, GenericEntity* targetEnt);
	virtual ~ParticleEffect() 
	{
		this->getEmitter()->~Emitter();
		delete m_emitter;
		m_emitter = NULL;
	};

	virtual void Update(double _dt);
	virtual void Render();

	/*set the mesh for the particles*/
	void setMesh(Mesh* meshName);
	/*get the mesh for the particle*/
	Mesh* getMesh();

	/*get the emitter*/
	Emitter* getEmitter();

private:
	Mesh* m_modelMesh; //Stores the mesh for the particle
	Emitter* m_emitter; //Emitter that stores all particles
};

namespace Create
{
	/*
	Namespace to create a particle effect
	Params:
			string _meshName			//Mesh name of the particle
			Vector3 Pos;				//Position where the emitter should be
			Vector3 Vel;				//Velocity of the emitter, if you want to move it
			EFFECT_TYPE type;			//Type of particle effect
			double effect_life_span;	//How long should this effect live (Try not to go over 5s)
			double particle_life_span;	//How long should this effect's particle live (Try not to go over 5s)
	*/
	ParticleEffect* Particle(
		const std::string& _meshName,
		const Vector3& pos,
		const Vector3& vel,
		const EFFECT_TYPE& type,
		const double& effect_life_span,
		const double& particle_life_span,
		const bool& follow,
		GenericEntity* targetEnt = nullptr);
}