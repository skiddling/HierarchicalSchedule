#pragma once
#include "teacher.h"

class Pattern;

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

	//new obj or method for new version
	map<Course, double> points_;//��¼ѧ��ÿ����Ŀ�ĳɼ�
	set<ClassUnit*> clsset_;//��¼��ѧ������Щ��ѧ���Ͽ�
	void IntoCls(ClassUnit* clsptr);
	void OutCls(ClassUnit* clsptr);
	double GetCouPoints(ClassUnit* cls);
	
private:

};