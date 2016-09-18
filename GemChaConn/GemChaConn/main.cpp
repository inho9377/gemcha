#include <clocale>
#include "Windows.h"
#include "iostream"
#include "sql.h"
#include "sqlext.h"
#include "TCHAR.h"
#include <string>
#include "Odbc.h"
#include <vector>
#include "UserInfo.h"

#pragma warning(disable:4996)



enum class SCENE {
	LOGIN_SCENE,
	JOIN_SCENE,
	MAIN_SCENE,
	RECOMMEND_SCENE,
	GAME_SELECT_SCENE,
	GAME_INFO_SCENE,
	INTERESTED_GAME_VIEW_SCENE
};



enum class GENRE {
	ACTION,
	RPG,
	HORROR,
	PUZZLE,
	TACTICAL,
	MUSIC
};

SCENE currentSCene = SCENE::LOGIN_SCENE;

int select_game_id;
std::string user_id;

bool passwordCheck(std::string id, std::string pw)
{
	std::vector<std::string> result;
	result = Odbc::GetInstance()->SelectPassword(id);

	if (result.empty())
		return false;

	if (((*result.begin()).c_str()) == pw)
		return true;
	else
		return false;
}

bool isLoginOK(std::string id, std::string pw)
{
	bool isIDExist;
	isIDExist = Odbc::GetInstance()->CheckDataExist("user_info", "user_id", id);
	
	
	if (!isIDExist)
		return false;

	bool isPwMatch = passwordCheck(id, pw);
	
	if (!isPwMatch)
		return false;


	return true;
}






void drawLoginSCene()
{
	int ret = 1;
	
	printf("input ID : 1, JOIN : 2 \n");
	scanf("%d", &ret);
	if (ret == 2)
	{
		currentSCene = SCENE::JOIN_SCENE;
		return;
	}

	else if (ret != 1)
	{
		printf("올바른 번호를 입력해주세요\n");
		return;
	}

	std::string id;
	std::string pw;
	printf("input ID : ");
	
	std::cin >> id;

	printf("input PW : ");
	std::cin >> pw;

	if (isLoginOK(id, pw))
	{
		currentSCene = SCENE::MAIN_SCENE;
		
		user_id = id;
		
		Odbc::GetInstance()->SetUserInfBoBySelect(user_id);
		return;
	}
	else
		printf("올바른 ID또는 비밀번호를 입력해주세요\n");



}

bool isLoginOK(std::string id)
{
	bool isIDExist;
	isIDExist = Odbc::GetInstance()->CheckDataExist("user_info", "user_id", id);

	return isIDExist;
}

void drawJoinScene()
{
	std::string id;
	printf("input id : ");
	std::cin >> id;
	
	if (isLoginOK(id))
	{
		printf("이미 존재하는 아이디입니다. 다시 입력해주십시오\n");
		return;
	}

	std::string pw;
	printf("input password : ");
	std::cin >> pw;

	bool ret = Odbc::GetInstance()->InsertDataUserInfo( id, pw);
	
	if (!ret)
	{
		printf("INSERT ERROR!\n");
		while (1);
		return;
	}

	bool ret2 = Odbc::GetInstance()->InsertDataUserRecommend(id);
	if (!ret2)
	{
		printf("INSERT ERROR!\n");
		while (1);
		return;
	}
	currentSCene = SCENE::LOGIN_SCENE;
}

void drawMainScene()
{
	
	printf("------SERVICE-------\n");
	printf("1. 게임 검색\n");
	printf("2. 게임 추천\n");
	printf("3. 관심 게임 보기\n");
	printf("4. 로그아웃\n\n\n");
	
	
	int ret;
	scanf("%d", &ret);
	
	switch (ret)
	{
	case 1:
		currentSCene = SCENE::GAME_SELECT_SCENE;
		break;
		
	case 2:
		currentSCene = SCENE::RECOMMEND_SCENE;
		break;

	case 3:
		currentSCene = SCENE::INTERESTED_GAME_VIEW_SCENE;
		break;
		
	case 4:
		CUserInfo::GetInstance()->_Inst = nullptr;
		currentSCene = SCENE::LOGIN_SCENE;
		break;
		
	default:
		break;
	}


}

