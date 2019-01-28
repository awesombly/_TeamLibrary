#include "AIZombie.h"


class AIZombieEx : public AIZombie
{
public:

public:
	void DeadEvent()									noexcept override;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone()									noexcept override;
public:
	AIZombieEx();
	virtual ~AIZombieEx() = default;
};