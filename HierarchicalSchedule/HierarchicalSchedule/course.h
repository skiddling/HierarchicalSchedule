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

//class Prefix;
//class ClassUnit;

class Course
{
public:
	bool visited_;//������������ʱ���õģ�ÿһ����Ŀ�൱����һ�����
	int course_id_;//���ڱ�ȫ���������Ŀ֮��Ż�������Ӧ��id
	string course_name_;
	vector<Course* > pre_node_que_;//Ҳ��һ�����ڽ�����������ģ���ע�˸ÿ�Ŀǰ�����е�ǰ��һ����Ŀ
	vector<bool> pre_node_;//���������Щ��Ŀ��ǰ׺�������������򣬲��ҷ�ֹ�ظ�

	set<int> prefix_set_;//�����жϸ�ǰ׺�Ƿ��Ѿ�������
	vector<int> prefixes_;//�ÿ�Ŀ�����е�ǰ׺����
	//vector<ClassUnit* > units_;//�ÿ�Ŀ�µ����еĵĿδ�
	vector<int> units_;//�ÿ�Ŀ�µ����еĿεĽڴ�

	Course();
	Course(string course_name, int cnum, bool visited = 0);
	//Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		return this->course_name_ < a.course_name_;
	}
	bool operator == (const Course &a)const {
		return this->course_name_ == a.course_name_;
	}
};