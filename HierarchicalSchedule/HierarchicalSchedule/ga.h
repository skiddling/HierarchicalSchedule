#pragma once
#include "schedule.h"

class GA
{
public:
	const int kScheduleSize_ = 50;//������Ⱥ����
	int rooms_, groups_;
	map<string, int> cou_id_map_;
	map<string, Course> cou_name_map_;
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	Schedule result_;//�����
	vector<Schedule> schedules_;//ÿ��schedule����һ������
	GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map);

	void Generate();//���ɿα�
	void InitSort();//����ʦ��ѧ���Ŀ�Ŀ��������

private:
	void GetStuPat();//��õ�ǰ����ѧ����ģʽ���
};