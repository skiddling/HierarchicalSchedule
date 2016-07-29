#include "schedule.h"

//在schedule层面上做的操作都是对schedule直接的属性进行更新修改
const double kMXMP = 0.1;
const double kMXMPG = 0.2;
const double kMXCP = .035;
const double kSTEP = 1.3;

int Schedule::stu_lower_ = 0;
int Schedule::stu_upper_ = 0;
int Schedule::rooms_ = 0;
int Schedule::groups_ = 0;
double Schedule::po_cross_ = 0.2;
double Schedule::po_mutate_gene_ = 0.1;
double Schedule::po_mutate_ = 0.01;

const double Schedule::mx_pmutate_ = 0.05;
const double Schedule::mx_pmutate_gene_ = 0.1;
const double Schedule::con_pcross_ = 0.2;
const double Schedule::con_pmutate = 0.01;
const double Schedule::con_pmutate_gene_ = 0.1;
const int Schedule::kTableTimeOut = 1000000;

Schedule::Schedule() {
}

Schedule::Schedule(vector<Course> cou_que, vector<Student> stu_que, vector<Teacher> tea_que,
	map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, map<vector<Course>, int> prefix_map,
	vector<Prefix> prefixes, vector<int> topo_sorted):
	cou_que_(cou_que), stu_que_(stu_que), tea_que_(tea_que), pattern_map_(pattern_map),
	pattern_que_(pattern_que), prefix_map_(prefix_map), prefixes_(prefixes), topo_sorted_(topo_sorted){
	table_ = vector<Group>(groups_, *(new Group(rooms_, rooms_)));
}

//对每个课表都进行相应的初始化工作，重点就是生成课表
void Schedule::Init() {
	//1.现生成每一节课
	GetTeaCls();
	//2.产生课表，也就是将老师的所有的课都分配到课表当中去
	int t1 = clock(), t2;
	while (1) {
		Schedule temps(*this);
		if (!temps.GetRanTab()) {
			*this = temps;
			break;
		}
		t2 = clock();
		if (t2 - t1 > kTableTimeOut) {
			success_falg_ = 0;
			return;
		}
	}
	success_falg_ = 1;
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
				cls_nuit_que_.push_back(*(new ClassUnit(tea_que_[i], cou_que_[cid], cls_nuit_que_.size())));
			}
			//进行下一门科目的课的产生
			it++;
		}
	}
	int tid;
	//等所有的课都已经安排好位置之后才能记录各个课的pos，不然会在添加课的途中因vetcot修改位置而导致记录失效
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		tid = cls_nuit_que_[i].teacher_.teacher_id_;
		cid = cls_nuit_que_[i].course_.course_id_;
		tea_que_[tid].units_que_.push_back(&cls_nuit_que_[i]);
		cou_que_[cid].units_.push_back(&cls_nuit_que_[i]);
	}
	//cout << "end of get units" << endl;
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
			if (!cou_que_[cid].satisfied_[i])
			//if(!prefixes_[pid].satisfied_)
				//如果存在不满足那么1这列才会出现数字，否则就是0
				avl[1][j] += prefixes_[pid].avl_time_[j];
		}
	}
	//cout << "end of get avl time" << endl;
	return avl;
}

bool Schedule::GetTimeList(int tag, map<GroupUnit, vector<int>>& timelist, vector<vector<int>> avl, vector<bool> trtimes) {
	GroupUnit tgu;
	bool flag = 1;
	for (int i = 0; i < groups_; i++) {
		if (avl[tag][i] && trtimes[i]) {
			flag = 0;
			tgu.leave_ = table_[i].leave_;
			tgu.times_ = avl[tag][i];
			timelist[tgu].push_back(i);
		}
	}
	return flag;
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

	int flag = 1;//表示最后选中哪个时间段
	//先对为满足的课进行满足，如果因为老师和空余组的关系无法找到时间，那么就再考虑满足老师和组的情况
	int k = 0, pid;
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		//pid = cou_que_[cid].prefixes_[i];
		//if (prefixes_[pid].satisfied_)k++;
		if (cou_que_[cid].satisfied_[i])k++;
	}
	cou_que_[cid].satis_num_ = k;
	int tag = k != cou_que_[cid].prefixes_.size() ? 1 : 0;
	//key表示数量级，存在多少个前缀需要满足，vector<int>表示在具体哪些时间段
	map<GroupUnit, vector<int> > timelist;
	vector<int> tque;
	//先判是否存在不满足的前缀
	//前缀存在不满足tag = 1, 满足就tag = 0
	flag = GetTimeList(tag, timelist, avl, trtimes);
	if (flag && tag == 1) GetTimeList(0, timelist, avl, trtimes);
	if (flag)return -1;
	tque = timelist.begin()->second;
	//cout << "end of get unit time" << endl;
	return tque[rand() % tque.size()];
}

void Schedule::AssignUnit(int gid, ClassUnit *cup) {
	//更新该组和该节课的信息
	int cpos = table_[gid].cpos_;
	//cup->unit_time_ = make_pair(gid, cpos);
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
		if (tp.avl_time_[gid] && cou_que_[cid].satisfied_[i] == 0) {
		//if (tp.avl_time_[gid] && tp.satisfied_ == 0){
			cou_que_[cid].satisfied_[i] = 1;
			//prefixes_[pid].satisfied_ = 1;
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
	//cout << "end of assign time" << endl;
}

bool Schedule::CheckPrefix(int cid) {
	int pid;
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		//pid = cou_que_[cid].prefixes_[i];
		//if (!prefixes_[pid].satisfied_)return true;
		if (!cou_que_[cid].satisfied_[i])return true;
	}
	return false;
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
		if (CheckPrefix(i))return 1;
		//cout << "course  " << i << endl;
	}
	cout << "end of the rand table" << endl;
	return 0;
}

void Schedule::GetAllPath() {
	//此处的重点是对每个pattern进行操作
	for (int i = 0; i < groups_; i++) {
		table_[i].GetCouSet(i);
	}
	for (int i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].GetAllPath(table_);
		pattern_que_[i].GetNotInTable();
	}
	//cout << "end of get not in table" << endl;
}

void Schedule::StuAssign() {
	for (int i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].StuAssign();
	}
}

void Schedule::CalCrashFitness() {
	crash_ = 0;
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		if (cls_nuit_que_[i].stu_num_ > stu_upper_)crash_ += cls_nuit_que_[i].stu_num_ - stu_upper_;
		else if (cls_nuit_que_[i].stu_num_ < stu_lower_)crash_ += stu_lower_ - cls_nuit_que_[i].stu_num_;
	}
	fitness = 1.0 / static_cast<double>(1 + crash_);
}