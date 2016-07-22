#include "schedule.h"

Schedule::Schedule() {
}

Schedule::Schedule(int rooms, int groups, vector<Course> cou_que, vector<Student> stu_que,
	vector<Teacher> tea_que, map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que,
	map<vector<Course>, int> prefix_map, vector<Prefix> prefixes, vector<int> topo_sorted):
	rooms_(rooms), groups_(groups), cou_que_(cou_que), stu_que_(stu_que), tea_que_(tea_que), pattern_map_(pattern_map), 
	pattern_que_(pattern_que), prefix_map_(prefix_map), prefixes_(prefixes), topo_sorted_(topo_sorted){
	table_ = vector<Group>(groups_, *(new Group(rooms_, rooms_)));
}

//��ÿ���α�������Ӧ�ĳ�ʼ���������ص�������ɿα�
void Schedule::Init() {
	//1.������ÿһ�ڿ�
	GetTeaCls();
	//2.�����α�Ҳ���ǽ���ʦ�����еĿζ����䵽�α���ȥ
	GetRanTab();
}

//��ÿ����ʦ��ÿһ�ڿζ����д���
void Schedule::GetTeaCls() {
	map<Course, int> ::iterator it;
	int cid;
	ClassUnit *cp;
	for (int i = 0; i < tea_que_.size(); i++) {
		it = tea_que_[i].courses_num_.begin();
		while (it != tea_que_[i].courses_num_.end()) {
			//����һ�ſ����еİ༶������
			cid = it->first.course_id_;
			for (int j = 0; j < it->second; j++) {
				//�����µĿΣ����ź�λ��
				cp = new ClassUnit(tea_que_[i], it->first, cls_nuit_que_.size());
				cls_nuit_que_.push_back(*cp);
				tea_que_[i].units_que_.push_back(cp);
				cou_que_[cid].units_.push_back(cp);
			}
			//������һ�ſ�Ŀ�ĿεĲ���
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

vector<vector<int> > Schedule::GetAvlTime(int cid) {
	int pid;
	//0���б�ʾ�ܵĺͣ�1��ʾ�����Ѿ�����֮��ĺ�
	vector<vector<int> > avl = vector<vector<int> >(2, vector<int>(groups_, 0));
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		pid = cou_que_[cid].prefixes_[i];
		for (int j = 0; j < groups_; j++) {
			avl[0][j] += prefixes_[pid].avl_time_[j];
			if (!cou_que_[cid].satisfied[i])
				avl[1][j] += prefixes_[pid].avl_time_[j];
		}
	}
	return avl;
}

int Schedule::GetUnitTime(int cid, int uid, vector<vector<int>> avl) {
	//�Ƚ����������ʦ����ʱ�����룬�ó�һ���ɰ���ʱ���б�	
	//avl�Ǹÿγ���Ҫ�����ŵ�ʱ��,trtimes����ʦ�͸����������ṩ��ʱ��
	vector<bool> trtimes = vector<bool>(groups_);
	int tid = cou_que_[cid].units_[uid]->teacher_.teacher_id_;
	for (int i = 0; i < groups_; i++) {
		trtimes[i] = tea_que_[tid].avl_time_[i] && table_[i].avl;
	}

	int tpos;//��ʾ���ѡ���ĸ�ʱ���
	int tag = cou_que_[cid].satis_num_ != cou_que_[cid].prefixes_.size() ? 1 : 0;
	//key��ʾ�����������ڶ��ٸ�ǰ׺��Ҫ���㣬vector<int>��ʾ�ھ�����Щʱ���
	map<GroupUnit, vector<int> > timelist;
	map<GroupUnit, vector<int> >::iterator it;
	GroupUnit tgu;
	vector<int> tque;
	//�����Ƿ���ڲ������ǰ׺
	//ǰ׺���ڲ�����tag = 1, �����tag = 0
	for (int i = 0; i < groups_; i++) {
		if (avl[1][i] && trtimes[i]) {
			tgu.leave_ = table_[i].leave_;
			if(tag) tgu.times_ = avl[1][i];
			else tgu.times_ = 1;
			timelist[tgu].push_back(i);
		}
	}
	it = timelist.end();
	tque = (it--)->second;
	tpos = tque[rand() % tque.size()];
	return tpos;
}

bool Schedule::GetRanTab() {
	vector<vector<int> > avl;
	int pos;
	for (int i = 0; i < cou_que_.size(); i++){
		for (int j = 0; j < cou_que_[i].units_.size(); j++) {
			//�����ÿ�Ŀ�����е�ǰ׺�����������ʱ��
			avl = GetAvlTime(i);
			//��øýڿ�Ӧ�����ĸ�ʱ���group������
			pos = GetUnitTime(i, j, avl);
		}
	}
	return 0;
}