#pragma once
#include "teacher.h"

class Pattern;
enum Sex {
	male, female
};

class Student
{
public:
	Sex sex_;//ѧ���Ա�
	int student_no;//ѧ����ţ��Լ���
	string student_id_;//ѧ���������,��ֹ������ͬ
	string student_name_;
	vector<Course> courses_;//Ҫ����Щ��Ŀ

	//Pattern *patp;//ָ���ѧ�����Ǹ�ģʽ
	int patp_;//��ѧ�������ĸ�ģʽ
	map<Course, pair<int, int> > time_table_;//���յ�ѧ���Ŀα�
	Student();
	Student(string student_id, string student_name, vector<Course> courses, string sex);
	void GetCouSort();//��courses����������

private:

};