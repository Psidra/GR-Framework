#ifndef Animation
#define Animation

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; leftright or updown
	void SetAnimationStatus(bool m_bFacingUp, bool m_bIsMoving, bool m_bIsHurt, double dt, bool _dodging = false);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);

	// Get the Animation index
	int GetAnimationIndex(void) const;

	void SetIndices_fStand(const int m_ifStand_Start, const int m_ifStand_End);
	void SetIndices_bStand(const int m_ibStand_Start, const int m_ibStand_End);
	void SetIndices_fWalk(const int m_ifWalk_Start, const int m_ifWalk_End);
	void SetIndices_bWalk(const int m_ibWalk_Start, const int m_ibWalk_End);
	void SetIndices_fHurt(const int m_ifHurt_Start, const int m_ifHurt_End);
	void SetIndices_bHurt(const int m_ibHurt_Start, const int m_ibHurt_End);
	void SetIndices_Succ(const int m_iSucc_Start, const int m_iSucc_End);
	void SetIndices_fDodge(const int m_ifDodge_Start, const int m_ifDodge_End);
	void SetIndices_bDodge(const int m_ibDodge_Start, const int m_ibDodge_End);

private:
	int m_iAnimation_Index;
	double m_dElapsedAnimTime;		//animation time elapsed 
	double m_dTimeBetweenEachFrame;	//time between frames

	bool m_bFacingUp;		//check if facing up
	bool m_bIsMoving;		//check if character moving
	bool m_bIsHurt;			//check if character is hurt
	bool dodging;
	bool m_bIsDead;

	//STAMDING
	int m_ifStand_Start, m_ifStand_End, m_ibStand_Start, m_ibStand_End;

	//WALKING
	int m_ifWalk_Start, m_ifWalk_End, m_ibWalk_Start, m_ibWalk_End;

	//HURT
	int m_ifHurt_Start, m_ifHurt_End, m_ibHurt_Start, m_ibHurt_End;

	//SUCC
	int m_iSucc_Start, m_iSucc_End;

	//DODGE
	int m_ifDodge_Start, m_ifDodge_End, m_ibDodge_Start, m_ibDodge_End;
};

#endif // !Animation