#pragma once
//#include "course.h"
#include "interruptflag.h"

class Prefix
{
public:
	int pre_id_;//��ǰ׺�ı�ţ���1��ʼ
	//bool satisfied_;//�������Ƿ�����
	vector<Course> pattern_;//��ǰ׺��ģʽ
	//����ģʽ���п��Լ����õ�ʱ��Σ���һ��ֻ��0��1�����ִ���1��ʾ�����ã�0��ʾ������
	vector<int> avl_time_;
	//ÿ����������е��еĿ��õ�ʱ���
	//map<vector<int>, int> avl_map;//ÿ���������е�ʱ���,value��ָ��eachvaltime���е��±�
	map<vector<int>, bool> avl_tab_;//���и����еľ���·��,1��ʾ���ã�0��ʾ�Ѿ���ռ����
	Prefix();
	Prefix(int rows);
	Prefix(vector<Course> pattern, int pre_id, int rows);

private:

};