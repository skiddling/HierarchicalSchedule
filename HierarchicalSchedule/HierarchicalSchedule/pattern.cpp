#include "pattern.h"
#include "classunit.h"
#include "group.h"

int Pattern::stu_lower_ = 0;
int Pattern::stu_upper_ = 0;

Pattern::Pattern() {
}

Pattern::Pattern(vector<Course> course_que, int stu_num) :
	course_que_(course_que), stu_num_(stu_num){
}

void Pattern::GetAllPath(vector<Group> table) {
	//dfs搜索得到所有的路径
	//一次排查每个科目
	//int rid;
	vector<bool> visited = vector<bool>(table.size(), false);
	//vector<pair<int, int> > path = vector<pair<int, int> >(course_que_.size());
	vector<ClassUnit* > path = vector<ClassUnit* >(course_que_.size());
	/*for (int i = 0; i < table.size(); i++) {
		DFS(i, visited, path, table);
	}*/
	DFS(0, visited, path, table);
	//因为每个pattern必然有一个路径所以dfs必然成功
	GetNotInTable();
	//根据notintable来构造路线组合
	GetPathCombination();
	//cout << "end of get combo" << endl;
}

void Pattern::DFS(int gid, vector<bool> visited, vector<ClassUnit* > path, vector<Group> table) {
	vector<int> unvisit = vector<int>(0);
	for (int cid = 0; cid < course_que_.size(); cid++) {
		if (visited[cid] == false) {
			unvisit.push_back(cid);
		}
	}
	if (unvisit.size() == 0) {
		//把模式加入到班级当中去
		for (int i = 0; i < path.size(); i++) {
			if (path[i]->patterns_.find(this) == path[i]->patterns_.end()) {
				path[i]->patterns_[this] = true;
			}
		}
		path_.push_back(path);
		return;
	}
	else if (gid == table.size())return;
	int rid;
	for (int cid = 0; cid < course_que_.size(); cid++) {
		//该课程没有被访问过
		if (visited[cid] == false) {
			if (table[gid].cou_set_.find(course_que_[cid]) != table[gid].cou_set_.end()) {
				visited[cid] = true;
				for (int i = 0; i < table[gid].cou_set_[course_que_[cid]].size(); i++) {
					rid = table[gid].cou_set_[course_que_[cid]][i];
					path[cid] = table[gid].group[rid];
					DFS(gid + 1, visited, path, table);
				}
				visited[cid] = false;
			}
		}
	}
	//计算剩下还有多少课
	int coursenumleft = 0;
	for (int i = 0; i < visited.size(); i++) {
		if (!visited[i])coursenumleft++;
	}
	//gid是当前的层次号，从0开始，而size是整个的数量，比如4-0 = 4，只有当前还剩下3个的时候才能继续去排，所以小于号
	if(coursenumleft < table.size() - gid)DFS(gid + 1, visited, path, table);
}

void Pattern::GetNotInTable() {
	//先获得所有的节点的set
	map<ClassUnit*, vector<bool> > isin;
	for (int i = 0; i < path_.size(); i++) {
		for (int j = 0; j < path_[i].size(); j++) {
			if (unit_set_.find(path_[i][j]) == unit_set_.end()) {
				unit_set_.insert(path_[i][j]);
				isin[path_[i][j]] = vector<bool>(path_.size(), false);
			}
			//构造in表
			in_unit_table_[path_[i][j]].push_back(i);
			isin[path_[i][j]][i] = true;
		}
	}
	//然后构造notin表
	map<ClassUnit*, vector<bool> >::iterator itin;
	for (itin = isin.begin(); itin != isin.end(); itin++) {
		for (int i = 0; i < path_.size(); i++) {
			if (itin->second[i] == false) {
				//该教室没有在该路径上出现过
				not_in_table_[itin->first].push_back(i);
			}
		}
	}
}

