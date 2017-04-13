#pragma once
#include "ga.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") 
//typedef void (DButil::*funcp)();

class DButil
{
public:
	int rooms, groups;
	int outtime;//运行时间
	vector<Course> couque;
	vector<Student> stuque;
	vector<Teacher> teacherque;

	//DButil();
	string StartPk(string pktaskid);
	string StartPk();
	void OutPutResult();

	Schedule result_;//最后结果
	
private:
	string pktaskid_;
	string statement_;
	string db_server_;
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string db_port_;

	int coursenum, stunum, teachernum;// , outtime;
	map<string, Course> courses;
	map<int, Course> coursesid;//用于数据库id查课程
	map<int, int> sturecord;//记录学生序号和id，已知id查序号用
	map<int, int> teacherrecord;//记录教师序号和id
	vector<string> roomtable;//记录班级和它的序号
	map<int, int> roomno2id;//教序号和教室数据库id进行对应
	vector<string> grouptable;//记录数据库当中读出来的组名
	map<int, int> groupno2id;//组名和数据库组id对应
	

	void GetDBInfo();
	void CutString(string& s);
	string SetDBProperties();
	void GetDataFromTable(void (DButil::*funcptr)(_RecordsetPtr& m_pRecordset), string tablename);
	void Get_T_SPKTask(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKClassRoom(_RecordsetPtr& m_pRecordset); 
	void Get_T_SPKGroup(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKStudent(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKStudentCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKTeacher(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKTeacherCourseConfig(_RecordsetPtr& m_pRecordset);
	void Post_T_SPKStudentCourseJXB(_RecordsetPtr& m_pRecordset);
	void Post_T_SPKTeacherCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_SPKTeacherCourse(_RecordsetPtr& m_pRecordset);
};