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
	int t1 = clock(), t2;
	while (GetRanTab()) {
		t2 = clock();
		if (t2 - t1 > kTableTimeOut) {
			success_falg_ = 0;
			return;
		}
	}
	success_falg_ = 1;
	return;
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
				cp = new ClassUnit(tea_que_[i], cou_que_[cid], cls_nuit_que_.size());
				cls_nuit_que_.push_back(*cp);
				tea_que_[i].units_que_.push_back(cp);
				cou_que_[cid].units_.push_back(cp);
			}
			//������һ�ſ�Ŀ�ĿεĲ���
			it++;
		}
	}
	cout << "end of get units" << endl;
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
			if (prefixes_[pid].avl_time_[j]) {
				avl[0][j] = 1;
			}
			if (!cou_que_[cid].satisfied[i])
				//������ڲ�������ô1���вŻ�������֣��������0
				avl[1][j] += prefixes_[pid].avl_time_[j];
		}
	}
	return avl;
}

int Schedule::GetUnitTime(int cid, int uid, vector<vector<int>> avl) {
	//�Ƚ����������ʦ����ʱ�����룬�ó�һ���ɰ���ʱ���б�	
	//avl�Ǹÿγ���Ҫ�����ŵ�ʱ��,trtimes����ʦ�͸����������ṩ��ʱ��
	vector<bool> trtimes = vector<bool>(groups_, 0);
	int tid = cou_que_[cid].units_[uid]->teacher_.teacher_id_;
	for (int i = 0; i < groups_; i++) {
		//trtimes[i] = tea_que_[tid].avl_time_[i] && table_[i].avl;
		if (tea_que_[tid].avl_time_[i] && table_[i].avl)
			trtimes[i] = 1;
	}

	int flag = 1, tpos;//��ʾ���ѡ���ĸ�ʱ���
	int tag = cou_que_[cid].satis_num_ != cou_que_[cid].prefixes_.size() ? 1 : 0;
	//key��ʾ�����������ڶ��ٸ�ǰ׺��Ҫ���㣬vector<int>��ʾ�ھ�����Щʱ���
	map<GroupUnit, vector<int> > timelist;
	//map<GroupUnit, vector<int> >::iterator it;
	GroupUnit tgu;
	vector<int> tque;
	//�����Ƿ���ڲ������ǰ׺
	//ǰ׺���ڲ�����tag = 1, �����tag = 0
	for (int i = 0; i < groups_; i++) {
		//���ȫ��������ôavl[1][i]ȫ������0
		if (avl[tag][i] && trtimes[i]) {
			flag = 0;
			tgu.leave_ = table_[i].leave_;
			if(tag) tgu.times_ = avl[1][i];
			else tgu.times_ = 1;
			timelist[tgu].push_back(i);
		}
	}
	if (flag)return -1;
	//it = timelist.end();
	tque = timelist.begin()->second;
	//tque = (it--)->second;
	tpos = tque[rand() % tque.size()];
	return tpos;
}

void Schedule::AssignUnit(int gid, ClassUnit *cup) {
	//���¸���͸ýڿε���Ϣ
	int cpos = table_[gid].cpos_;
	cup->unit_time_ = make_pair(gid, cpos);
	table_[gid].AddUnit(cup);
	//������ʦ��Ϣ
	//cup->teacher_.avl_time_[gid] = 0;
	tea_que_[cup->teacher_.teacher_id_].avl_time_[gid] = 0;
	//�Ըÿ�Ŀ��ǰ׺���и��²���
	Course cou = cup->course_;
	vector<Course> temppat;
	int pid, npid, cid = cup->course_.course_id_;
	map<vector<int>, bool>::iterator ita;
	Prefix tp;
	vector<int> temptime;
	for (int i = 0; i < cou.prefixes_.size(); i++) {
		pid = cou.prefixes_[i];
		tp = prefixes_[pid];
		//��ǰ׺�ܱ�����
		if (tp.avl_time_[gid] && cou_que_[cid].satisfied[i] == 0) {
			cou_que_[cid].satisfied[i] = 1;
			cou_que_[cid].satis_num_++;
		}
		for (ita = tp.avl_tab_.begin(); ita != tp.avl_tab_.end(); ita++) {
			//�����и�ʱ����пգ���ô�Ͱ����ʱ����õ�
			if((ita->first)[gid] == 1){
				temppat = prefixes_[pid].pattern_;
				temppat.push_back(cou);//�γ��µĺ�׺					
				if (prefix_map_.find(temppat) != prefix_map_.end()) {
					//�����һ��ǰ׺�����
					npid = prefix_map_[temppat];
				}
				else npid = -1;
				//temptime��ǰ׺�Ŀ���ʱ��
				temptime = ita->first;
				//����ʱ����õ�
				temptime[gid] = 0;
				if (npid > 0 && prefixes_[npid].avl_tab_.find(temptime) == prefixes_[npid].avl_tab_.end()) {
					prefixes_[npid].avl_tab_[temptime] = 1;
					for (int k = 0; k < groups_; k++) {
						if (temptime[k] == 1)
							prefixes_[npid].avl_time_[k] = 1;
					}
				}
			}
		}
	}
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
			if (pos < 0)return 1;
			AssignUnit(pos, cou_que_[i].units_[j]);
		}
	}
	cout << "end of the rand table" << endl;
	return 0;
}