void Pattern::GetPathCombination() {
	//获得所有的not	
	map<ClassUnit*, vector<int> >::iterator it = not_in_table_.begin();
	set<ClassUnit* > units;
	set<vector<int> > comboset;
	vector<int> que = vector<int> (0);
	for (; it != not_in_table_.end(); it++) {
		for(int i = 0; i < it->second.size(); i++)
		GetEachCombo(it->first, units, i, que);
	}
}

void Pattern::GetEachCombo(ClassUnit* cp, set<ClassUnit* > units, int pos, vector<int> que) {
	if (pos == not_in_table_[cp].size()) {
		//if (comboset.find(que) == comboset.end()) {
			notin_path_combos_[cp].push_back(que);
		//	comboset.insert(que);
		//}
		return;
	}
	bool tag = true;
	for (int i = pos; i < not_in_table_[cp].size(); i++) {
		int pid = not_in_table_[cp][i];
		if (GetIsIn(cp, units, pid))continue;
		else {
			for (int j = 0; j < path_[pid].size(); j++) {
				units.insert(path_[pid][j]);
			}
			que.push_back(pid);
			//if (comboset.find(que) == comboset.end())comboset.insert(que);
			GetEachCombo(cp, units, i + 1, que);
			//tag=false表示当前这个序列并不是最终序列，而是一个子序列
			tag = false;
		}
	}
	if (tag) {
		//if (comboset.find(que) == comboset.end()) {
			notin_path_combos_[cp].push_back(que);
		//	comboset.insert(que);
		//}
	}
}

bool Pattern::GetIsIn(ClassUnit * cp, set<ClassUnit*> units, int pid) {
	for (int i = 0; i < path_[pid].size(); i++) {
		if (units.find(path_[pid][i]) != units.end())return true;
	}
	return false;
}

void Pattern::GetRandTab(vector<int>& ary) {
	int id, asz = ary.size();
	for (int i = 0; i < asz; i++) {
		id = rand() % asz;
		if (id != i)swap(ary[i], ary[id]);
	}
}

void Pattern::StuAssign() {
	//有一个延迟操作的概念，只有当modify的时候才会进行把学生分配到各个班级当中
	//mutate和cross操作都不会去涉及classunit的操作
	int stuleft = stu_num_, temp, id, psz = path_.size() - 1;
	stu_num_in_que_ = vector<int>(psz + 1, 0);
	vector<int> ary = vector<int>(path_.size());
	for (int i = 0; i < ary.size(); i++) {
		ary[i] = i;
	}
	GetRandTab(ary);
	chosen_path_tab_ = vector<bool>(path_.size(), false);
	for (int i = 0; i < psz; i++) {
		id = ary[i];
		temp = rand() % (stuleft + 1);
		if (temp != 0) {
			stu_num_in_que_[id] = temp;
			stuleft -= temp;
			chosen_path_tab_[id] = true;
			if (!stuleft)return;
		}
	}
	stu_num_in_que_[ary[psz]] = stuleft;
	chosen_path_tab_[ary[psz]] = true;
}

void Pattern::Mutate(double mp) {
	//对于当前模式下的所有的路径进行变化人数
	int psz = path_.size(), temp, id;
	if (psz == 1)return;
	double r;
	for (int i = 0; i < psz; i++) {
		//if (chosen_path_tab_[i]) {
		if(stu_num_in_que_[i]){
			r = static_cast<double>(rand() * rand()) / kRndPluRnd;
			//r = 0;
			if (r < mp) {
				temp = rand() % (stu_num_in_que_[i] + 1);
				if (!temp)break;
				id = GetRandId(i);
				Update(i, id, temp);
			}
		}
	}
}

void Pattern::Cross() {
	if (path_.size() == 1)return;
	vector<int> stuin;
	for (int i = 0; i < path_.size(); i++) {
		//if (chosen_path_tab_[i])stuin.push_back(i);
		if (stu_num_in_que_[i])stuin.push_back(i);
	}
	int id;
	GetRandTab(stuin);
	for (int i = 0; i < stuin.size(); i++) {
		id = stuin[i];
		//if (chosen_path_tab_[id]) {
		if(stu_num_in_que_[id]){
			SwapStu(id);
		}
	}
}

