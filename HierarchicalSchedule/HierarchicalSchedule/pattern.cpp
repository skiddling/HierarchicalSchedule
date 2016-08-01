#include "pattern.h"
#include "classunit.h"
#include "group.h"

Pattern::Pattern() {
}

Pattern::Pattern(vector<Course> course_que, int stu_num) :
	course_que_(course_que), stu_num_(stu_num){
}

void Pattern::GetAllPath(vector<Group> table) {
	//dfs搜索得到所有的路径
	//一次排查每个科目
	int rid;
	vector<bool> visited = vector<bool>(table.size(), false);
	//vector<pair<int, int> > path = vector<pair<int, int> >(course_que_.size());
	vector<ClassUnit* > path = vector<ClassUnit* >(course_que_.size());
	/*for (int i = 0; i < table.size(); i++) {
		DFS(i, visited, path, table);
	}*/
	DFS(0, visited, path, table);
	//因为每个pattern必然有一个路径所以dfs必然成功
}

void Pattern::DFS(int gid, vector<bool> visited, vector<ClassUnit* > path, vector<Group> table) {
	vector<int> unvisit = vector<int>(0);
	for (int cid = 0; cid < course_que_.size(); cid++) {
		if (visited[cid] == false) {
			unvisit.push_back(cid);
		}
	}
	if (unvisit.size() == 0) {
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
	DFS(gid + 1, visited, path, table);
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
	vector<int> ary = vector<int>(path_.size());
	for (int i = 0; i < ary.size(); i++) {
		ary[i] = i;
	}
	GetRandTab(ary);
	chosen_path_tab_ = vector<bool>(path_.size(), false);
	for (int i = 0; i < psz; i++) {
		id = ary[i];
		temp = rand() % stuleft;
		if (temp != 0) {
			stu_num_in_que_[id] = temp;
			stuleft -= temp;
			chosen_path_tab_[id] = true;
			if (!stuleft)return;
		}
	}
	stu_num_in_que_[psz] = stuleft;
	chosen_path_tab_[psz] = true;
}

void Pattern::Mutate(double mp) {
	//对于当前模式下的所有的路径进行变化人数
	int psz = path_.size() - 1, temp, id;
	if (!psz)return;
	double r;
	for (int i = 0; i < psz; i++) {
		if (chosen_path_tab_[i]) {
			r = static_cast<double>(rand() * rand()) / kRndPluRnd;
			if (r < mp) {
				temp = rand() % stu_num_in_que_[i];
				id = GetRandId(i, psz + 1);
				Update(i, id, temp);
			}
		}
	}
}

void Pattern::Cross() {
	if (path_.size() == 1)return;
	vector<int> stuin;
	for (int i = 0; i < path_.size(); i++) {
		if (chosen_path_tab_[i])stuin.push_back(i);
	}
	int id;
	GetRandTab(stuin);
	for (int i = 0; i < stuin.size(); i++) {
		id = stuin[i];
		if (chosen_path_tab_[id]) {
			
		}
	}
}

