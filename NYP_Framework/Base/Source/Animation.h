#ifndef Animation
#define Animation

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; leftright or updown
	void SetAnimationStatus(bool m_bFacingUp, bool m_bIsMoving);
	// Update the Animation Index
	void UpdateAnimationIndex();

	// Get the Animation index
	int GetAnimationIndex(void) const;

	void SetIndices_fStand(const int m_ifStand_Start, const int m_ifStand_End);
	void SetIndices_bStand(const int m_ibStand_Start, const int m_ibStand_End);
	void SetIndices_fWalk(const int m_ifWalk_Start, const int m_ifWalk_End);
	void SetIndices_bWalk(const int m_ibWalk_Start, const int m_ibWalk_End);

private:
	int m_iAnimation_Index;

	bool m_bFacingUp;		//check if facing up
	bool m_bIsMoving;		//check if character moving

	//STAMDING
	int m_ifStand_Start, m_ifStand_End, m_ibStand_Start, m_ibStand_End;

	//WALKING
	int m_ifWalk_Start, m_ifWalk_End, m_ibWalk_Start, m_ibWalk_End;
};

#endif // !Animation