void Pattern::SwapStu(int oid) {
	//0表示的要出去的，1表示进入的
	int iid = GetRandId(oid), temp[2];
	do {
		temp[0] = rand() % (stu_num_in_que_[oid] + 1);
		//cout << temp[0] <<  " " << stu_num_in_que_[oid] << endl;
	} while (!temp[0]);
	if (stu_num_in_que_[iid] > 0)temp[1] = rand() % (stu_num_in_que_[iid] + 1);
	else temp[1] = 0;
	stu_num_in_que_[oid] = stu_num_in_que_[oid] - temp[0] + temp[1];
	stu_num_in_que_[iid] = stu_num_in_que_[iid] - temp[1] + temp[0];
	/*if (!stu_num_in_que_[oid])chosen_path_tab_[oid] = 0;
	if (stu_num_in_que_[iid])chosen_path_tab_[iid] = 1;
	else chosen_path_tab_[iid] = 0;*/
}

void Pattern::PutStuDown2Cls() {
	for (int i = 0; i < path_.size(); i++) {
		//if (chosen_path_tab_[i]) {
		if(stu_num_in_que_[i]){
			for (int j = 0; j < path_[i].size(); j++) {
				path_[i][j]->stu_num_ += stu_num_in_que_[i];
				path_[i][j]->pat_path_stus_num_[this][i] = stu_num_in_que_[i];
			}
		}
	}
}

int Pattern::GetAvlStuNum(ClassUnit* cp, bool tag) {
	//tag=0表示要查找人数多于最小人数限额的班级
	//1表示相反
	//如果是要得到最多的，通过该路径分配的人数和该路径当中每个班级能提供的数量来进行得到最后的结果
	//每次进行修正的时候才会用到avl_num_each_path
	avl_num_each_path_ = vector<int>(path_.size(), 0);
	avl_sum_ = 0;
	int pid, temp;
	for (int i = 0; i < not_in_table_[cp].size(); i++) {
		pid = not_in_table_[cp][i];
		temp = INT_MAX;
		for (int j = 0; j < path_[pid].size(); j++) {
			if (!tag) {
				//如果是求最多能提供多少人数，就是当前人数-人数下限
				if(path_[pid][j]->stu_num_ > stu_lower_)
					temp = min(temp, path_[pid][j]->stu_num_ - stu_lower_);
				else {
					temp = 0;
					break;
				}
			}
			else {
				//能给出多少空的位置
				/*if (path_[pid][j]->stu_num_ < stu_upper_)
					temp = min(temp, stu_upper_ - path_[pid][j]->stu_num_);*/
				if (path_[pid][j]->stu_num_ < path_[pid][j]->course_.stu_upper_)
					temp = min(temp, path_[pid][j]->course_.stu_upper_ - path_[pid][j]->stu_num_);
				else {
					temp = 0;
					break;
				}
			}
		}
		//只有当需要拿出来的时候才会
		//如果需要减少人数，那么最多就是把该路径当中的人数都搬走
		if(!tag) temp = min(stu_num_in_que_[pid], temp);
		avl_num_each_path_[pid] = temp;
		//avl_sum_ += temp;
	}
	avl_sum_ = GetMaxAvlStus(cp);
	//if (avl_sum_ == 0)avl_num_each_path_.clear();
	//如果需要增加人数，那么最多就是把cp当中所有该pat的学生都搬走
	if (tag)temp = min(avl_sum_, cp->patterns_stus_[this]);
	return temp;
}

int Pattern::GetMaxAvlStus(ClassUnit * cp) {
	int csz = notin_path_combos_[cp].size(), qsz;
	int mx = 0, temp, pid;
	for (int i = 0; i < csz; i++) {
		temp = 0;
		qsz = notin_path_combos_[cp][i].size();
		for (int j = 0; j < qsz; j++) {
			pid = notin_path_combos_[cp][i][j];
			temp += avl_num_each_path_[pid];
		}
		if (temp > mx) {
			mx = temp;
			//notint当中的第i个combo组合的序号
			combosid = i;
		}
	}
	return mx;
}

