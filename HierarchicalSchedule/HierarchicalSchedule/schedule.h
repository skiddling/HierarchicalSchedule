#pragma once
#include "group.h"

class Schedule
{
public:
	static const double mx_pcross_;
	static const double mx_pmutate_;
	static const double mx_pmutate_gene_;
	static const double con_pmutate ;
	static const double con_pmutate_gene_;
	static const double con_pcross_;
	static const int kTableTimeOut;//生成一张具有可行性的课表的时间限制

	double fitness;//适应值
	static double po_mutate_;//具体运行时候的变异概率
	static double po_mutate_gene_;//具体运行时基因变异概率
	static double po_cross_;//具体运行时候的交叉概率
	static int stu_upper_, stu_lower_;//学生班级人数上限
	int crash_;//冲突值，用来进行评分
	int success_falg_;
	static int rooms_, groups_;

	vector<Course> cou_que_;//用于存放所有的科目
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;

	map<vector<Course>, int> pattern_map_;//记录所有学生的课程模式
	vector<Pattern> pattern_que_;

	vector<ClassUnit> cls_nuit_que_;//当前所有课形成的队列
	vector<Group> table_;//课表

	map<vector<Course>, int> prefix_map_;//用于和prefix_联用
	vector<Prefix> prefixes_;//所有的前缀模式

	vector<int> topo_sorted_;//科目的拓扑排序表

	Schedule();
	Schedule(vector<Course> cou_que, vector<Student> stu_que, vector<Teacher> tea_que, 
		map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, 
		map<vector<Course>, int> prefix_map, vector<Prefix> prefixes, vector<int> topo_sorted);
	void Init();//生成课表
	void GetAllPath();//获得所有模式的上课路径
	void StuAssign();//初始化分配学生人数
	void CalCrashFitness();//计算冲突值
	void Mutate(double mxfit);
	void Cross(double mxfit);
	void Modify();
	void GetResult();
	void OutPutResult();
	void ResetStuNum();

private:
	void GetTeaCls();//构造老师的课程指针
	//产生课表
	bool GetRanTab();
	//获得某个科目的可安排时间
	vector<vector<int> > GetAvlTime(int cid);
	int GetUnitTime(int cid, int uid, vector<vector<int> > avl);
	void AssignUnit(int gid, ClassUnit *cup);
	bool GetTimeList(int tag, map<GroupUnit, vector<int> > &timelist, vector<vector<int> > avl, vector<bool> trtimes);
	bool CheckPrefix(int cid);
	void GetStuNum();
};