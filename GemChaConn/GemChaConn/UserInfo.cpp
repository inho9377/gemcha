#include "UserInfo.h"



using namespace std;

CUserInfo* CUserInfo::_Inst = nullptr;

CUserInfo::CUserInfo()
{}

CUserInfo::~CUserInfo()
{
	_Inst = nullptr;
}

CUserInfo* CUserInfo::GetInstance()
{
	if (_Inst == nullptr)
		_Inst = new CUserInfo();
	else
		return _Inst;
}

void CUserInfo::SetStarnum(std::string genre, int input_starnum)
{
	if (genre == "HORROR")
		m_starnum_horror += input_starnum;
	else if (genre == "ACTION")
		m_starnum_action += input_starnum;
	else if (genre == "RPG")
		m_starnum_rpg += input_starnum;
	else if (genre == "INDI")
		m_starnum_indi += input_starnum;
	else if (genre == "MUSIC")
		m_starnum_music += input_starnum;
	else if (genre == "TACTICAL")
		m_starnum_tatical += input_starnum;
	else if (genre == "PUZZLE")
		m_starnum_puzzle += input_starnum;
	else if (genre == "FPS")
		m_starnum_fps += input_starnum;
	else if (genre == "ADVENTURE")
		m_starnum_adventure += input_starnum;
	
	
	SetRecommendGenre();

}

int CUserInfo::intMax(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

void CUserInfo::SetRecommendGenre()
{
	int result_max;
	result_max = intMax(m_starnum_action, m_starnum_horror);
	result_max = intMax(result_max, m_starnum_indi);
	result_max = intMax(result_max, m_starnum_music);
	result_max = intMax(result_max, m_starnum_rpg);
	result_max = intMax(result_max, m_starnum_tatical);
	result_max = intMax(result_max, m_starnum_puzzle);

	if (result_max == m_starnum_action)
		recommend_genre = "ACTION";
	if (result_max == m_starnum_horror)
		recommend_genre = "HORROR";
	if (result_max == m_starnum_rpg)
		recommend_genre = "RPG";
	if (result_max == m_starnum_indi)
		recommend_genre = "INDI";
	if (result_max == m_starnum_music)
		recommend_genre = "MUSIC";
	if (result_max == m_starnum_tatical)
		recommend_genre = "TACTICAL";
	if (result_max == m_starnum_puzzle)
		recommend_genre = "PUZZLE";
	if (result_max == m_starnum_fps)
		recommend_genre = "FPS";
	if (result_max == m_starnum_adventure)
		recommend_genre = "ADVENTURE";

}