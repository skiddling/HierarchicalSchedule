#pragma once
#include "teacher.h"
#include "student.h"

class ClassRoom
{
public:
	bool alterable_;//是否需要进行排课，1代表需要，0代表不需要
	pair<int, int> class_time_;//初始化的时候分配这个教室的具体的上课时间
	Teacher* teacher_;//每个上课教室都会有一个老师，没有就是NULL
	Course* course_;//每个上课的教室都会有一个上课的科目，没有就是NULL
	vector<Student*> students_;//用来保存这个班已经存在的学生
	bool could_in_;//能否往这个班级添加学生，如果该班级是预先设定好不允许再添加就是0，否则是1
	int left_num_;//剩余班级人数数量，如果是负数就是表示超出多少个
	ClassRoom();

private:

};
