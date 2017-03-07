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
	int dva_male_num_;//男生人数偏差
	int dva_female_num_;
	double dva_avg_points_;//班级平均分差距
	double tot_avg_points_;//这个科目的总的平均分
	//double points_;//科目分数
	int stu_upper_;//具体每个学科的班级人数上限
	int stu_lower_;//具体每个学科的班级人数下限
	int class_num_;//每个科目下面有多少个班级
	bool visited_;//用于拓扑排序时候用的，每一个科目相当于是一个结点
	int course_id_;//将在被全部创建完科目之后才会生成相应的id
	set<int> not_in_set_;//不能排课的时间段
	string course_name_;
	vector<Course* > pre_node_que_;//也是一样用于进行拓扑排序的，标注了该科目前的所有的前面一个科目
	vector<bool> pre_node_;//用来标记哪些科目是前缀，用于拓扑排序，并且防止重复

	int satis_num_;//满足的前缀数量
	vector<bool> satisfied_;//标识该科目的每个前缀是否在当前的序列当中被满足
	set<int> prefix_set_;//用来判断该前缀是否已经存在了
	vector<int> prefixes_;//该科目的所有的前缀序列
	vector<ClassUnit* > units_;//该科目下的所有的的课次
	//vector<int> units_;//该科目下的所有的课的节次

	Course();
	Course(string course_name, int cls, int cnum, double dvainpoint, int stuupper, int stulower, set<int>notintset, bool visited = 0);
	//Course(int course_id, string course_name);
	bool operator < (const Course a)const {
		//同一个科目下的学科判班级的数量，不同学科的就判名字
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