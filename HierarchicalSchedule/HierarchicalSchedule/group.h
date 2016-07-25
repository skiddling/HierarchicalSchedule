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
	map<Course, vector<int> > cou_set_;//����Ŀ��γɵ�һ���б�
	Group();
	Group(int rooms, int leave, bool avl = 1);
	void AddUnit(ClassUnit *up);
	inline void GetCouSet();

private:
	//void GetRandTable(int rows, int cols);

};

inline void Group::GetCouSet() {
	Course cou;
	for (int i = 0; i < group.size(); i++) {
		cou = group[i]->course_;
		cou_set_[cou].push_back(i);
	}
}