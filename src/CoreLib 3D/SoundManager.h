#pragma once
#include "IBasis.h"
#include "ISingleton.h"
#include "MySound.h"


class SoundManager : public IBasis, public ISingleton<SoundManager>
{
private:	
	FMOD::System*			m_pSystem;
	TCHAR					m_Buffer[256];

	map<string, MySound>	m_SoundList;
	MySound*				m_curBGM;
	float					m_masterVolume = 1.0f;
public:
	// 사운드 이름, 위치, 사거리
	queue<tuple<string, D3DXVECTOR3, float> > m_SoundQueue;
	D3DXVECTOR3* m_pListenerPos = nullptr;
public:
	void	Load(const string_view& soundName, const bool& isPlay = false, const int& option = FMOD_DEFAULT) noexcept;
	void	SetBGM(const string_view& soundName) noexcept;
	void	Play(const string_view& soundName, const bool& isPlay = false) noexcept;
	void    PlayVariation(string&& soundName, const bool& isPlay, const int& vCount) noexcept;
	void	Stop(const string_view& soundName)	 noexcept;
	void	Paused(const string_view& soundName) noexcept;
	void	OperVolume(const string_view& soundName, const float& value) noexcept;
	void	SetMasterVolume(const float& value)	 noexcept;
public:
	bool	Init()		noexcept override;
	bool	Frame()		noexcept override;
	bool	Render()	noexcept override;
	bool	Release()	noexcept override;
private:
	friend class  ISingleton<SoundManager>;
	SoundManager() = default;
public:
	virtual ~SoundManager() = default;
};