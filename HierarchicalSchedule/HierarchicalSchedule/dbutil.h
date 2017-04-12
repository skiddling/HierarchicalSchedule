#pragma once
#include "ga.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") 
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

	int rooms, groups, coursenum, stunum, teachernum, outtime;
	map<string, Course> courses;
	map<int, Course> coursesid;//用于数据库id查课程
	map<int, int> sturecord;//记录学生序号和id，已知id查序号用
	map<int, int> teacherrecord;//记录教师序号和id
	vector<string> roomtable;//记录班级和它的序号
	vector<Course> couque;
	vector<Student> stuque;
	vector<Teacher> teacherque;

	void GetDBInfo();
	void CutString(string& s);
	string SetDBProperties();
	void GetDataFromTable(void (DButil::*funcptr)(_RecordsetPtr& m_pRecordset), string tablename);
	void Get_T_SPKTask(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKStudent(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKStudentCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKTeacher(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKTeacherCourseConfig(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKClassRoom(_RecordsetPtr& m_pRecordset); 
};