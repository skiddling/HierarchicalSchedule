#pragma once
#include "student.h"

class ClassUnit
{
public:
	int unit_id_;
	bool alterable_;//是否需要进行排课，1代表需要，0代表不需要,不需要安排的班级时间和具体上课学生都被固定
	pair<int, int> unit_time_;//初始化的时候分配这个教室的具体的上课时间
	Teacher* teacher_;//每个上课教室都会有一个老师，没有就是NULL
	Course* course_;//每个上课的教室都会有一个上课的科目，没有就是NULL
	vector<Student *> students_;//用来保存这个班已经存在的学生
	map<int, bool> patterns_;//用来存当前班级有多少个patterns
	map<int, int> patterns_stus_;//每个patterns有多少个学生
	ClassUnit(int unit_id);
	ClassUnit(int unit_id, pair<int, int> unit_time);
	void init();
private:

};
