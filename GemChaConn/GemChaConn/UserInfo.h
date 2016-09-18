#pragma once
#include<string>

class CUserInfo
{
public:
	static CUserInfo* _Inst;
	static CUserInfo* CUserInfo::GetInstance();

	int m_starnum_horror = 0;
	int m_starnum_action = 0;
	int m_starnum_rpg = 0;
	int m_starnum_indi = 0;
	int m_starnum_music = 0;
	int m_starnum_puzzle = 0;
	int m_starnum_tatical = 0;
	int m_starnum_fps = 0;
	int m_starnum_adventure = 0;
	void SetStarnum(std::string genre, int input_starnum);
	void SetRecommendGenre(); 
	std::string recommend_genre;

private:
	CUserInfo();
	~CUserInfo();

	int id;
	int intMax(int a, int b);
	


};

