#pragma once
#include "timetable.h"

class Schedule
{
public:
	int rooms_, groups_;
	//map<string, bool> cou_id_map_;
	//map<string, Course> cou_name_map_;
	//set<Course> cou_set_;//用于存放所有的科目的情况
	vector<Course> cou_que_;//用于存放所有的科目
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;

	map<vector<Course>, int> pattern_map_;//记录所有学生的课程模式
	vector<Pattern> pattern_que_;

	vector<ClassUnit> cls_nuit_que_;//当前所有课形成的队列
	TimeTable time_table_;//当前由clsunitque形成的课表

	map<vector<Course>, int> prefix_map_;//用于和prefix_联用
	vector<Prefix> prefixes_;//所有的前缀模式

	Schedule();
	Schedule(int rooms, int groups, vector<Course> cou_que, vector<Student> stu_que, 
		vector<Teacher> tea_que, map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, 
		map<vector<Course>, int> prefix_map, vector<Prefix> prefixes);
	void GetTeaCls();//构造老师的课程指针
	bool GetRanTab();

private:
	void MakeTabRand(vector<vector<int> > &table);
};