#include "Animation.h"
#include <iostream>
CAnimation::CAnimation()
	: m_bAnimationInvert(false)
	, m_bFacingUp(false)
	, m_bFacingRight(false)
	, m_bIsMoving(false)
	, m_iAnimation_Index(0)
	, m_iRightUp_Start(0)
	, m_iRightUp_End(0)
	, m_iLeftUp_Start(0)
	, m_iLeftUp_End(0)
	, m_iRightDown_Start(0)
	, m_iRightDown_End(0)
	, m_iLeftDown_Start(0)
	, m_iLeftDown_End(0)
{
}


CAnimation::~CAnimation()
{
}

// Set Animation status; leftright or updown
void CAnimation::SetAnimationStatus(bool m_bFacingUp, bool m_bFacingRight, bool m_bIsMoving)
{
	if (this->m_bFacingUp != m_bFacingUp || this->m_bFacingRight != m_bFacingRight)
	{
		this->m_bFacingUp = m_bFacingUp;
		this->m_bFacingRight = m_bFacingRight;
		this->m_bIsMoving = m_bIsMoving;	

		if (m_bFacingUp && m_bFacingRight)			//right up
			m_iAnimation_Index = m_iRightUp_Start;
		else if (m_bFacingUp && !m_bFacingRight)	//left up
			m_iAnimation_Index = m_iLeftUp_Start;
		else if (!m_bFacingUp && m_bFacingRight)	//right down
			m_iAnimation_Index = m_iRightDown_Start;
		else if (!m_bFacingUp && !m_bFacingRight)	//left down
			m_iAnimation_Index = m_iLeftDown_Start;
	}

	UpdateAnimationIndex();
}
// Update the Animation Index
void CAnimation::UpdateAnimationIndex()// need dt in param
{
	if (m_bFacingUp && m_bFacingRight)
	{
		if (m_iAnimation_Index >= m_iRightUp_End)
			m_iAnimation_Index = m_iRightUp_Start;
		m_iAnimation_Index += 1;
	}
	else if (m_bFacingUp && !m_bFacingRight)
	{
		if (m_iAnimation_Index >= m_iLeftUp_End)
			m_iAnimation_Index = m_iLeftUp_Start;
		m_iAnimation_Index += 1;
	}
	if (!m_bFacingUp && m_bFacingRight)
	{
		if (m_iAnimation_Index > m_iRightDown_End)
			m_iAnimation_Index = m_iRightDown_Start;
		m_iAnimation_Index += 1;
	}
	else if(!m_bFacingUp && !m_bFacingRight)
	{
		if (m_iAnimation_Index > m_iLeftDown_End)
			m_iAnimation_Index = m_iLeftDown_Start;
		m_iAnimation_Index += 1;
	}
}
// Get the Animation status
bool CAnimation::GetAnimationStatus(void) const
{
	return m_bAnimationInvert;
}
// Get the Animation index
int CAnimation::GetAnimationIndex(void) const
{
	return m_iAnimation_Index;
}

// Set rightup indices
void CAnimation::SetRightUpIndices(const int m_iRightUp_Start, const int m_iRightUp_End)
{
	this->m_iRightUp_Start = m_iRightUp_Start;
	this->m_iRightUp_End = m_iRightUp_End;
}

// Set leftup indices
void CAnimation::SetLeftUpIndices(const int m_iLeftUp_Start, const int m_iLeftUp_End)
{
	this->m_iLeftUp_Start = m_iLeftUp_Start;
	this->m_iLeftUp_End = m_iLeftUp_End;
}

// Set rightdown indices
void CAnimation::SetRightDownIndices(const int m_iRightDown_Start, const int m_iRightDown_End)
{
	this->m_iRightDown_Start = m_iRightDown_Start;
	this->m_iRightDown_End = m_iRightDown_End;
}

// Set rightdown indices
void CAnimation::SetLeftDownIndices(const int m_iLeftDown_Start, const int m_iLeftDown_End)
{
	this->m_iLeftDown_Start = m_iLeftDown_Start;
	this->m_iLeftDown_End = m_iLeftDown_End;
}

//
//// Set left indices
//void CAnimation::SetLeftIndices(const int m_iLeft_Start, const int m_iLeft_End)
//{
//	this->m_iLeft_Start = m_iLeft_Start;
//	this->m_iLeft_End = m_iLeft_End;
//}
//
//void CAnimation::SetUpIndices(const int m_iUp_Start, const int m_iUp_End)
//{
//	this->m_iUp_Start = m_iUp_Start;
//	this->m_iUp_End = m_iUp_End;
//}
//
//void CAnimation::SetDownIndices(const int m_iDown_Start, const int m_iDown_End)
//{
//	this->m_iDown_Start = m_iDown_Start;
//	this->m_iDown_End = m_iDown_End;
//}
