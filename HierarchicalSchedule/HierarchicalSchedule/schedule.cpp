#include "schedule.h"

Schedule::Schedule(int rooms, int groups, map<string, int> cou_id_map, map<string, Course> cou_name_map,
	vector<Student> stu_que, vector<Teacher> tea_que):
	rooms_(rooms), groups_(groups), cou_id_map_(cou_id_map), cou_name_map_(cou_name_map){
}

//对每个老师的每一节课都进行创建
void Schedule::GetTeaCls() {
	map<Course, int> ::iterator it;
	for (int i = 0; i < tea_que_.size(); i++) {
		it = tea_que_[i].courses_num_.begin();
		while (it != tea_que_[i].courses_num_.end()) {
			//单独一门课所有的班级都产生
			for (int j = 0; j < it->second; j++) {
				cls_nuit_que_.push_back = *(new ClassUnit(tea_que_[i], it->first, cls_nuit_que_.size()));
				tea_que_[i].units_que_.push_back(&(*(cls_nuit_que_.end() - 1)));
			}
			//进行下一门科目的课的产生
			it++;
		}
	}
}

void Schedule::MakeTabRand(vector<vector<int> > &table) {
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			table[i][j] = j;
		}
	}
	int t;
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			t = rand() % table[i].size();
			if (t != j)
				swap(table[i][j], table[i][t]);
		}
	}
}

void Schedule::GetRanTab() {
	vector<vector<int> > randgroups = vector<vector<int> >(tea_que_.size(), vector<int>(groups_));
	MakeTabRand(randgroups);
	vector<vector<int> > randrooms = vector<vector<int> >(groups_, vector<int>(rooms_));
	MakeTabRand(randrooms);
	vector<int> roompos = vector<int> (groups_, 0);

	//将老师的每节课都进行分配
	for (int i = 0; i < tea_que_.size(); i++) {
		tea_que_[i].AssignUnits(randgroups[i], randrooms, roompos);
	}
}