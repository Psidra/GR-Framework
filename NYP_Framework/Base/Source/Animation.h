#ifndef Animation
#define Animation

class CAnimation
{
public:
	CAnimation();
	virtual ~CAnimation();

	// Set Animation status; leftright or updown
	void SetAnimationStatus(bool m_bFacingUp, bool m_bFacingRight, bool m_bIsMoving);
	// Update the Animation Index
	void UpdateAnimationIndex();

	// Get the Animation status
	bool GetAnimationStatus(void) const;


	// Get the Animation index
	int GetAnimationIndex(void) const;

	// Set rightup indices
	void SetRightUpIndices(const int m_iRightUp_Start, const int m_iRightUp_End);
	// Set leftup indices
	void SetLeftUpIndices(const int m_iLeftUp_Start, const int m_iLeftUp_End);
	// Set rightdown indices
	void SetRightDownIndices(const int m_iRightDown_Start, const int m_iRightDown_End);
	// Set leftdown indices
	void SetLeftDownIndices(const int m_iLeftDown_Start, const int m_iLeftDown_End);

private:

	bool m_bAnimationInvert;//check if invert
	bool m_bFacingUp;		//check if facing up
	bool m_bFacingRight;	//check if facing up
	bool m_bIsMoving;		//checl of character moving

	// 0 == The default hero frame, 
	int m_iAnimation_Index;

	//facing direction
	//-----facing up
	// Start index of right facing Animations
	int m_iRightUp_Start;
	// End index of right facing Animations
	int m_iRightUp_End;
	// Start index of left facing Animations
	int m_iLeftUp_Start;
	// End index of left facing Animations
	int m_iLeftUp_End;

	//-----facing down
	// Start index of right facing Animations
	int m_iRightDown_Start;
	// End index of right facing Animations
	int m_iRightDown_End;
	// Start index of left facing Animations
	int m_iLeftDown_Start;
	// End index of left facing Animations
	int m_iLeftDown_End;
};

#endif // !Animation