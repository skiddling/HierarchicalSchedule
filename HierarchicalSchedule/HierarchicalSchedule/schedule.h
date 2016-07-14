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
	map<Pattern, int> pattern_map_;//��¼����ѧ���Ŀγ�ģʽ
	vector<Pattern> pattern_que_;
	vector<ClassUnit> cls_nuit_que_;//��ǰ���п��γɵĶ���
	TimeTable time_table_;//��ǰ��clsunitque�γɵĿα�

	Schedule();
	Schedule(int rooms, int groups, map<string, int> cou_id_map, map<string, Course> cou_name_map,
		vector<Student> stu_que, vector<Teacher> tea_que);
	void GetTeaCls();//������ʦ�Ŀγ�ָ��
	void GetRanTab();

private:
	void MakeTabRand(vector<vector<int> > &table);
};