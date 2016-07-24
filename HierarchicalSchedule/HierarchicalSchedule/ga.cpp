#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_que_(cou_que){

	topo_sorted_ = vector<int>(cou_que_.size());
	prefixes_.push_back(*(new Prefix(groups_)));
	InitSort();

	//1.获得所有学生的模式类型
	GetStuPat();
	GetPrefixes();
	//因为已经对科目排序过了，所以不需要再用拓扑排序进行排序
	//TopoSort();

	//公共的内容已经完成，剩下就是对每个schedule进行生成
	//2.先把每一节课初始化，然后生成课表
	//老师队列已经有了，但是每个老师的课还不存在，所以要生成每个老师的课
	result_ = *(new Schedule(rooms_, groups_, cou_que_, stu_que_, tea_que_, patterns_map_, patterns_, prefix_map_, prefixes_, topo_sorted_));
	schedules_ = vector<Schedule>(kScheduleSize_, result_);
	result_.Init();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].Init();
	}
	cout << "end of the get rand table" << endl;
	OutPutTable();
}

//从所有学生当中获得所有的模式，并且统计出所有的模式各有多少的学生
void GA::GetStuPat() {
	for (int i = 0; i < stu_que_.size(); i++) {
		if (patterns_map_.find(stu_que_[i].courses_) == patterns_map_.end()) {
			//patterns_map_[stu_que_[i].courses_] = *(new Pattern(stu_que_[i].courses_));
			//patterns_.push_back(patterns_map_[stu_que_[i].courses_]);
			patterns_.push_back(*(new Pattern(stu_que_[i].courses_)));
			patterns_map_[stu_que_[i].courses_] = patterns_.size() - 1;
		}
		else patterns_[patterns_map_[stu_que_[i].courses_]].stu_num_++;
		stu_que_[i].patp_ = patterns_map_[stu_que_[i].courses_];
	}
	//对得到的所有的模式进行排序，以便于进行获得前缀的操作
	sort(patterns_.begin(), patterns_.end());
}

void GA::GetPrefixes() {
	for (int i = 0; i < patterns_.size(); i++) {
		vector<Course> cque;
		int len = patterns_[i].course_que_.size(), iid, oid, cid;
		for (int j = 0; j < len - 1; j++) {
			cque.push_back(patterns_[i].course_que_[j]);
			cid = patterns_[i].course_que_[j + 1].course_id_;
			if (prefix_map_.find(cque) == prefix_map_.end()) {
				prefixes_.push_back(*(new Prefix(cque, prefixes_.size(), groups_)));
				prefix_map_[cque] = prefixes_.size() - 1;
				/*if (cque.size() == 1) {
					prefixes_.back().pre_id_ = 0;
				}*/
			}
			//获得每个科目的所有前缀
			if (cou_que_[cid].prefix_set_.find(prefix_map_[cque]) == cou_que_[cid].prefix_set_.end()) {
				cou_que_[cid].prefixes_.push_back(prefix_map_[cque]);
				cou_que_[cid].prefix_set_.insert(prefix_map_[cque]);
			}
		}
		for (int j = 1; j < len; j++) {
			//iid = cque[j - 1].course_id_;
			//oid = cque[j].course_id_;
			iid = patterns_[i].course_que_[j - 1].course_id_;
			oid = patterns_[i].course_que_[j].course_id_;
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
	for (int i = 0; i < cou_que_.size(); i++) {
		if (!cou_que_[i].prefixes_.size()) {
			cou_que_[i].prefixes_.push_back(0);
		}
		cou_que_[i].satisfied = vector<bool>(cou_que_[i].prefixes_.size(), 0);
	}
	//将这些科目的信息都更新到每个老师当中去
	int cid;
	for (int i = 0; i < tea_que_.size(); i++) {
		for (int j = 0; j < tea_que_[i].courses_.size(); j++) {
			cid = tea_que_[i].courses_[j].course_id_;
			tea_que_[i].courses_[j] = cou_que_[cid];
		}
	}
	//还要对所有前缀根据各自的前缀长度进行排序，也就是科目的前缀
	cout << "end of get prefix" << endl;
}

//对学生和老师的科目进行排序
void GA::InitSort() {
	//sort(cou_que_.begin(), cou_que_.end());
	for (int i = 0; i < tea_que_.size(); i++) {
		tea_que_[i].GetCouSort();
	}
	for (int i = 0; i < stu_que_.size(); i++) {
		stu_que_[i].GetCouSort();
	}
}

void GA::DFS(int k) {
	cou_que_[k].visited_ = 1;
	for (int i = 0; i < cou_que_[k].pre_node_que_.size(); i++) {
		if (!cou_que_[k].pre_node_que_[i]->visited_)
			DFS(cou_que_[k].pre_node_que_[i]->course_id_);
	}
	topo_sorted_[cnt_++] = k;
}

void GA::TopoSort() {
	for (int i = 0; i < cou_que_.size(); i++) {
		if (!cou_que_[i].visited_) {
			DFS(i);
		}
	}
	/*for (int i = 0; i < topo_sorted_.size(); i++) {
		cout << topo_sorted_[i] << " ";
	}
	cout << endl;*/
}

void GA::OutPutTable() {
	ofstream fout("randtable.txt");
	for (int k = 0; k < kScheduleSize_; k++) {
		for (int i = 0; i < groups_; i++) {
			for (int j = 0; j < rooms_; j++) {
				fout << schedules_[k].table_[i].group[j]->course_.course_name_ << "  " << schedules_[k].table_[i].group[j]->teacher_.teacher_name_<< "     ";
			}
			fout << endl;
		}
		fout << endl << endl << endl;
	}
	fout.close();
}

bool GA::Generate() {
	int errors = 0;
	for (int i = 0; i < kScheduleSize_; i++) {
		errors += schedules_[i].success_falg_;
	}
	if (errors == kScheduleSize_)return 1;
	//开始正式分配学生
	return 0;
}