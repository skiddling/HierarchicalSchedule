#pragma once
#include "ga.h"

//typedef void (DButil::*funcp)();

class DButil
{
public:
	DButil();
	string StartPk(string pktaskid);
private:
	string pktaskid_;
	string statement_;
	string db_server_;
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string db_port_;

	void GetDBInfo();
	void CutString(string& s);
	//void GetDataFromTable(funcp);
	void GetDataFromTable(void (DButil::*)());
	
};