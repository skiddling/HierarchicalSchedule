#include "pattern.h"
#include "classunit.h"
#include "group.h"

Pattern::Pattern() {
}

Pattern::Pattern(vector<Course> course_que, int stu_num) :
	course_que_(course_que), stu_num_(stu_num){
}

void Pattern::GetAllPath(vector<Group> table) {
	//dfs�����õ����е�·��
	vector<bool> tagque = vector<bool>(table.size(), false);
	for (int i = 0; i < course_que_.size(); i++) {
		
	}
}

void Pattern::DFS() {
	
}
