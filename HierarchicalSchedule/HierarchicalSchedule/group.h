#pragma once
#include "classunit.h"

class Group 
{
public:
	int gid_;
	int rooms_;
	int cpos_;
	int leave_;//ʣ�¶��ٽ���
	bool avl;//�����Ƿ���Ȼ���п���Ľ����ܹ��������ſγ�
	vector<ClassUnit *> group;
	Group();
	Group(int rooms, int leave);

private:
	//void GetRandTable(int rows, int cols);

};