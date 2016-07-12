#pragma once
#include "student.h"

//单独一个模式的定义
class Pattern
{
public:
	vector<Course> course_que_;
	map<Course, int> course_stus_;//每个科目下有多少学生	
	Pattern();

	bool operator = (const Pattern &a)const {
		if (a.course_que_.size() != this->course_que_.size())return 0;
		for (int i = 0; i < a.course_que_.size(); i++) {
			if (this->course_stus_.find(a.course_que_[i]) == this->course_stus_.end())return 0;
		}
		return 1;
	}
};