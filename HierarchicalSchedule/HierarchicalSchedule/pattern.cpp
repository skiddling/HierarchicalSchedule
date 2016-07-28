#include "pattern.h"
#include "classunit.h"
#include "group.h"

Pattern::Pattern() {
}

Pattern::Pattern(vector<Course> course_que, int stu_num) :
	course_que_(course_que), stu_num_(stu_num){
}

void Pattern::GetAllPath(vector<Group> table) {
	//dfs�����õ����е�·��
	//һ���Ų�ÿ����Ŀ
	int rid;
	vector<bool> visited = vector<bool>(table.size(), false);
	vector<pair<int, int> > path = vector<pair<int, int> >(course_que_.size());
	DFS(0, visited, path, table);
	//��Ϊÿ��pattern��Ȼ��һ��·������dfs��Ȼ�ɹ�
}

void Pattern::DFS(int gid, vector<bool> visited, vector<pair<int, int> > path, vector<Group> table) {
	if (gid == course_que_.size()) {
		path_.push_back(path);
		return;
	}
	int rid;
	for (int cid = 0; cid < course_que_.size(); cid++) {
		//�ÿγ�û�б����ʹ�
		if (visited[cid] == false) {
			if (table[gid].cou_set_.find(course_que_[cid]) != table[gid].cou_set_.end()) {
				for (int i = 0; i < table[gid].cou_set_[course_que_[cid]].size(); i++) {
					rid = table[gid].cou_set_[course_que_[cid]][i];
					path[cid] = make_pair(gid, rid);
				}
			}
			visited[cid] = true;
			DFS(gid + 1, visited, path, table);
			visited[cid] = false;
		}
	}
}

void Pattern::GetNotInTable() {
	//�Ȼ�����еĽڵ��set
	map<pair<int, int>, vector<bool> > isin;
	for (int i = 0; i < path_.size(); i++) {
		for (int j = 0; j < path_[i].size(); j++) {
			if (unit_set_.find(path_[i][j]) == unit_set_.end()) {
				unit_set_.insert(path_[i][j]);
				isin[path_[i][j]] = vector<bool>(path_.size(), false);
			}
			isin[path_[i][j]][i] = true;
		}
	}
	//Ȼ����notin��
	map<pair<int, int>, vector<bool> >::iterator itin;
	for (itin = isin.begin(); itin != isin.end(); itin++) {
		for (int i = 0; i < path_.size(); i++) {
			if (itin->second[i] == false) {
				//�ý���û���ڸ�·���ϳ��ֹ�
				not_in_table_[itin->first].push_back(i);
			}
		}
	}
}