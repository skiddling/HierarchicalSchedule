#include "teacher.h"

Teacher::Teacher() {
}

Teacher::Teacher(string teacher_id, string teacher_name, vector<Course> courses, map<Course, int> courses_num) :
	teacher_id_(teacher_id), teacher_name_(teacher_name), courses_(courses), courses_num_(courses_num){
}

void Teacher::GetCouSort() {
	sort(this->courses_.begin(), this->courses_.end());
}

void Teacher::AssignUnits(vector<int> &randgroups, vector<vector<int> > &randrooms, vector<int> &roompos) {
	int g, r;
	for (int i = 0; i < units_que_.size(); i++) {
		g = randgroups[i];
		r = randrooms[g][roompos[g]];
		//units_que_[i]->unit_time = make_pair(g, r);
		//units_que_[i]->unit_time_ = make_pair(g, r);
	}
}