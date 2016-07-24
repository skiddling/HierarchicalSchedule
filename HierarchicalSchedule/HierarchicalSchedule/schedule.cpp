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

//对每个课表都进行相应的初始化工作，重点就是生成课表
void Schedule::Init() {
	//1.现生成每一节课
	GetTeaCls();
	//2.产生课表，也就是将老师的所有的课都分配到课表当中去
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

//对每个老师的每一节课都进行创建
void Schedule::GetTeaCls() {
	map<Course, int> ::iterator it;
	int cid;
	ClassUnit *cp;
	for (int i = 0; i < tea_que_.size(); i++) {
		it = tea_que_[i].courses_num_.begin();
		while (it != tea_que_[i].courses_num_.end()) {
			//单独一门课所有的班级都产生
			cid = it->first.course_id_;
			for (int j = 0; j < it->second; j++) {
				//产生新的课，并放好位置
				cp = new ClassUnit(tea_que_[i], cou_que_[cid], cls_nuit_que_.size());
				cls_nuit_que_.push_back(*cp);
				tea_que_[i].units_que_.push_back(cp);
				cou_que_[cid].units_.push_back(cp);
			}
			//进行下一门科目的课的产生
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
	//0这行表示总的和，1表示除了已经满足之外的和
	vector<vector<int> > avl = vector<vector<int> >(2, vector<int>(groups_, 0));
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		pid = cou_que_[cid].prefixes_[i];
		for (int j = 0; j < groups_; j++) {
			if (prefixes_[pid].avl_time_[j]) {
				avl[0][j] = 1;
			}
			if (!cou_que_[cid].satisfied[i])
				//如果存在不满足那么1这列才会出现数字，否则就是0
				avl[1][j] += prefixes_[pid].avl_time_[j];
		}
	}
	return avl;
}

int Schedule::GetUnitTime(int cid, int uid, vector<vector<int>> avl) {
	//先将空余组和老师空余时间相与，得出一个可安排时间列表	
	//avl是该课程需要被安排的时间,trtimes是老师和该组有能力提供的时间
	vector<bool> trtimes = vector<bool>(groups_, 0);
	int tid = cou_que_[cid].units_[uid]->teacher_.teacher_id_;
	for (int i = 0; i < groups_; i++) {
		//trtimes[i] = tea_que_[tid].avl_time_[i] && table_[i].avl;
		if (tea_que_[tid].avl_time_[i] && table_[i].avl)
			trtimes[i] = 1;
	}

	int flag = 1, tpos;//表示最后选中哪个时间段
	int tag = cou_que_[cid].satis_num_ != cou_que_[cid].prefixes_.size() ? 1 : 0;
	//key表示数量级，存在多少个前缀需要满足，vector<int>表示在具体哪些时间段
	map<GroupUnit, vector<int> > timelist;
	//map<GroupUnit, vector<int> >::iterator it;
	GroupUnit tgu;
	vector<int> tque;
	//先判是否存在不满足的前缀
	//前缀存在不满足tag = 1, 满足就tag = 0
	for (int i = 0; i < groups_; i++) {
		//如果全部满足那么avl[1][i]全部都是0
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
	//更新该组和该节课的信息
	int cpos = table_[gid].cpos_;
	cup->unit_time_ = make_pair(gid, cpos);
	table_[gid].AddUnit(cup);
	//更新老师信息
	//cup->teacher_.avl_time_[gid] = 0;
	tea_que_[cup->teacher_.teacher_id_].avl_time_[gid] = 0;
	//对该科目的前缀进行更新操作
	Course cou = cup->course_;
	vector<Course> temppat;
	int pid, npid, cid = cup->course_.course_id_;
	map<vector<int>, bool>::iterator ita;
	Prefix tp;
	vector<int> temptime;
	for (int i = 0; i < cou.prefixes_.size(); i++) {
		pid = cou.prefixes_[i];
		tp = prefixes_[pid];
		//该前缀能被满足
		if (tp.avl_time_[gid] && cou_que_[cid].satisfied[i] == 0) {
			cou_que_[cid].satisfied[i] = 1;
			cou_que_[cid].satis_num_++;
		}
		for (ita = tp.avl_tab_.begin(); ita != tp.avl_tab_.end(); ita++) {
			//该序列该时间段有空，那么就把这个时间段用掉
			if((ita->first)[gid] == 1){
				temppat = prefixes_[pid].pattern_;
				temppat.push_back(cou);//形成新的后缀					
				if (prefix_map_.find(temppat) != prefix_map_.end()) {
					//获得下一个前缀的序号
					npid = prefix_map_[temppat];
				}
				else npid = -1;
				//temptime是前缀的可用时间
				temptime = ita->first;
				//将该时间段用掉
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
			//遍历该科目的所有的前缀，并获得所有时间
			avl = GetAvlTime(i);
			//获得该节课应该在哪个时间段group被放置
			pos = GetUnitTime(i, j, avl);
			if (pos < 0)return 1;
			AssignUnit(pos, cou_que_[i].units_[j]);
		}
	}
	cout << "end of the rand table" << endl;
	return 0;
}