#pragma once
#include "teacher.h"
#include "student.h"

class ClassRoom
{
public:
	bool alterable_;//�Ƿ���Ҫ�����ſΣ�1������Ҫ��0������Ҫ
	pair<int, int> class_time_;//��ʼ����ʱ�����������ҵľ�����Ͽ�ʱ��
	Teacher* teacher_;//ÿ���Ͽν��Ҷ�����һ����ʦ��û�о���NULL
	Course* course_;//ÿ���ϿεĽ��Ҷ�����һ���ϿεĿ�Ŀ��û�о���NULL
	vector<Student*> students_;//��������������Ѿ����ڵ�ѧ��
	bool could_in_;//�ܷ�������༶���ѧ��������ð༶��Ԥ���趨�ò���������Ӿ���0��������1
	int left_num_;//ʣ��༶��������������Ǹ������Ǳ�ʾ�������ٸ�
	ClassRoom();

private:

};
