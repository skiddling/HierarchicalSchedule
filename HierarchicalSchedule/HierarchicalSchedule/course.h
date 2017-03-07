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
#include <chrono>
#include <random>
#include <functional>
#include <thread>
#include <future>
#include <mutex>
#include <functional>
#include "gtest\gtest.h"

using namespace std;

const double kRndPluRnd = static_cast<double>(RAND_MAX * RAND_MAX);

enum Sex {
	male, female
};

class GroupUnit {
public:
	int times_;
	int leave_;
	GroupUnit(int times = 0, int leave = 0) : times_(times), leave_(leave) {
	}
	bool operator < (const GroupUnit &a) const {
		if (times_ != a.times_)return times_ > a.times_;
		else return leave_ > a.leave_;
		//return times_ > a.times_;
		/*if (times_ != a.times_ && (abs(leave_ - a.leave_) <= 2))return times_ > a.times_;
		else return leave_ > a.leave_;*/
	}
	
	bool operator == (const GroupUnit &a) const {
		if (times_ == a.times_ && leave_ == a.leave_)return 1;
		return 0;
	}

};

//class Prefix;
class ClassUnit;

class Course
{
public:
	map<Sex, int> num_of_stus_in_sex_;
	map<Sex, double> sex_upper_;
	map<Sex, double> sex_lower_;
	map<Sex, double> total_scores_in_sex_;
	map<Sex, double> avg_scores_in_sex_;
	int dva_male_num_;//��������ƫ��
	int dva_female_num_;
	double dva_avg_points_;//�༶ƽ���ֲ��
	double tot_avg_points_;//�����Ŀ���ܵ�ƽ����
	//double points_;//��Ŀ����
	int stu_upper_;//����ÿ��ѧ�Ƶİ༶��������
	int stu_lower_;//����ÿ��ѧ�Ƶİ༶��������
	int class_num_;//ÿ����Ŀ�����ж��ٸ��༶
	bool visited_;//������������ʱ���õģ�ÿһ����Ŀ�൱����һ�����
	int course_id_;//���ڱ�ȫ���������Ŀ֮��Ż�������Ӧ��id
	set<int> not_in_set_;//�����ſε�ʱ���
	string course_name_;
	vector<Course* > pre_node_que_;//Ҳ��һ�����ڽ�����������ģ���ע�˸ÿ�Ŀǰ�����е�ǰ��һ����Ŀ
	vector<bool> pre_node_;//���������Щ��Ŀ��ǰ׺�������������򣬲��ҷ�ֹ�ظ�

	int satis_num_;//�����ǰ׺����
	vector<bool> satisfied_;//��ʶ�ÿ�Ŀ��ÿ��ǰ׺�Ƿ��ڵ�ǰ�����е��б�����
	set<int> prefix_set_;//�����жϸ�ǰ׺�Ƿ��Ѿ�������
	vector<int> prefixes_;//�ÿ�Ŀ�����е�ǰ׺����
	vector<ClassUnit* > units_;//�ÿ�Ŀ�µ����еĵĿδ�
	//vector<int> units_;//�ÿ�Ŀ�µ����еĿεĽڴ�

	Course();
	Course(string course_name, int cls, int cnum, double dvainpoint, int stuupper, int stulower, set<int>notintset, bool visited = 0);
	//Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		//ͬһ����Ŀ�µ�ѧ���а༶����������ͬѧ�Ƶľ�������
		if (course_name_.substr(0, course_name_.length() - 1) != a.course_name_.substr(0, a.course_name_.length() - 1))
			return this->course_name_ < a.course_name_;
		else if (not_in_set_.size() != a.not_in_set_.size())return not_in_set_.size() > a.not_in_set_.size();
		//else if(class_num_ != a.class_num_)return class_num_ < a.class_num_;
		else return course_name_ < a.course_name_;
	}
	bool operator == (const Course &a)const {
		return this->course_name_ == a.course_name_;
	}

	bool operator !=(const Course &a)const {
		return course_name_ != a.course_name_;
	}

	//new method for new version
	void GetSexUpLow();
};