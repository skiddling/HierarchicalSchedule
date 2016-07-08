#pragma once
#include "student.h"

class ClassUnit
{
public:
	int unit_id_;
	bool alterable_;//�Ƿ���Ҫ�����ſΣ�1������Ҫ��0������Ҫ,����Ҫ���ŵİ༶ʱ��;����Ͽ�ѧ�������̶�
	pair<int, int> unit_time_;//��ʼ����ʱ�����������ҵľ�����Ͽ�ʱ��
	Teacher* teacher_;//ÿ���Ͽν��Ҷ�����һ����ʦ��û�о���NULL
	Course* course_;//ÿ���ϿεĽ��Ҷ�����һ���ϿεĿ�Ŀ��û�о���NULL
	vector<Student *> students_;//��������������Ѿ����ڵ�ѧ��
	map<int, bool> patterns_;//�����浱ǰ�༶�ж��ٸ�patterns
	map<int, int> patterns_stus_;//ÿ��patterns�ж��ٸ�ѧ��
	ClassUnit(int unit_id);
	ClassUnit(int unit_id, pair<int, int> unit_time);
	void init();
private:

};
