#include "DetectMemoryLeak.h"
#include "Animation.h"
#include <iostream>
#include "PlayerInfo\PlayerInfo.h"
CAnimation::CAnimation()
	: m_bFacingUp(false)
	, m_bIsMoving(false)
	, m_iAnimation_Index(0)
	, m_ifStand_Start(0)
	, m_ifStand_End(0)
	, m_ibStand_Start(0)
	, m_ibStand_End(0)
	, m_ifWalk_Start(0)
	, m_ifWalk_End(0)
	, m_ibWalk_Start(0)
	, m_ibWalk_End(0)
	, m_iSucc_Start(0)
	, m_iSucc_End(0)
	, m_dElapsedAnimTime(0.0)
	, m_dTimeBetweenEachFrame(1.0)
{
}


CAnimation::~CAnimation()
{
}

// Set Animation status
void CAnimation::SetAnimationStatus(bool m_bFacingUp, bool m_bIsMoving, bool m_bIsHurt, double dt)
{
	if (this->m_bFacingUp != m_bFacingUp || this->m_bIsMoving != m_bIsMoving || this->m_bIsHurt != m_bIsHurt)
	{
		this->m_bFacingUp = m_bFacingUp;
		this->m_bIsMoving = m_bIsMoving;
		this->m_bIsHurt = m_bIsHurt;

		if (m_bIsHurt)
		{
			if (m_bFacingUp)
				m_iAnimation_Index = m_ibHurt_Start;
			else
				m_iAnimation_Index = m_ifHurt_Start;
		}
		else
		{
			if (m_bIsMoving && !m_bFacingUp)
				m_iAnimation_Index = m_ifWalk_Start;
			else if (m_bIsMoving && m_bFacingUp)
				m_iAnimation_Index = m_ibWalk_Start;
			else if (!m_bIsMoving && !m_bFacingUp)
				m_iAnimation_Index = m_ifStand_Start;
			else if (!m_bIsMoving && m_bFacingUp)
				m_iAnimation_Index = m_ibStand_Start;
		}
	}

	UpdateAnimationIndex(dt);
}
// Update the Animation Index
void CAnimation::UpdateAnimationIndex(double dt)// need dt in param
{
	m_dElapsedAnimTime += dt * 10;

	//if (m_bIsHurt)

	if (m_dElapsedAnimTime > m_dTimeBetweenEachFrame)
	{
		if (Player::GetInstance()->m_bPullEffect)
		{
			m_iAnimation_Index += 1;
			if (m_iAnimation_Index > m_iSucc_End)
				m_iAnimation_Index = m_iSucc_Start;
			m_dElapsedAnimTime = 0.0;
		}
		else if (m_bIsHurt)	//character hurt
		{
			if (m_bFacingUp)
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ibHurt_End)
					m_iAnimation_Index = m_ibHurt_Start;
				m_dElapsedAnimTime = 0.0;
			}
			else
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ifHurt_End)
					m_iAnimation_Index = m_ifHurt_Start;
				m_dElapsedAnimTime = 0.0;
			}
		}
		else
		{
			if (m_bIsMoving && m_bFacingUp)		//moving while looking up
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ibWalk_End)
					m_iAnimation_Index = m_ibWalk_Start;
				m_dElapsedAnimTime = 0.0;
			}
			else if (m_bIsMoving && !m_bFacingUp)	//moving while looking down
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ifWalk_End)
					m_iAnimation_Index = m_ifWalk_Start;
				m_dElapsedAnimTime = 0.0;
			}
			else if (!m_bIsMoving && !m_bFacingUp)	//standing while looking down
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ifStand_End)
					m_iAnimation_Index = m_ifStand_Start;
				m_dElapsedAnimTime = 0.0;
			}
			else if (!m_bIsMoving && m_bFacingUp)	//standing while looking up
			{
				m_iAnimation_Index += 1;
				if (m_iAnimation_Index > m_ibStand_End)
					m_iAnimation_Index = m_ibStand_Start;
				m_dElapsedAnimTime = 0.0;
			}
		}
	}
}
// Get the Animation index
int CAnimation::GetAnimationIndex(void) const
{
	return m_iAnimation_Index;
}

void CAnimation::SetIndices_fStand(const int m_ifStand_Start, const int m_ifStand_End)
{
	this->m_ifStand_Start = m_ifStand_Start;
	this->m_ifStand_End = m_ifStand_End;
}

void CAnimation::SetIndices_bStand(const int m_ibStand_Start, const int m_ibStand_End)
{
	this->m_ibStand_Start = m_ibStand_Start;
	this->m_ibStand_End = m_ibStand_End;
}

void CAnimation::SetIndices_fWalk(const int m_ifWalk_Start, const int m_ifWalk_End)
{
	this->m_ifWalk_Start = m_ifWalk_Start;
	this->m_ifWalk_End = m_ifWalk_End;
}

void CAnimation::SetIndices_bWalk(const int m_ibWalk_Start, const int m_ibWalk_End)
{
	this->m_ibWalk_Start = m_ibWalk_Start;
	this->m_ibWalk_End = m_ibWalk_End;
}

void CAnimation::SetIndices_fHurt(const int m_ifHurt_Start, const int m_ifHurt_End)
{	
	this->m_ifHurt_Start = m_ifHurt_Start;
	this->m_ifHurt_End = m_ifHurt_End;
}

void CAnimation::SetIndices_bHurt(const int m_ibHurt_Start, const int m_ibHurt_End)
{
	this->m_ibHurt_Start = m_ibHurt_Start;
	this->m_ibHurt_End = m_ibHurt_End;
}

void CAnimation::SetIndices_Succ(const int m_iSucc_Start, const int m_iSucc_End)
{
	this->m_iSucc_Start = m_iSucc_Start;
	this->m_iSucc_End = m_iSucc_End;
}
