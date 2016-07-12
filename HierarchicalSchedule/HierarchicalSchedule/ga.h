#pragma once
#include "summarytable.h"

class GA
{
public:
	int rooms_, groups_;
	map<string, int> cou_id_map_;
	map<string, Course> cou_name_map_;
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	SummaryTable stu_pat_smr_;//ѧ��ģʽ����
	GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map);

private:
	void GetStuPat();//��õ�ǰ����ѧ����ģʽ���
};