#pragma once
#include "course.h"

class Prefix
{
public:
	int pre_id_;//��ǰ׺��ǰ׺��,һ����Ŀ��ǰ׺�þ���0
	vector<Course> pre_pat_;//ģʽ��ǰ׺
	vector<int> avl_time_;//����ģʽ���Լ����õ�ʱ���
	vector<int> used_time_;//�Ѿ�ռ�õ�ʱ��
	Prefix();
	Prefix(int rows);
	Prefix(vector<Course> pre_pat);

private:

};