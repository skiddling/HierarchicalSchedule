#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_que_(cou_que){

	prefixes_.push_back(*(new Prefix(groups_)));
	InitSort();

	//1.先把每一节课初始化，然后生成课表
	//老师队列已经有了，但是每个老师的课还不存在，所以要生成每个老师的课
	result_ = *(new Schedule(rooms_, groups_, cou_que_, stu_que_, tea_que_));
	schedules_ = vector<Schedule>(kScheduleSize_, result_);
	result_.GetTeaCls();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].GetTeaCls();
	}

	//2.获得所有学生的模式类型
	GetStuPat();

	//3.对每个对象都随机产生一个课表
	GetRandTab();

}

//从所有学生当中获得所有的模式，并且统计出所有的模式各有多少的学生
void GA::GetStuPat() {
	for (int i = 0; i < stu_que_.size(); i++) {
		if (patterns_map_.find(stu_que_[i].courses_) == patterns_map_.end()) {
			patterns_map_[stu_que_[i].courses_] = *(new Pattern(stu_que_[i].courses_));
			patterns_.push_back(patterns_map_[stu_que_[i].courses_]);
		}
		else patterns_map_[stu_que_[i].courses_].stu_num_++;
		stu_que_[i].patp = &patterns_map_[stu_que_[i].courses_];
	}
	//对得到的所有的模式进行排序，以便于进行获得前缀的操作
	sort(patterns_.begin(), patterns_.end());
}

void GA::GetPrefixes() {
	for (int i = 0; i < patterns_.size(); i++) {
		vector<Course> cque;
		int len = patterns_[i].course_que_.size(), iid, oid;
		for (int j = 0; j < len; j++) {
			cque.push_back(patterns_[i].course_que_[j]);
			if (prefix_map_.find(cque) == prefix_map_.end()) {
				prefixes_.push_back(*(new Prefix(cque)));
				prefix_map_[cque] = prefixes_.size() - 1;
				if (cque.size() == 1) {
					prefixes_.back().pre_id_ = 0;
				}
			}
		}
		for (int j = 1; j < len; j++) {
			iid = cque[j - 1].course_id_;
			oid = cque[j].course_id_;
			//cout << iid << ' ' << oid << endl;
			/*if (iid == 3 && oid == 4) {
				for (int k = 0; k < patterns_[i].course_que_.size(); k++) {
					cout << patterns_[i].course_que_[k].course_name_ << "\t";
				}
				cout << endl;
			}*/
			if (!cou_que_[oid].pre_node_[iid]) {
				cou_que_[oid].pre_node_[iid] = 1;
				cou_que_[oid].pre_node_que_.push_back(&cou_que_[iid]);
			}
		}
	}
	//还要对所有前缀根据各自的前缀长度进行排序，也就是科目的前缀
	cout << "end of get prefix" << endl;
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

//每个对象都随机生成一个课表
void GA::GetRandTab() {
	//
	GetPrefixes();
		
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].prefixes_ = prefixes_;
		schedules_[i].prefix_map_ = prefix_map_;
		int ts = clock(), te, tag = 0;
		while (schedules_[i].GetRanTab()) {
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