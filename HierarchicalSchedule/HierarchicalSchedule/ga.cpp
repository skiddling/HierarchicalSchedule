#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_id_map_(cou_id_map), cou_name_map_(cou_name_map){
	InitSort();
	//1.先把每一节课初始化，然后生成课表
	//老师队列已经有了，但是每个老师的课还不存在，所以要生成每个老师的课
	result_ = *(new Schedule(rooms_, groups_, cou_id_map_, cou_name_map_, stu_que_, tea_que_));
	schedules_ = vector<Schedule>(kScheduleSize_, result_);
	result_.GetTeaCls();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].GetTeaCls();
	}
	//2.对每个对象都随机产生一个课表

	GetStuPat();
}

void GA::GetStuPat() {
		
}

//对学生和老师的科目进行排序
void GA::InitSort() {
	for (int i = 0; i < tea_que_.size(); i++) {
		tea_que_[i].GetCouSort();
	}
	for (int i = 0; i < stu_que_.size(); i++) {
		stu_que_[i].GetCouSort();
	}
}

void GA::Generate() {
	
}