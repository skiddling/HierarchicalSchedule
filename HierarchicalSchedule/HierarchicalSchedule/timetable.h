#pragma once
#include "classunit.h"

class TimeTable
{
public:
	static vector<Course> courses_;//���еĿ�Ŀ,���еĿα���һ����Ŀ����
	vector<Teacher> teachers_;//���е���ʦ,ÿ���α�һ����������ʦ����
	vector<ClassUnit> class_units_que_;//��¼���пΣ��γ�һ�����У�ÿ���α����ӵ����ôһ������
	vector<vector<ClassUnit *> > time_table_;//�ܿα�
	TimeTable(int rows, int cols, vector<Teacher> teachers, vector<ClassUnit> class_units_que);

private:
	void GetRandTable(int rows, int cols);

};