#include "Animation.h"
#include <iostream>
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
{
}


CAnimation::~CAnimation()
{
}

// Set Animation status; leftright or updown
void CAnimation::SetAnimationStatus(bool m_bFacingUp, bool m_bIsMoving)
{
	if (this->m_bFacingUp != m_bFacingUp || this->m_bIsMoving != m_bIsMoving)
	{
		this->m_bFacingUp = m_bFacingUp;
		this->m_bIsMoving = m_bIsMoving;

		if (m_bIsMoving && !m_bFacingUp)
			m_iAnimation_Index = m_ifWalk_Start;
		else if (m_bIsMoving && m_bFacingUp)
				m_iAnimation_Index = m_ibWalk_Start;
		else if (!m_bIsMoving && !m_bFacingUp)
			m_iAnimation_Index = m_ifStand_Start;
		else if (!m_bIsMoving && m_bFacingUp)
			m_iAnimation_Index = m_ibStand_Start;
	}

	UpdateAnimationIndex();
}
// Update the Animation Index
void CAnimation::UpdateAnimationIndex()// need dt in param
{
	if (m_bIsMoving && m_bFacingUp)
	{
		m_iAnimation_Index += 1;
		if (m_iAnimation_Index > m_ibWalk_End)
			m_iAnimation_Index = m_ibWalk_Start;
	}
	else if (m_bIsMoving && !m_bFacingUp)
	{
		m_iAnimation_Index += 1;
		if (m_iAnimation_Index > m_ifWalk_End)
			m_iAnimation_Index = m_ifWalk_Start;
	}
	else if (!m_bIsMoving && !m_bFacingUp)
	{
		m_iAnimation_Index += 1;
		if (m_iAnimation_Index > m_ifStand_End)
			m_iAnimation_Index = m_ifStand_Start;
	}
	else if (!m_bIsMoving && m_bFacingUp)
	{
		m_iAnimation_Index += 1;
		if (m_iAnimation_Index > m_ibStand_End)
			m_iAnimation_Index = m_ibStand_Start;
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
