#pragma once
#include "student.h"

//����һ��ģʽ�Ķ���
class Pattern
{
public:
	vector<Course> course_que_;
	map<Course, int> course_stus_;//ÿ����Ŀ���ж���ѧ��	
	Pattern();

	bool operator = (const Pattern &a)const {
		if (a.course_que_.size() != this->course_que_.size())return 0;
		for (int i = 0; i < a.course_que_.size(); i++) {
			if (this->course_stus_.find(a.course_que_[i]) == this->course_stus_.end())return 0;
		}
		return 1;
	}
};