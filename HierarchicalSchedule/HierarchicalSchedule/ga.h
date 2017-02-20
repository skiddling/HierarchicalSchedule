#pragma once
//#include "schedule.h"
#include "InterruptibleThread.h"

class GA
{
public:
	static const int kScheduleSize_ = 20;//定义种群数量
	//const int kCheckTimeOut = 1000000;//生成一张具有可行性的课表的时间限制
	static int rooms_, groups_;
	static double step;
	int cnt_;//cnt用于拓扑排序
	double mxfit_;//存一轮当中最大的适应度
	static int stu_upper_, stu_lower_;//一个班级的学生人数的上限
	vector<Course> cou_que_;//用于存放所有的科目
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	Schedule result_;//最后结果
	vector<Schedule> schedules_[2];//每个schedule代表一个个体


	map<vector<Course>, int> patterns_map_;//所有模式的总表，用于模式查询
	vector<Pattern> patterns_;//所有的选课模式序列，需要进行排序

	map<vector<Course>, int> prefix_map_;//用于和prefix_联用,数字指向vector<Prefix>下标
	vector<Prefix> prefixes_;//所有的前缀模式

	vector<double> fits;
	//vector<ClassUnit> cls_units_;//所有的课

	vector<int> topo_sorted_;//将科目进行拓扑排序之后的科目的顺序

	GA(vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que_);
	bool Generate();//生成课表
	void OutPutResult();
	void GetResult();

private:
	void InitSort();//给老师和学生的科目进行排序
	void GetStuPat();//获得当前所有学生的模式情况
	void GetPrefixes();
	void DFS(int k);//拓扑排序当中的一部分
	void TopoSort();//对所有的科目进行拓扑排序
	void OutPutTable();
	void Mutate();
	void Cross();
	void Modify();
	void Select();
	void CalCrash();
	bool Init();
};