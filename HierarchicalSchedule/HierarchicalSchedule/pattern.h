#pragma once
#include "student.h"

class ClassUnit;
class Group;

//����һ��ģʽ�Ķ���
//�ǽ���ѧ��������������Ҫ���з�ѧ�����Ŵ��㷨�Ľ������Ȳ���
class Pattern
{
public:
	int pattern_id_;//��ģʽ��id
	int stu_num_;//��ģʽ�����е�ѧ������
	vector<Course> course_que_;
	vector<vector<ClassUnit*> > avl_units_que_;//�ÿα��µ����еĿ��ԷŸ�ģʽ�İ༶����
	//map<ClassUnit*, bool> units_map_;//�����ܴ�ſεı�
	map<ClassUnit*, vector<int> > not_in_table_;//������ĳ���ε������������ܵĿε�����
	map<ClassUnit*, vector<int> > in_unit_table_;//������ĳ���༶�����е�����
	map<int, int> stu_num_in_que_;//ÿ�����е����ж�����
	vector<vector<ClassUnit* > > path_;//�ڿα����ܹ�ʵ�ָ�ģʽ�����е�·��
	set<ClassUnit* > unit_set_;//��¼���п��ܾ������Ľ���
	//map<pair<int, int>, vector<int> > not_in_table_;//û�о�������·���γɵı�

	Pattern();
	Pattern(vector<Course> course_que, int stu_num = 1);
	void GetAllPath(vector<Group> table);
	void GetNotInTable();
	void StuAssign();

	bool operator == (const Pattern &a)const {
		return course_que_ == a.course_que_;
	}

	bool operator < (const Pattern &a)const {
		return course_que_ < a.course_que_;
	}

private:
	void DFS(int gid, vector<bool> visited, vector<ClassUnit* > path, vector<Group> table);
};