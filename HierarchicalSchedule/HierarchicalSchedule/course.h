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
	bool visited_;//用于拓扑排序时候用的，每一个科目相当于是一个结点
	int course_id_;//将在被全部创建完科目之后才会生成相应的id
	string course_name_;
	vector<Course* > pre_node_;//也是一样用于进行拓扑排序的，标注了该科目前的所有的前面一个科目
	vector<Prefix* > prefixes_;//该科目的所有的前缀序列
	vector<ClassUnit* > units_;//该科目下的所有的的课次

	Course();
	Course(string course_name, bool visited = 0);
	//Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		return this->course_name_ < a.course_name_;
	}
	bool operator == (const Course &a)const {
		return this->course_name_ == a.course_name_;
	}
};