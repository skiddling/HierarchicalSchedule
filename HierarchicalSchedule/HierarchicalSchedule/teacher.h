#pragma once
#include "prefix.h"

class ClassUnit;

class Teacher
{
public:
	int teacherdbid_;//���ݿ⵱�е�id
	//string teacher_no_;
	int teacher_id_;//��������
	string teacher_name_;
	vector<Course> courses_;//һ����ʦ���ܻ��Ӧ�����ͬ���ֵĿ�Ŀ
	map<Course, int> courses_num_;//ÿһ����Ŀ�϶��ٸ���
	vector<ClassUnit *> units_que_;//����ÿ����ʦ�ϵİ༶
	vector<int> units_pos_;//����ָ��unit�ڶ��е��е�λ��
	vector<bool> avl_time_;//��ʦ���õ�ʱ�䣬1��ʾ�����ã�0��ʾ�Ѿ��õ���

	Teacher();
	//Teacher(int groups, string teacher_no, string teacher_name, vector<Course> courses, map<Course, int> courses_num);
	Teacher(int groups, string teacher_name, vector<Course> courses, map<Course, int> courses_num);
	Teacher(int groups, int teacherdbid, string teacher_name);
	void GetCouSort();
	void AssignUnits(vector<int> &randgroups, vector<vector<int> > &randrooms, vector<int> &roompos);//��ÿһ���ζ����з���

private:
	
};
