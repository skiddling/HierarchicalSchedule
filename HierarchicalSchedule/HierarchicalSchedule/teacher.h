#pragma once
#include "prefix.h"

class ClassUnit;

class Teacher
{
public:
	int teacher_no;
	string teacher_id_;
	string teacher_name_;
	vector<Course> courses_;//һ����ʦ���ܻ��Ӧ�����ͬ���ֵĿ�Ŀ
	map<Course, int> courses_num_;//ÿһ����Ŀ�϶��ٸ���
	vector<ClassUnit *> units_que_;//����ÿ����ʦ�ϵİ༶
	Teacher();
	Teacher(string teacher_id, string teacher_name, vector<Course> courses, map<Course, int> courses_num);
	void GetCouSort();
	void AssignUnits(vector<int> &randgroups, vector<vector<int> > &randrooms, vector<int> &roompos);//��ÿһ���ζ����з���

private:
	
};
