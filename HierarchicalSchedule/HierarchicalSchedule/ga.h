#pragma once
#include "schedule.h"

class GA
{
public:
	const int kScheduleSize_ = 50;//定义种群数量
	int rooms_, groups_;
	map<string, int> cou_id_map_;
	map<string, Course> cou_name_map_;
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	Schedule result_;//最后结果
	vector<Schedule> schedules_;//每个schedule代表一个个体
	GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map);

	void Generate();//生成课表
	void InitSort();//给老师和学生的科目进行排序

private:
	void GetStuPat();//获得当前所有学生的模式情况
};