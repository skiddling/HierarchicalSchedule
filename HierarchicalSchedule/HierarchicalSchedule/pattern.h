#pragma once
#include "student.h"

class ClassUnit;

//����һ��ģʽ�Ķ���
//�ǽ���ѧ��������������Ҫ���з�ѧ�����Ŵ��㷨�Ľ������Ȳ���
class Pattern
{
public:
	int pattern_id_;//��ģʽ��id
	int stu_num_;//��ģʽ�����е�ѧ������
	vector<Course> course_que_;
	vector<vector<ClassUnit*> > avl_units_que_;//�ÿα��µ����еĿ��ԷŸ�ģʽ�İ༶����
	map<ClassUnit*, bool> units_map_;//�����ܴ�ſεı�
	map<ClassUnit*, vector<int> > not_in_unit_table_;//������ĳ���ε������������ܵĿε�����
	map<ClassUnit*, vector<int> > in_unit_table_;//������ĳ���༶�����е�����
	map<int, int> stu_num_in_que_;//ÿ�����е����ж�����

	Pattern();
	Pattern(vector<Course> course_que, int stu_num = 1);

	bool operator == (const Pattern &a)const {
		return course_que_ == a.course_que_;
	}

	bool operator < (const Pattern &a)const {
		return course_que_ < a.course_que_;
	}
};