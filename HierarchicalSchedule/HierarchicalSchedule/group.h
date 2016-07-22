#pragma once
#include "classunit.h"

class Group 
{
public:
	int gid_;
	int rooms_;
	int cpos_;
	int leave_;//剩下多少教室
	bool avl;//该组是否仍然还有空余的教室能够用来安排课程
	vector<ClassUnit *> group;
	Group();
	Group(int rooms, int leave);

private:
	//void GetRandTable(int rows, int cols);

};