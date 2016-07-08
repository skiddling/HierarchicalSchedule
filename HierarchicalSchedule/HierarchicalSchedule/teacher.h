#pragma once
#include "course.h"

class Teacher
{
public:
	int teacher_id_;
	string teacher_name_;
	vector<Course> courses_;//һ����ʦ���ܻ��Ӧ�����ͬ���ֵĿ�Ŀ
	map<Course, vector<int> > course_table_;//��¼ÿ����Ŀ�϶��ٴο�
	Teacher(int teacher_id, string teacher_name, vector<Course> courses, map<Course, vector<int> > course_table);
private:

};
