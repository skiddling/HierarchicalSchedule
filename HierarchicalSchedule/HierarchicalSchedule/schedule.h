#pragma once
#include "timetable.h"

class Schedule
{
public:
	int rooms_, groups_;
	map<string, int> cou_id_map_;
	map<string, Course> cou_name_map_;
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	map<Pattern, int> pattern_map_;//记录所有学生的课程模式
	vector<Pattern> pattern_que_;
	vector<ClassUnit> cls_nuit_que_;//当前所有课形成的队列
	TimeTable time_table_;//当前由clsunitque形成的课表

	Schedule();
	Schedule(int rooms, int groups, map<string, int> cou_id_map, map<string, Course> cou_name_map,
		vector<Student> stu_que, vector<Teacher> tea_que);
	void GetTeaCls();//构造老师的课程指针
	void GetRanTab();

private:
	void MakeTabRand(vector<vector<int> > &table);
};