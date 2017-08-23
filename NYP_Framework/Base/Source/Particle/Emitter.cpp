///////////////////////////////////////////////////////////////////////////////
/*!
\file   Emitter.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for emitter class.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#include "Emitter.h"
#include "../GenericEntity.h"

void Emitter::update(double dt)
{
	/*Age the emitter*/
	this->m_age += dt;

	if (!this->m_bFollow)
		this->m_position += this->m_velocity * dt;
	else
		this->m_position = this->targetEnt->GetPosition();

	this->m_position.y -= 0.2f;

	if (this->m_age > this->m_emitterLifeSpan)
		this->m_isActive = false;

	/*Spawn particles*/
	if (m_liveParticleCount < MAX_PARTICLE && this->m_isActive)
	{
		Particle* particle = fetchParticle();
		particle->m_age = 0;
		particle->m_position = this->m_position;
		particle->m_direction = Vector3(0, 1, 0);
		particle->m_isActive = true;
		particle->m_scale = Vector3(0.1f, 0.1f, 0.1f);
		/*REDO: Based on velocity user input.*/ // If i feel like it
		particle->m_velocity = Vector3(Math::RandFloatMinMax(-1, 1), 0, Math::RandFloatMinMax(-1, 1));
		++m_liveParticleCount;
	}
	/*Update particles*/
	for (auto p : m_particles)
	{	
		//Skip update when particle is inactive
		if (!p->m_isActive) continue;

		//Age the particle
		p->m_age += dt;

		//Set particle inactive when particle is old
		if (p->m_age > this->m_particleLifeSpan)
		{
			p->m_isActive = false;
			p->m_position = this->m_position;
			--m_liveParticleCount;
		}
		//Update particles based on type
		switch (m_effect_type)
		{
		case ET_FIRE:
		{
			p->m_velocity += Vector3(0, -9.8, 0) * dt;
			p->m_position += p->m_velocity * dt;
			break;
		}	
		case ET_BLEED:
		{
			p->m_velocity += Vector3(0, -9.8f, 0) * dt;
			p->m_position += p->m_velocity * dt;
			break;
		}
		/*Add more particle types here*/
		default:
			break;
		}
	}

	if (m_liveParticleCount == 0)
	{
		this->m_isDone = true;
	}
}

void Emitter::render()
{
	for (auto p : m_particles)
	{
		//Skip render if inactive
		if (!p->m_isActive)continue;

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(p->m_position.x, p->m_position.y, p->m_position.z);
		/*modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (particle->pos.x + m_emitter.x), 
												camera.position.z - (particle->pos.z + m_emitter.z))),
												0.f, 1.f, 0.f);*/
		modelStack.Scale(p->m_scale.x, p->m_scale.y, p->m_scale.z);
		RenderHelper::RenderMesh(m_particle_mesh);
		modelStack.PopMatrix();
	}
}

Particle * Emitter::fetchParticle()
{
	/*Search for inactive particles*/
	for (std::vector<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		Particle *go = (Particle *)*it;
		if (!go->m_isActive)
		{
			go->m_isActive = true;
			return go;
		}
	}
	/*Not enough particles, create more*/
	for (unsigned i = 0; i < 10; ++i)
	{
		Particle *go = new Particle();
		m_particles.push_back(go);
	}
	/*Returns the last particle that's created*/
	Particle *go = m_particles.back();
	go->m_isActive = true;
	return go;
}

void Emitter::setMesh(Mesh * meshName)
{
	this->m_particle_mesh = meshName;
}

Mesh * Emitter::getMesh()
{
	return this->m_particle_mesh;
}

void Emitter::setPos(Vector3 pos)
{
	this->m_position = pos;
}

Vector3 Emitter::getPos()
{
	return this->m_position;
}

bool Emitter::getIsDone()
{
	return this->m_isDone;
}

bool Emitter::getFollow()
{
	return this->m_bFollow;
}

void Emitter::setFollow(bool _follow)
{
	this->m_bFollow = _follow;
}

