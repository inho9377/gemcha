#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <clocale>
#include <codecvt>
#include "sql.h"
#include "sqlext.h"
#include <string>
#include <vector>
#include "UserInfo.h"

class Odbc
{
public:

	enum class TableName
	{
		USER_INFO,
		GAME_INFO,
		USER_GAME,
		OPINION,
		INTERESTED_GAME
	};

	static Odbc* _Inst;
	static Odbc* Odbc::GetInstance();
	
	bool			IsConnect(){ return _IsConnect; }

	bool			Connect(wchar_t* odbcName, wchar_t* mysqlId, wchar_t* password);
	void			Disonnect();

	int				GetUserId(std::string userName);
	std::string		GetPassword(std::string name);


	
	bool			InsertData(std::string tableName, std::string colNames, std::string value, bool isValueString);
	
	bool			InsertDataUserGame(std::string user_id, int game_id, int star_point);
	bool			InsertDataInterestedGame(std::string user_id, int game_id, bool LikeORHate);
	bool			InsertOpinion(std::string user_id, int game_id, std::string input_opinion);
	bool			InsertDataUserInfo(std::string user_id, std::string pw);
	bool			InsertDataUserRecommend(std::string uer_id);
	bool			CheckDataExist(std::string tableName, std::string colName, std::string value);

	std::vector<std::string>		SelectData(std::string tableName, std::string colNames, bool whereCon = false, std::string whereCol = "id", std::string value = "", bool isLike = false);
	std::vector<std::string>		SelectData(std::string tableName, std::string colNames, bool whereCon = false, std::string whereCol = "id", int value = 0);
	std::vector<std::string>		SelectDataInterested(std::string tableName, std::string colNames, bool whereCon = false, std::string whereCol = "id", int id = 0);
	std::vector<std::string>		SelectDataTop10();
	std::vector<std::string>		SelectDataTop10(std::string genre);
	std::vector<std::string>		SelectOpinion(int game_id);
	std::string		SelectDataGenre(int game_id);
	std::string		SelectTotalNumRated(int game_id);
	std::string		SelectavgStar(int game_id);
	std::vector<std::string>		SelectPassword(std::string id);

	bool			PushQuery(std::wstring query);
	
	void			ReadFileAndInsert(const char *path);
	void			CheckSuccess(int ret);
	bool			UpdateDataGameInfo(int game_id, int avg_starpoint);
	bool			UpdateDataUserInfo(std::string user_id);
	
	const char*		CreateCSV(const char* filename, int num);
	void			SelectBookData();
	
	std::wstring	utf8_to_wstring(const std::string& str);
	std::string		wstring_to_utf8(const std::wstring& str);
	
	void			StartTranscation();
	void			Commit();
	void			Rollback();

	void			SetUserInfBoBySelect(std::string user_id);
	bool			UpdateDataUserRecommend(std::string user_id, int starpoint);

private:
	Odbc();
	~Odbc();

	bool		_IsConnect = false;
	SQLHENV		_hEnv;
	SQLHDBC		_hDbc;
	SQLHSTMT	_hStmt;
};