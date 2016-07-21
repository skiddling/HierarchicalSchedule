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
class ClassUnit;

class Course
{
public:
	int class_num_;//ÿ����Ŀ�����ж��ٸ��༶
	bool visited_;//������������ʱ���õģ�ÿһ����Ŀ�൱����һ�����
	int course_id_;//���ڱ�ȫ���������Ŀ֮��Ż�������Ӧ��id
	string course_name_;
	vector<Course* > pre_node_que_;//Ҳ��һ�����ڽ�����������ģ���ע�˸ÿ�Ŀǰ�����е�ǰ��һ����Ŀ
	vector<bool> pre_node_;//���������Щ��Ŀ��ǰ׺�������������򣬲��ҷ�ֹ�ظ�

	int satis_num_;//�����ǰ׺����
	vector<bool> satisfied;//��ʾ��Щǰ׺�Ѿ���������
	set<int> prefix_set_;//�����жϸ�ǰ׺�Ƿ��Ѿ�������
	vector<int> prefixes_;//�ÿ�Ŀ�����е�ǰ׺����
	vector<ClassUnit* > units_;//�ÿ�Ŀ�µ����еĵĿδ�
	//vector<int> units_;//�ÿ�Ŀ�µ����еĿεĽڴ�

	Course();
	Course(string course_name, int cls, int cnum, bool visited = 0);
	//Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		//ͬһ����Ŀ�µ�ѧ���а༶����������ͬѧ�Ƶľ�������
		if (course_name_.substr(0, course_name_.length() - 1) != a.course_name_.substr(0, a.course_name_.length() - 1))
			return this->course_name_ < a.course_name_;
		else return class_num_ < a.class_num_;
	}
	bool operator == (const Course &a)const {
		return this->course_name_ == a.course_name_;
	}
};