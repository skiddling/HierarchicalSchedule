#pragma once
#include "student.h"

class ClassUnit;

//单独一个模式的定义
//是进行学生分配的主题对象，要进行分学生的遗传算法的交叉变异等操作
class Pattern
{
public:
	int pattern_id_;//该模式的id
	int stu_num_;//该模式下所有的学生人数
	vector<Course> course_que_;
	vector<vector<ClassUnit*> > avl_units_que_;//该课表下的所有的可以放该模式的班级序列
	map<ClassUnit*, bool> units_map_;//所有能存放课的表
	map<ClassUnit*, vector<int> > not_in_unit_table_;//不放在某个课的所有其他可能的课的序列
	map<ClassUnit*, vector<int> > in_unit_table_;//存在于某个班级的所有的序列
	map<int, int> stu_num_in_que_;//每种序列当中有多少人

	Pattern();
	Pattern(vector<Course> course_que, int stu_num = 1);

	bool operator == (const Pattern &a)const {
		return course_que_ == a.course_que_;
	}

	bool operator < (const Pattern &a)const {
		return course_que_ < a.course_que_;
	}
};