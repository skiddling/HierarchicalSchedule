#include "teacher.h"
#include "classunit.h"

Teacher::Teacher() {
}

Teacher::Teacher(int groups, string teacher_no, string teacher_name, vector<Course> courses, map<Course, int> courses_num) :
	teacher_no_(teacher_no), teacher_name_(teacher_name), courses_(courses), courses_num_(courses_num){
	avl_time_ = vector<bool>(groups, 1);
}

void Teacher::GetCouSort() {
	sort(this->courses_.begin(), this->courses_.end());
}

void Teacher::AssignUnits(vector<int> &randgroups, vector<vector<int> > &randrooms, vector<int> &roompos) {
	//对每一门科目的每一节课都进行分配
	int g, r;
	for (int i = 0; i < units_que_.size(); i++) {
		g = randgroups[i];
		r = randrooms[g][roompos[g]];
		units_que_[i]->unit_time_ = make_pair(g, r);
		roompos[g]++;
	}
}