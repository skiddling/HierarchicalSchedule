#include "classunit.h"

int ClassUnit::stu_lower_ = 0;
int ClassUnit::stu_upper_ = 0;

ClassUnit::ClassUnit(int unit_id, int stu_num) : unit_id_(unit_id), stu_num_(stu_num){
	alterable_ = 1;
}

ClassUnit::ClassUnit(int unit_id, pair<int, int> unit_time, int stu_num) {
}

ClassUnit::ClassUnit(Teacher teacher, Course course, int unit_id, int stu_num):
	teacher_(teacher), course_(course), unit_id_(unit_id), stu_num_(stu_num){
}

void ClassUnit::init() {

}

void ClassUnit::Modify(bool tag) {
	//0表示人数不够，需要从当前包含的模式当中抽取一定数量的学生来平衡这个班级的人数
	//1表示人数过多，需要把当前的学生数量放一部分到其他的班级当中
	//对于每个模式而言，都有一个不在当前的班级的表，只有当该班级在这些模式当中存在可以替换的班级列表才能将这个模式选中
	int neednum = tag ? (stu_num_ - stu_upper_) : (stu_lower_ - stu_num_);
	vector<Pattern* > avlpatque;
	map<Pattern*, bool>::iterator it = patterns_.begin();
	map<int, int> pid2id;
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
	//根据具体的是人数不够还是超过来进行分类讨论
	map<Pattern*, int> avlstunum;
	vector<int> avlnumpat = vector<int>(avlpatque.size(), 0);
	int temp, avlstusum = 0;
	for (int i = 0; i < avlpatque.size(); i++) {
		temp = avlpatque[i]->GetAvlStuNum(this, tag);
		if (temp) {
			avlstunum[avlpatque[i]] = temp;
			avlnumpat[i] = temp;
			avlstusum += temp;
		}
	}
	//如果供应人数多那么就随机选择需求数量，如果不够就全部都用来满足需求
	int i = 0;
	vector<int> needused = vector<int>(avlpatque.size(), 0);
	if (avlstusum > neednum) {
		while (avlstusum) {
			if (avlnumpat[i]) {
				temp = rand() % avlnumpat[i];
				avlnumpat[i] -= temp;
				needused[i] = temp;
			}
			i++;
			if (i == avlpatque.size()) i = 0;
		}
	}
	else {
		for (; i < avlpatque.size(); i++) {
			needused[i] = avlnumpat[i];
		}
	}
	//对每个模式进行人员的分配
	it = patterns_.begin();
	for (i = 0; i < avlpatque.size(); i++, it++) {
		if (needused[i]) {
			it->first->ModifyStuNum(tag, this, needused[i]);
		}
	}
}
