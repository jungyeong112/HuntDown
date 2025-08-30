#pragma once
class CFlagManager
{
public:
	CFlagManager();
	~CFlagManager();


public:
	static CFlagManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CFlagManager();
		}
		return m_pInstance;
	}
	static void Destory_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	bool Get_Doorbreaching() { return m_bDoorbreaching; }
	void Set_Doorbreaching() { m_bDoorbreaching = true; }

	bool Get_Stage2Flag() { return m_bStage2Flag; }
	void Set_Stage2Flag() { m_bStage2Flag = true; }
private:
	bool m_bDoorbreaching{ false };
	bool m_bStage2Flag{ false };
	static CFlagManager* m_pInstance;
};

