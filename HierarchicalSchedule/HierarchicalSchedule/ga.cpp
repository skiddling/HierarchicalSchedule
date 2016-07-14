#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, map<string, int> cou_id_map, map<string, Course> cou_name_map) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_id_map_(cou_id_map), cou_name_map_(cou_name_map){
	InitSort();

	//1.�Ȱ�ÿһ�ڿγ�ʼ����Ȼ�����ɿα�
	//��ʦ�����Ѿ����ˣ�����ÿ����ʦ�Ŀλ������ڣ�����Ҫ����ÿ����ʦ�Ŀ�
	result_ = *(new Schedule(rooms_, groups_, cou_id_map_, cou_name_map_, stu_que_, tea_que_));
	schedules_ = vector<Schedule>(kScheduleSize_, result_);
	result_.GetTeaCls();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].GetTeaCls();
	}

	//2.�������ѧ����ģʽ����
	GetStuPat();

	//3.��ÿ�������������һ���α�
	GetRandTab();

}

//������ѧ�����л�����е�ģʽ������ͳ�Ƴ����е�ģʽ���ж��ٵ�ѧ��
void GA::GetStuPat() {
	for (int i = 0; i < stu_que_.size(); i++) {
		if (patterns.find(stu_que_[i].courses_) == patterns.end()) {
			patterns[stu_que_[i].courses_] = *(new Pattern(stu_que_[i].courses_));
		}
		else patterns[stu_que_[i].courses_].stu_num_++;
		stu_que_[i].patp = &patterns[stu_que_[i].courses_];
	}
}

//��ѧ������ʦ�Ŀ�Ŀ��������
void GA::InitSort() {
	for (int i = 0; i < tea_que_.size(); i++) {
		tea_que_[i].GetCouSort();
	}
	for (int i = 0; i < stu_que_.size(); i++) {
		stu_que_[i].GetCouSort();
	}
}

//���α��Ƿ����Ҫ��
bool GA::CheckTabAvl(TimeTable timetable) {
	
}

//ÿ�������������һ���α�
void GA::GetRandTab() {
	for (int i = 0; i < kScheduleSize_; i++) {
		int ts = clock(), te, tag = 0;
		while (1) {
			schedules_[i].GetRanTab();
			if(CheckTabAvl(schedules_[i].time_table_))break;
			te = clock();
			if (te - ts > kCheckTimeOut) {
				tag = 1;
				break;
			}
		}
		if (tag) {
			
		}
	}
}

void GA::Generate() {
	
}