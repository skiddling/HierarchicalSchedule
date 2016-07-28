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

void Pattern::StuAssign() {
	int stusum = stu_num_, temp;
	for (int i = 0; i < path_.size(); i++) {
		if(i != path_.size() - 1)temp = rand() % stusum;
		else temp = stusum;
		for (int j = 0; j < path_[i].size(); j++) {
			path_[i][j]->stu_num_ += temp;
		}
		stusum -= temp;
		if (stusum == 0)break;
	}
}