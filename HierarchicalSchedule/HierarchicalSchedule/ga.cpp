#include "ga.h"

GA::GA(int rooms, int groups, vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que) :
	rooms_(rooms), groups_(groups), stu_que_(stu_que), tea_que_(tea_que), cou_que_(cou_que){

	topo_sorted_ = vector<int>(cou_que_.size());
	prefixes_.push_back(*(new Prefix(groups_)));
	InitSort();

	//1.�������ѧ����ģʽ����
	GetStuPat();
	GetPrefixes();
	//��Ϊ�Ѿ��Կ�Ŀ������ˣ����Բ���Ҫ�������������������
	//TopoSort();

	//�����������Ѿ���ɣ�ʣ�¾��Ƕ�ÿ��schedule��������
	//2.�Ȱ�ÿһ�ڿγ�ʼ����Ȼ�����ɿα�
	//��ʦ�����Ѿ����ˣ�����ÿ����ʦ�Ŀλ������ڣ�����Ҫ����ÿ����ʦ�Ŀ�
	result_ = *(new Schedule(rooms_, groups_, cou_que_, stu_que_, tea_que_, patterns_map_, patterns_, prefix_map_, prefixes_, topo_sorted_));
	schedules_ = vector<Schedule>(kScheduleSize_, result_);
	result_.Init();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[i].Init();
	}
	cout << "end of the get rand table" << endl;
	OutPutTable();
}

//������ѧ�����л�����е�ģʽ������ͳ�Ƴ����е�ģʽ���ж��ٵ�ѧ��
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
	//�Եõ������е�ģʽ���������Ա��ڽ��л��ǰ׺�Ĳ���
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
			//���ÿ����Ŀ������ǰ׺
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
	//����Щ��Ŀ����Ϣ�����µ�ÿ����ʦ����ȥ
	int cid;
	for (int i = 0; i < tea_que_.size(); i++) {
		for (int j = 0; j < tea_que_[i].courses_.size(); j++) {
			cid = tea_que_[i].courses_[j].course_id_;
			tea_que_[i].courses_[j] = cou_que_[cid];
		}
	}
	//��Ҫ������ǰ׺���ݸ��Ե�ǰ׺���Ƚ�������Ҳ���ǿ�Ŀ��ǰ׺
	cout << "end of get prefix" << endl;
}

//��ѧ������ʦ�Ŀ�Ŀ��������
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
	//��ʼ��ʽ����ѧ��
	return 0;
}