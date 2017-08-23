#ifndef Animation
#define Animation

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; leftright or updown
	void SetAnimationStatus(bool m_bFacingUp, bool m_bIsMoving, bool m_bIs_Shooting, double dt);
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

private:
	int m_iAnimation_Index;
	double m_dElapsedAnimTime;		//animation time elapsed 
	double m_dTimeBetweenEachFrame;	//time between frames

	bool m_bFacingUp;		//check if facing up
	bool m_bIsMoving;		//check if character moving
	bool m_bIsHurt;			//check if character is hurt
	bool m_bIsDead;

	//STAMDING
	int m_ifStand_Start, m_ifStand_End, m_ibStand_Start, m_ibStand_End;

	//WALKING
	int m_ifWalk_Start, m_ifWalk_End, m_ibWalk_Start, m_ibWalk_End;

	//HURT
	int m_ifHurt_Start, m_ifHurt_End, m_ibHurt_Start, m_ibHurt_End;

};

#endif // !Animation