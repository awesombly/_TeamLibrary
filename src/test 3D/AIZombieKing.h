#include "AIZombie.h"


class AIZombieKing : public AIZombie
{
public:
	GameObject* m_Breath = nullptr;
	float m_delayBreath = 0.0f;
	float m_delayStump  = 0.0f;
public:
	void DeadEvent()									noexcept override;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone()									noexcept override;
public:
	AIZombieKing();
	virtual ~AIZombieKing() = default;
};