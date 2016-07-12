#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_id_map_(cou_id_map), cou_name_map_(cou_name_map){
	for (int i = 0; i < stu_que_.size(); i++) {
		stu_que_[i].GetSort();
	}
	GetStuPat();
}

void GA::GetStuPat() {
		
}