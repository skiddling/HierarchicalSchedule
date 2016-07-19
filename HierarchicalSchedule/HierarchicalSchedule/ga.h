#pragma once
#include "schedule.h"

class GA
{
public:
	const int kScheduleSize_ = 50;//定义种群数量
	const int kCheckTimeOut = 1000000;//生成一张具有可行性的课表的时间限制
	int rooms_, groups_;
	//map<string, Course> cou_name_map_;//用于存放所有的科目
	//set<Course> cou_set_;//所有的科目的存放
	vector<Course> cou_que_;//用于存放所有的科目
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	Schedule result_;//最后结果
	vector<Schedule> schedules_;//每个schedule代表一个个体
	map<vector<Course>, Pattern> patterns_map_;//所有模式的总表，用于模式查询
	vector<Pattern> patterns_;//所有的选课模式序列，需要进行排序
	map<vector<Course>, int> prefix_map_;//用于和prefix_联用
	vector<Prefix> prefixes_;//所有的前缀模式
	vector<Course> topo_sorted_;//将科目进行拓扑排序之后的科目的顺序

	GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que_);
	void Generate();//生成课表
	void InitSort();//给老师和学生的科目进行排序

private:
	void GetStuPat();//获得当前所有学生的模式情况
	void GetPrefixes();
	void GetRandTab();//每个个体都随机生成一个总表
};