void viewTop10Game()
{
	std::vector<std::string> search_result;

	search_result = Odbc::GetInstance()->SelectDataTop10();
	if (search_result.empty())
	{
		currentSCene = SCENE::MAIN_SCENE;
		printf("검색 결과가 없습니다.\n");
		return;
	}

	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");
	for (auto idx = search_result.begin(); idx != search_result.end(); idx++)
	{

		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("정보를 볼 게임의 ID 선택해주세요 : ");
	scanf("%d", &choose_id);

	select_game_id = choose_id;
	currentSCene = SCENE::GAME_INFO_SCENE;

}

void view_likeTop10Game()
{

	std::vector<std::string> search_result;
	search_result = Odbc::GetInstance()->SelectDataTop10(CUserInfo::GetInstance()->recommend_genre);
	if (search_result.empty())
	{
		currentSCene = SCENE::MAIN_SCENE;
		printf("검색 결과가 없습니다.\n");
		return;
	}

	
	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");
	for (auto idx = search_result.begin(); idx != search_result.end(); idx++)
	{

		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("정보를 볼 게임의 ID 선택해주세요 : ");
	scanf("%d", &choose_id);

	select_game_id = choose_id;
	currentSCene = SCENE::GAME_INFO_SCENE;





}


void drawRecommendScene()
{
	printf("------SERVICE-------\n");
	printf("1. top 10 게임\n");
	printf("2. 선호 장르 top 10 게임\n\n\n\n");


	
	int ret;
	scanf("%d", &ret);
	     
	switch (ret)
	{
	case 1:
		viewTop10Game();
		break;
	case 2:
		view_likeTop10Game();
		break;
	default:
		break;
	}


}




bool IsValueRangeOK(int value, int first, int last)
{
	if(value > last)
		return false;
	if(value < first)
		return false;
	
	return true;
}

void drawGameSelectScene()
{
	system("cls");
	printf("무엇으로 검색하시겠습니까?\n");
	printf("1. 장르 검색\n");
	printf("2. 제목 검색\n");
	printf("3. 메인으로이동\n\n");
	
	int ret=0;
	scanf("%d", &ret);
	while(!IsValueRangeOK(ret, 1,3))
	{
		printf("제대로된 값을 입력해주세요\n");
		scanf("%d", &ret);
	}
	
	if(ret==3)
	{
		currentSCene = SCENE::MAIN_SCENE;
		return;
	}

	std::string search_value;
	printf("검색값 : ");
	std::cin >> search_value;


	std::vector<std::string> search_result;

	if (ret == 2)
		search_result = Odbc::GetInstance()->SelectData("game_info", "*", true, "title", search_value, true);
	else
		search_result = Odbc::GetInstance()->SelectData("game_info", "*", true, "genre", search_value, true); 

	if (search_result.empty())
	{
		currentSCene = SCENE::MAIN_SCENE;
		printf("검색 결과가 없습니다.\n");
		return;
	}
	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");

	for (auto idx = search_result.begin(); idx != search_result.end();idx++)
	{
		
		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("정보를 볼 게임의 ID 선택해주세요 : ");
	scanf("%d", &choose_id);

	select_game_id = choose_id;
	currentSCene = SCENE::GAME_INFO_SCENE;
}

void ApplyStarPoint(int input_star_point)
{
	//transaction
	Odbc::GetInstance()->StartTranscation();

	bool insertCheck = Odbc::GetInstance()->InsertDataUserGame(user_id, select_game_id, input_star_point);

	if (!insertCheck)
	{
		printf("INSERT ERRROR!\n");
		Odbc::GetInstance()->Rollback();
		return;
	}



	///////////평균평점계산

	
	int result_avg = atoi(Odbc::GetInstance()->SelectavgStar(select_game_id).c_str());
	int result_numrated = atoi(Odbc::GetInstance()->SelectTotalNumRated(select_game_id).c_str());

	int avg = ((result_avg*result_numrated) + input_star_point / (result_numrated+1));



	insertCheck = Odbc::GetInstance()->UpdateDataGameInfo(select_game_id, avg);
	if (!insertCheck)
	{
		printf("Update ERRROR!\n");
		Odbc::GetInstance()->Rollback();
		return;
	}
		



	insertCheck = Odbc::GetInstance()->UpdateDataUserInfo(user_id);
	if (!insertCheck)
	{
		printf("Update ERRROR!\n");
		Odbc::GetInstance()->Rollback();
		return;
	}

	insertCheck = Odbc::GetInstance()->UpdateDataUserRecommend(user_id, input_star_point);
	if (!insertCheck)
	{
		printf("Update ERRROR!\n");
		Odbc::GetInstance()->Rollback();
		return;
	}

	Odbc::GetInstance()->Commit();
	//commit

}

void ApplyOpinion(std::string input_opinion)
{
	bool insertCheck = Odbc::GetInstance()->InsertOpinion(user_id, select_game_id, input_opinion);
	
	if (!insertCheck)
		printf("INSERT ERRROR!\n");


}

void drawGameInfoScene()
{
	//게임정보조회
	std::vector<std::string> ret_game_info;
	std::vector<std::string> ret_opinion_info;
	ret_game_info = Odbc::GetInstance()->SelectData("game_info", "*", true, "game_id", select_game_id);
	ret_opinion_info = Odbc::GetInstance()->SelectOpinion(select_game_id);

	
	for (auto idx = ret_game_info.begin(); idx != ret_game_info.end(); idx++)
	{
		printf("ID, TITLE, GENRE, CONSOLE, AVG_STR\n");
		
		printf("%s\n", (*idx).c_str());
	}
	printf("\n\n\n");

	printf("-opinion-\n");
	for (auto idx = ret_opinion_info.begin(); idx != ret_opinion_info.end(); idx++)
	{
		printf("%s\n", (*idx).c_str());
	}





	int ret=0;
	while(ret != 5)
	{
		printf("무엇을 하시겠습니까?\n");
		printf("1. 별점 하사\n");
		printf("2. 관심게임등록 \n");
		printf("3. 관심없어요 \n");
		printf("4. 의견게시 \n");
		printf("5. 메인으로 이동\n\n\n");
		scanf("%d", &ret);
		while(!IsValueRangeOK(ret, 1,5))
		{
			
			
			printf("제대로된 값을 입력해주세요\n");
			scanf("%d", &ret);
		}
		
		
		if(ret == 1)
		{
			int input_star_point = -1;
			printf("부여할 별점(0~10) : ");
			scanf("%d", &input_star_point);
			while(!IsValueRangeOK(input_star_point, 0, 10))
			{	
				printf("제대로된 값을 입력해주세요\n");
				scanf("%d", &input_star_point);
			}

			ApplyStarPoint(input_star_point);
		}
		else if (ret == 2)
		{
			//관심게임등록
			Odbc::GetInstance()->InsertDataInterestedGame(user_id, select_game_id, true);

			printf("관심게임으로 등록되었습니다!\n");
			currentSCene = SCENE::MAIN_SCENE;
		}
		else if (ret == 3)
		{
			Odbc::GetInstance()->InsertDataInterestedGame(user_id, select_game_id, false);

			printf("무관심게임으로 등록되었습니다.\n");
			currentSCene = SCENE::MAIN_SCENE;
		}
		else if (ret == 4)
		{
			std::string opinion;
			printf("한 줄로 게임에 대한 감상을 써주세요 : ");
			std::getline(std::cin,opinion);
			//std::cin >> opinion;
			ApplyOpinion(opinion);
			printf("의견 감사합니다!\n");
			currentSCene = SCENE::MAIN_SCENE;

		}
		system("cls");
	}
	
	
	currentSCene = SCENE::MAIN_SCENE;
	
}

void drawInterestedGameScene()
{
	std::vector<std::string> ret_vector;
	
	ret_vector = Odbc::GetInstance()->SelectDataInterested("Interested_Game", "game_id", true, "user_id", select_game_id);

	for (auto idx = ret_vector.begin(); idx != ret_vector.end(); idx++)
	{
		printf("%s\n", (*idx).c_str());
	}


	if (ret_vector.empty())
	{
		printf("검색 결과가 없습니다.\n");
		currentSCene = SCENE::MAIN_SCENE;
		return;
	}

	int choose_id;
	printf("정보를 볼 게임의 ID 선택해주세요 : ");
	scanf("%d", &choose_id);

	select_game_id = choose_id;
	currentSCene = SCENE::GAME_INFO_SCENE;
}

void drawScene()
{
	system("cls");
	
	switch (currentSCene)
	{
	case SCENE::LOGIN_SCENE:
		drawLoginSCene();
		break;
	case SCENE::JOIN_SCENE:
		drawJoinScene();
		break;
	case SCENE::MAIN_SCENE:
		drawMainScene();
		break;
	case SCENE::RECOMMEND_SCENE:
		drawRecommendScene();
		break;
	case SCENE::GAME_SELECT_SCENE:
		drawGameSelectScene();
		break;
	case SCENE::GAME_INFO_SCENE:
		drawGameInfoScene();
		break;
	case SCENE::INTERESTED_GAME_VIEW_SCENE:
		drawInterestedGameScene();
	default:
		break;
	}
}


int _tmain(int argc, _TCHAR* argv[]) {
	//SetConsoleCP(65001);
	//SetConsoleOutputCP(CP_UTF8);
	std::setlocale(LC_ALL, "");

	bool ret;
	ret = Odbc::GetInstance()->Connect(L"mysqlodbc", L"nextuser", L"a");

	if (ret)
		printf("connect complete!\n");
	else
		printf("ConnecError! \n");


	while (1)
	{
		drawScene();
	}

	

	


	while (1);
}
