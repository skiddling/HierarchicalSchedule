#pragma once
#include "course.h"

class Teacher
{
public:
	int teacher_no;
	string teacher_id_;
	string teacher_name_;
	vector<Course> courses_;//һ����ʦ���ܻ��Ӧ�����ͬ���ֵĿ�Ŀ
	map<Course, int> courses_num_;//ÿһ����Ŀ�϶��ٸ���
	Teacher(string teacher_id, string teacher_name, vector<Course> courses, map<Course, int> courses_num);
private:

};
