#pragma once
#include "course.h"

class Prefix
{
public:
	int pre_id_;//��ǰ׺��ǰ׺��,һ����Ŀ��ǰ׺�þ���0
	vector<Course> pre_pat_;//ģʽ��ǰ׺
	vector<int> avl_time_;//����ģʽ���Լ����õ�ʱ��Σ���һ��ֻ��0��1�����ִ���1��ʾ�����ã�0��ʾ������
	vector<int> used_time_;//�Ѿ�ռ�õ�ʱ������ʾ��avl�෴
	Prefix();
	Prefix(int rows);
	Prefix(vector<Course> pre_pat);

private:

};