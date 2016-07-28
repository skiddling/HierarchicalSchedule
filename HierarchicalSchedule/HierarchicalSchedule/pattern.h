#pragma once
#include "student.h"

class ClassUnit;
class Group;

//单独一个模式的定义
//是进行学生分配的主题对象，要进行分学生的遗传算法的交叉变异等操作
class Pattern
{
public:
	int pattern_id_;//该模式的id
	int stu_num_;//该模式下所有的学生人数
	vector<Course> course_que_;
	vector<vector<ClassUnit*> > avl_units_que_;//该课表下的所有的可以放该模式的班级序列
	//map<ClassUnit*, bool> units_map_;//所有能存放课的表
	map<ClassUnit*, vector<int> > not_in_table_;//不放在某个课的所有其他可能的课的序列
	map<ClassUnit*, vector<int> > in_unit_table_;//存在于某个班级的所有的序列
	map<int, int> stu_num_in_que_;//每种序列当中有多少人
	vector<vector<ClassUnit* > > path_;//在课表当中能够实现该模式的所有的路径
	set<ClassUnit* > unit_set_;//记录所有可能经历过的教室
	//map<pair<int, int>, vector<int> > not_in_table_;//没有经历过的路径形成的表

	Pattern();
	Pattern(vector<Course> course_que, int stu_num = 1);
	void GetAllPath(vector<Group> table);
	void GetNotInTable();
	void StuAssign();

	bool operator == (const Pattern &a)const {
		return course_que_ == a.course_que_;
	}

	bool operator < (const Pattern &a)const {
		return course_que_ < a.course_que_;
	}

private:
	void DFS(int gid, vector<bool> visited, vector<ClassUnit* > path, vector<Group> table);
};