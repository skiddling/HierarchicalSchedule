#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

const double kRndPluRnd = static_cast<double>(RAND_MAX * RAND_MAX);

class Prefix;
class ClassUnit;

class Course
{
public:
	int course_id_;
	string course_name_;
	vector<Prefix* > prefixes_;//该科目的前缀序列
	vector<ClassUnit* > units_;//该科目下的所有的的课次

	Course();
	Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		return this->course_name_ < a.course_name_;
	}
	bool operator == (const Course &a)const {
		return this->course_name_ == a.course_name_;
	}
};