void Pattern::ModifyStuNum(bool tag, ClassUnit* cp, int neednum) {
	//tag=0表示要拿出neednum这些学生到cp当中去
	//tag=1表示cp要拿出neednum这些学生到当前pat当中来
	//修改的人数都是只对comboid指向的那个路径组合的班级人数进行修改
	vector<int> usednum = vector<int>(path_.size(), 0);
	int i = 0, temp, recode = neednum, pid;
	if (avl_sum_ > neednum) {
		while (i < notin_path_combos_[cp][combosid].size() && neednum) {
			pid = notin_path_combos_[cp][combosid][i];
			temp = rand() % (avl_num_each_path_[pid] + 1);
			if (temp >= neednum)temp = neednum;
			if (temp) {
				neednum -= temp;
				avl_num_each_path_[pid] -= temp;
				usednum[pid] += temp;
			}
			i++;
			if (i == notin_path_combos_[cp][combosid].size())i = 0;
		}
	}
	else {
		for (; i < notin_path_combos_[cp][combosid].size(); i++) {
			pid = notin_path_combos_[cp][combosid][i];
			usednum[pid] = avl_num_each_path_[pid];
		}
	}
	//进行修改人数
	for (i = 0; i < path_.size(); i++) {
		if (usednum[i]) {
			if (tag)IncreaseStuNum(i, usednum[i]);
			else DecreaseStuNum(i, usednum[i]);
		}
	}
	avl_num_each_path_.clear();
}

void Pattern::IncreaseStuNum(int pid, int stunum) {
	stu_num_in_que_[pid] += stunum;
	for (int i = 0; i < path_[pid].size(); i++) {
		path_[pid][i]->stu_num_ += stunum;
		path_[pid][i]->patterns_stus_[this] += stunum;
		path_[pid][i]->pat_path_stus_num_[this][pid] += stunum;
	}
}

pair<int, int> Pattern::GetMxStuNum(ClassUnit* cp) {
	int mxn = 0, mxp, pid, temp;
	for (int i = 0; i < in_unit_table_[cp].size(); i++) {
		pid = in_unit_table_[cp][i];
		temp = INT_MAX;
		for (int j = 0; j < path_[pid].size(); j++) {
			//if (path_[pid][j]->stu_num_ > stu_upper_) {
			if(path_[pid][j]->stu_num_ > path_[pid][j]->course_.stu_upper_){
				temp = 0;
				break;
			}
			else {
				//temp = min(temp, stu_upper_ - path_[pid][j]->stu_num_);
				temp = min(temp, path_[pid][j]->course_.stu_upper_ - path_[pid][j]->stu_num_);
			}
		}
		if (temp > mxn) {
			mxn = temp;
			mxp = i;
		}
	}
	return make_pair(mxp, mxn);
}

void Pattern::AssignStuDown2Cls(vector<ClassUnit> &clsque) {
	int sp = 0;
	int cid;
	for (int i = 0; i < stu_num_in_que_.size(); i++) {
		if (stu_num_in_que_[i]) {
			for (int j = 0; j < stu_num_in_que_[i]; j++) {
				for (int k = 0; k < path_[i].size(); k++) {
					cid = path_[i][k]->unit_id_;
					clsque[cid].students_.push_back(stu_que_[sp + j]);
				}
			}
			sp += stu_num_in_que_[i];
		}
	}
}

void Pattern::DecreaseStuNum(int pid, int stunum) {
	//将这条线上的学生人数都减少相应的数量
	stu_num_in_que_[pid] -= stunum;
	for (int i = 0; i < path_[pid].size(); i++) {
		path_[pid][i]->stu_num_ -= stunum;
		path_[pid][i]->patterns_stus_[this] -= stunum;
		path_[pid][i]->pat_path_stus_num_[this][pid] -= stunum;
	}
}