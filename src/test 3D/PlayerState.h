#pragma once

enum class EPlayerState :char {
	Basic = 0,
	Hit,
	LSkill,
	RSkill,
	Dead,
	Dash,
	Run,
	Special,
};

class PlayerController;

class PlayerState
{
public:
	PlayerController*	m_pOwner = nullptr;
	EPlayerState		m_myState;
public:
	PlayerState(const EPlayerState& state) : m_myState(state) {};
	virtual ~PlayerState() = default;

	bool CommonProcess(const float& spf) noexcept;
	virtual bool Process(const float& spf) noexcept = 0;
	virtual void StateInit(PlayerController* pOwner) noexcept = 0;
};

// 기본
class PlayerStateBasic : public PlayerState
{
public:
public:
	PlayerStateBasic() : PlayerState(EPlayerState::Basic) {};
	virtual ~PlayerStateBasic() = default;
public:
	virtual bool Process(const float& spf) noexcept override;
	virtual void StateInit(PlayerController* pOwner) noexcept override;
};

// LButton
class PlayerStateLSkill : public PlayerState
{
public:
	PlayerStateLSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~PlayerStateLSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// RButton
class PlayerStateRSkill : public PlayerState
{
public:
	PlayerStateRSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~PlayerStateRSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// 대쉬
class PlayerStateRun : public PlayerState
{
public:
	PlayerStateRun() : PlayerState(EPlayerState::Run) {};
	virtual ~PlayerStateRun() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// 방어
class PlayerStateGuard : public PlayerState
{
public:
	PlayerStateGuard() : PlayerState(EPlayerState::Run) {};
	virtual ~PlayerStateGuard() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

/// ==================================== 궁수 ===========================================
// 기본
class ArcherStateBasic : public PlayerState
{
public:
public:
	ArcherStateBasic() : PlayerState(EPlayerState::Basic) {};
	virtual ~ArcherStateBasic() = default;
public:
	virtual bool Process(const float& spf) noexcept override;
	virtual void StateInit(PlayerController* pOwner) noexcept override;
};

// LButton
class ArcherStateLSkill : public PlayerState
{
public:
	ArcherStateLSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~ArcherStateLSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// RButton
class ArcherStateRSkill : public PlayerState
{
public:
	ArcherStateRSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~ArcherStateRSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// 대쉬
class ArcherStateDash : public PlayerState
{
public:
	ArcherStateDash() : PlayerState(EPlayerState::Dash) {};
	virtual ~ArcherStateDash() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};


/// ==================================== 법사 ===========================================
// 기본
class MageStateBasic : public PlayerState
{
public:
public:
	MageStateBasic() : PlayerState(EPlayerState::Basic) {};
	virtual ~MageStateBasic() = default;
public:
	virtual bool Process(const float& spf) noexcept override;
	virtual void StateInit(PlayerController* pOwner) noexcept override;
};

// LButton
class MageStateLSkill : public PlayerState
{
public:
	MageStateLSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~MageStateLSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// RButton
class MageStateRSkill : public PlayerState
{
public:
	MageStateRSkill() : PlayerState(EPlayerState::LSkill) {};
	virtual ~MageStateRSkill() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};

// 대쉬
class MageStateDash : public PlayerState
{
public:
	MageStateDash() : PlayerState(EPlayerState::Dash) {};
	virtual ~MageStateDash() = default;
public:
	virtual bool Process(const float& spf)				noexcept override;
	virtual void StateInit(PlayerController* pOwner)	noexcept override;
};
