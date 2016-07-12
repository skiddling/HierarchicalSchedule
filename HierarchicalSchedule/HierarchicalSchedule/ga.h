#pragma once
#include "summarytable.h"

class GA
{
public:
	int rooms_, groups_;
	map<string, int> cou_id_map_;
	map<string, Course> cou_name_map_;
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	SummaryTable stu_pat_smr_;//学生模式汇总
	GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map);

private:
	void GetStuPat();//获得当前所有学生的模式情况
};