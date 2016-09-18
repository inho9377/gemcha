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
		printf("�ùٸ� ��ȣ�� �Է����ּ���\n");
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
		printf("�ùٸ� ID�Ǵ� ��й�ȣ�� �Է����ּ���\n");



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
		printf("�̹� �����ϴ� ���̵��Դϴ�. �ٽ� �Է����ֽʽÿ�\n");
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
	printf("1. ���� �˻�\n");
	printf("2. ���� ��õ\n");
	printf("3. ���� ���� ����\n");
	printf("4. �α׾ƿ�\n\n\n");
	
	
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
		printf("�˻� ����� �����ϴ�.\n");
		return;
	}

	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");
	for (auto idx = search_result.begin(); idx != search_result.end(); idx++)
	{

		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("������ �� ������ ID �������ּ��� : ");
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
		printf("�˻� ����� �����ϴ�.\n");
		return;
	}

	
	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");
	for (auto idx = search_result.begin(); idx != search_result.end(); idx++)
	{

		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("������ �� ������ ID �������ּ��� : ");
	scanf("%d", &choose_id);

	select_game_id = choose_id;
	currentSCene = SCENE::GAME_INFO_SCENE;





}


void drawRecommendScene()
{
	printf("------SERVICE-------\n");
	printf("1. top 10 ����\n");
	printf("2. ��ȣ �帣 top 10 ����\n\n\n\n");


	
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
	printf("�������� �˻��Ͻðڽ��ϱ�?\n");
	printf("1. �帣 �˻�\n");
	printf("2. ���� �˻�\n");
	printf("3. ���������̵�\n\n");
	
	int ret=0;
	scanf("%d", &ret);
	while(!IsValueRangeOK(ret, 1,3))
	{
		printf("����ε� ���� �Է����ּ���\n");
		scanf("%d", &ret);
	}
	
	if(ret==3)
	{
		currentSCene = SCENE::MAIN_SCENE;
		return;
	}

	std::string search_value;
	printf("�˻��� : ");
	std::cin >> search_value;


	std::vector<std::string> search_result;

	if (ret == 2)
		search_result = Odbc::GetInstance()->SelectData("game_info", "*", true, "title", search_value, true);
	else
		search_result = Odbc::GetInstance()->SelectData("game_info", "*", true, "genre", search_value, true); 

	if (search_result.empty())
	{
		currentSCene = SCENE::MAIN_SCENE;
		printf("�˻� ����� �����ϴ�.\n");
		return;
	}
	printf("====================================================\n");
	printf("|game_id|title|genre|console|avg_starpoint|num_rated| \n");

	for (auto idx = search_result.begin(); idx != search_result.end();idx++)
	{
		
		printf("%s\n", (*idx).c_str());
	}

	int choose_id;
	printf("������ �� ������ ID �������ּ��� : ");
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



	///////////����������

	
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
	//����������ȸ
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
		printf("������ �Ͻðڽ��ϱ�?\n");
		printf("1. ���� �ϻ�\n");
		printf("2. ���ɰ��ӵ�� \n");
		printf("3. ���ɾ���� \n");
		printf("4. �ǰ߰Խ� \n");
		printf("5. �������� �̵�\n\n\n");
		scanf("%d", &ret);
		while(!IsValueRangeOK(ret, 1,5))
		{
			
			
			printf("����ε� ���� �Է����ּ���\n");
			scanf("%d", &ret);
		}
		
		
		if(ret == 1)
		{
			int input_star_point = -1;
			printf("�ο��� ����(0~10) : ");
			scanf("%d", &input_star_point);
			while(!IsValueRangeOK(input_star_point, 0, 10))
			{	
				printf("����ε� ���� �Է����ּ���\n");
				scanf("%d", &input_star_point);
			}

			ApplyStarPoint(input_star_point);
		}
		else if (ret == 2)
		{
			//���ɰ��ӵ��
			Odbc::GetInstance()->InsertDataInterestedGame(user_id, select_game_id, true);

			printf("���ɰ������� ��ϵǾ����ϴ�!\n");
			currentSCene = SCENE::MAIN_SCENE;
		}
		else if (ret == 3)
		{
			Odbc::GetInstance()->InsertDataInterestedGame(user_id, select_game_id, false);

			printf("�����ɰ������� ��ϵǾ����ϴ�.\n");
			currentSCene = SCENE::MAIN_SCENE;
		}
		else if (ret == 4)
		{
			std::string opinion;
			printf("�� �ٷ� ���ӿ� ���� ������ ���ּ��� : ");
			std::getline(std::cin,opinion);
			//std::cin >> opinion;
			ApplyOpinion(opinion);
			printf("�ǰ� �����մϴ�!\n");
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
		printf("�˻� ����� �����ϴ�.\n");
		currentSCene = SCENE::MAIN_SCENE;
		return;
	}

	int choose_id;
	printf("������ �� ������ ID �������ּ��� : ");
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
