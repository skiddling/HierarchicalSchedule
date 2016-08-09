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
	cout << unit_id_ << " " << tag << " " << stu_num_ << endl;
	int neednum = tag ? (stu_num_ - stu_upper_) : (stu_lower_ - stu_num_);
	vector<Pattern* > avlpatque;
	map<Pattern*, int> avlinpat;
	map<Pattern*, bool>::iterator it = patterns_.begin();
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
	//根据具体的是人数不够还是超过来进行分类讨论
	//1.选出可以供认出的人的去处
	int temp, avlstusum = 0;
	for (int i = 0; i < avlpatque.size(); i++) {
		temp = avlpatque[i]->GetAvlStuNum(this, tag);
		//如果人数过多，想要移走，也只能把属于该pat的学生都移走
		if (tag)temp = min(temp, patterns_stus_[avlpatque[i]]);
		if (temp) {
			avlinpat[avlpatque[i]] = temp;
			avlstusum += temp;
		}
	}
	cout << "end of get avl stu num" << endl;
	cout << avlstusum << "                      " << neednum << endl;
	//2.选出哪些学生需要被扔出去
	//如果供应人数多那么就随机选择需求数量，如果不够就全部都用来满足需求
	/*if (tag)DecreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);
	else IncreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);*/
	int i = 0;
	vector<int> needused = vector<int>(avlpatque.size(), 0);
	map<Pattern*, int>::iterator ita = avlinpat.begin();
	map<Pattern*, int> patused;
	if (avlstusum > neednum) {
		//要先选中哪些学生需要被换出去
		if (tag)GetSelectedStus(neednum);
		while (neednum) {
			if (ita->second) {
				temp = rand() % (ita->second + 1);
				if (temp > neednum)temp = neednum;
				ita->second -= temp;
				if (patused.find(ita->first) == patused.end())patused[ita->first] = 0;
				patused[ita->first] += temp;
				neednum -= temp;
			}
			ita++;
			if (ita == avlinpat.end())ita = avlinpat.begin();
		}
	}
	else {
		if (tag)GetSelectedStus(avlstusum);
		for (; ita != avlinpat.end(); ita++) {
			patused[ita->first] = ita->second;
		}
	}
	cout << "end of get needused" << endl;
	//3.将扔出去的人进行数据交换
	//对每个模式进行人员的分配
	map<Pattern*, int>::iterator itp = patused.begin();
	for (; itp != patused.end(); itp++) {
		if(itp->second)itp->first->ModifyStuNum(tag, this, itp->second);
	}
	cout << "end of modify stu num" << endl;
	//如果人数超过还要把当前的人数搬出去
	if (tag) {
		map<Pattern*, map<int, int> >::iterator its = selected_stus_.begin();
		while (its != selected_stus_.end()) {
			map<int, int>::iterator itm = its->second.begin();
			//int psum = 0;
			while (itm != its->second.end()) {
				//psum += itm->second;
				its->first->DecreaseStuNum(itm->first, itm->second);
				//pat_path_stus_num_[its->first][itm->first] -= itm->second;
				itm++;
			}
			//patterns_stus_[its->first] -= psum;
			its++;
		}
	}
	cout << "end of motify stu num" << endl;
}

void ClassUnit::GetSelectedStus(int neednum) {
	//随机选择需要删除的学生选中
	selected_stus_.clear();
	map<Pattern*, map<int, int> > ppsn = pat_path_stus_num_;
	map<Pattern*, map<int, int> >::iterator itp = ppsn.begin();
	int temp;
	while (neednum) {
		map<int, int>::iterator itm = itp->second.begin();
		while (itm != itp->second.end() && neednum) {
			if (itm->second) {
				temp = rand() % (itm->second + 1);
				if (temp) {
					if (temp >= neednum)temp = neednum;
					neednum -= temp;
					ppsn[itp->first][itm->first] -= temp;
					//itm->second -= temp;
					selected_stus_[itp->first][itm->first] = pat_path_stus_num_[itp->first][itm->first] - ppsn[itp->first][itm->first];
				}
			}
			itm++;
		}	
		if (!neednum)break;
		itp++;
		if (itp == ppsn.end())itp = ppsn.begin();
	}
}

void ClassUnit::IncreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat) {
	//学生人数不够，需要从其他教室当中把学生调过来
	/*int neednum = stu_lower_ - stu_num_;
	vector<Pattern* > avlpatque;
	GetAvlPatQue(avlpatque);
	map<Pattern*, int> avlstunum;
	vector<int> avlnumpat = vector<int>(avlpatque.size(), 0);
	int temp, avlstusum = 0;
	for (int i = 0; i < avlpatque.size(); i++) {
		temp = avlpatque[i]->GetAvlStuNum(this, 0);
		if (temp) {
			avlstunum[avlpatque[i]] = temp;
			avlnumpat[i] = temp;
			avlstusum += temp;
		}
	}*/

}

void ClassUnit::DecreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat) {
	//学生人数太多，需要将本班级的学生人数分散到其他的教室当中去
	/*int neednum = stu_num_ - stu_upper_;
	vector<Pattern* > avlpatque;
	GetAvlPatQue(avlpatque);*/
}

void ClassUnit::OutPutStu(ofstream &fout) {
	fout << course_.course_name_ << string(' ', 11 - course_.course_name_.length()) << teacher_.teacher_name_ << endl;
	fout << unit_time_.first << "  " << unit_time_.second << endl;
	for (int i = 0; i < students_.size(); i++) {
		fout << students_[i]->student_name_ << string(' ', 11 - students_[i]->student_name_.length()) << "  " << students_[i]->student_id_ << endl;
	}
	fout << endl << endl;
}

void ClassUnit::GetPatStusNum() {
	map<Pattern*, map<int, int> >::iterator itp = pat_path_stus_num_.begin();
	for (; itp != pat_path_stus_num_.end(); itp++) {
		map<int, int>::iterator itm = itp->second.begin();
		int sum = 0;
		for (; itm != itp->second.end(); itm++) {
			sum += itm->second;
		}
		patterns_stus_[itp->first] = sum;
	}
}

void ClassUnit::ResetStuData() {
	if(stu_num_ > 49)cout << unit_id_ << "                                    " << stu_num_ << endl;
	stu_num_ = 0;
	patterns_stus_.clear();
	pat_path_stus_num_.clear();
	selected_stus_.clear();
}

void ClassUnit::GetAvlPatQue(vector<Pattern*>& avlpatque) {
	map<Pattern*, bool>::iterator it = patterns_.begin();
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
}
