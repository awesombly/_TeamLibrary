#include "AIZombie.h"


class AIZombieKing : public AIZombie
{
public:

public:
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone()									noexcept override;
public:
	AIZombieKing();
	virtual ~AIZombieKing() = default;
};