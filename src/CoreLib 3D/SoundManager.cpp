#include "SoundManager.h"
#include "Input.h"
#include "Timer.h"


bool SoundManager::Init() noexcept
{
	FMOD::System_Create(&m_pSystem);
	//		       ä�μ�, �Ӽ�
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_curBGM = nullptr;
	return true;
}

bool SoundManager::Frame() noexcept
{
	m_pSystem->update();

#pragma region VolumeControl
	if (Input::Get().GetKeyState(VK_HOME) == EKeyState::HOLD)
	{
		if (m_curBGM != nullptr)
			OperVolume(m_curBGM->m_SoundName, Timer::SPF);
	}
	if (Input::Get().GetKeyState(VK_END) == EKeyState::HOLD)
	{
		if (m_curBGM != nullptr)
			OperVolume(m_curBGM->m_SoundName, -Timer::SPF);
	}
	if (Input::Get().GetKeyState(VK_INSERT) == EKeyState::HOLD)
	{
		m_masterVolume = clamp(m_masterVolume + Timer::SPF, 0.0f, 1.0f);
	}
	if (Input::Get().GetKeyState(VK_DELETE) == EKeyState::HOLD)
	{
		m_masterVolume = clamp(m_masterVolume - Timer::SPF, 0.0f, 1.0f);
	}
#pragma endregion


	while (!m_SoundQueue.empty())
	//if (!m_SoundQueue.empty())
	{
		auto&[soundName, position, range] = m_SoundQueue.front();
		if (m_SoundList.find(soundName) == m_SoundList.end())
		{
			ErrorMessage(__FUNCTION__ + " -> Sound Not Find : "s + soundName);
			m_SoundQueue.pop();
			//continue;
			return true;
		}

		position = position - *m_pListenerPos;
		float distance = D3DXVec3LengthSq(&position);
		if (distance <= range)
		{
			Play(soundName);
			m_SoundList[soundName].SetVolume((1.0f - distance / range) * m_masterVolume);
		}
		m_SoundQueue.pop();
	}
	return true;
}

bool SoundManager::Render() noexcept
{
	return true;
}

bool SoundManager::Release() noexcept
{
	// ���� ó��
	for (auto& iter : m_SoundList)
	{
		iter.second.m_Sound->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}


// ���� ���� �ε�
void SoundManager::Load(const string_view& soundName, const bool& isPlay, const int& option) noexcept
{
	m_SoundList[soundName.data()].m_SoundName = soundName.data();
	string soundPath = "../../data/sound/"s + soundName.data();
	m_pSystem->createSound(soundPath.c_str(), option, 0, &(m_SoundList[soundName.data()].m_Sound));

	if (isPlay)
	{
		m_SoundList[soundName.data()].SetVolume(m_masterVolume);
		m_pSystem->playSound(m_SoundList[soundName.data()].m_Sound, 0, false, &m_SoundList[soundName.data()].m_Channel);
	}
}

void SoundManager::SetBGM(const string_view& soundName) noexcept
{
	if( m_curBGM != nullptr)
		Stop(m_curBGM->m_SoundName);

	m_curBGM = &m_SoundList[soundName.data()];
	Play(soundName.data(), true);
}

// isPlay�� true�� ������� �ƴҶ��� ���?
void SoundManager::Play(const string_view& soundName, const bool& isPlay) noexcept
{
	bool isPlaying = isPlay;
	if (isPlaying)
	{ 
		if (m_SoundList[soundName.data()].m_Channel != nullptr)
		{
			m_SoundList[soundName.data()].m_Channel->isPlaying(&isPlaying);
		}
	}
	if (!isPlaying)
	{
		m_SoundList[soundName.data()].SetVolume(m_masterVolume);
		m_pSystem->playSound(m_SoundList[soundName.data()].m_Sound, 0, false, &m_SoundList[soundName.data()].m_Channel);
	}
}

void SoundManager::PlayVariation(string&& soundName, const bool& isPlay, const int& vCount) noexcept
{
	soundName = soundName + (char)('1' + (rand() % vCount)) + ".mp3";
	Play(soundName, isPlay);
}

void SoundManager::PlayQueue(const char* soundName, const D3DXVECTOR3& position, const float& maxDistance) noexcept
{
	m_SoundQueue.emplace(soundName, position, maxDistance);
}

void SoundManager::Stop(const string_view& soundName) noexcept
{
	m_SoundList[soundName.data()].m_Channel->stop();
}

void SoundManager::Paused(const string_view& soundName) noexcept
{
	bool isPaused;
	m_SoundList[soundName.data()].m_Channel->getPaused(&isPaused);
	m_SoundList[soundName.data()].m_Channel->setPaused(!isPaused);
}

void SoundManager::OperVolume(const string_view& soundName, const float& value) noexcept
{
	m_SoundList[soundName.data()].OperVolume(value);
}

void SoundManager::SetMasterVolume(const float& value) noexcept
{
	m_masterVolume = value;
	/*for (auto& iter : m_SoundList)
	{
		iter.second.SetVolume(value);
	}*/
}