#include "classunit.h"

//int ClassUnit::stu_lower_ = 0;
//int ClassUnit::stu_upper_ = 0;

ClassUnit::ClassUnit(int unit_id, int stu_num) : 
	unit_id_(unit_id), stu_num_(stu_num){
	alterable_ = 1;
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
}

ClassUnit::ClassUnit(int unit_id, pair<int, int> unit_time, int stu_num) {
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
}

ClassUnit::ClassUnit(Teacher teacher, Course course, int unit_id, int stu_num):
	teacher_(teacher), course_(course), unit_id_(unit_id), stu_num_(stu_num){
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
	stu_lower_ = course.stu_lower_;
	stu_upper_ = course.stu_upper_;
}

void ClassUnit::init() {

}

void ClassUnit::Modify(bool tag) {
	//0表示人数不够，需要从当前包含的模式当中抽取一定数量的学生来平衡这个班级的人数
	//1表示人数过多，需要把当前的学生数量放一部分到其他的班级当中
	//对于每个模式而言，都有一个不在当前的班级的表，只有当该班级在这些模式当中存在可以替换的班级列表才能将这个模式选中
	cout << unit_id_ << " " << tag << " " << stu_num_ << endl;
	//int neednum = tag ? (stu_num_ - stu_upper_) : (stu_lower_ - stu_num_);
	int neednum = stu_num_ - course_.stu_upper_;
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
	if (tag) {
		if (avlstusum > neednum) {
			//要先选中哪些学生需要被换出去
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
			for (; ita != avlinpat.end(); ita++) {
				patused[ita->first] = ita->second;
			}
		}
		GetSelectedStus(patused);
	}
	else {
		//依然用selected来装每个pat能存放的最大的人员数量
		map<Pattern*, bool>::iterator itp = patterns_.begin();
		while (itp != patterns_.end()) {
			selected_stus_[itp->first].insert(itp->first->GetMxStuNum(this));
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
	//如果人数不够就把人数加进来
	if (!tag)AddStu2Path(patused);
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

void ClassUnit::GetSelectedStus(map<Pattern*, int> patused) {
	//随机从要输出的pat当中选出相应人数的学生
	selected_stus_.clear();
	map<Pattern*, map<int, int> > ppsn = pat_path_stus_num_;
	map<Pattern*, int>::iterator itp = patused.begin();
	while (itp != patused.end()) {
		int neednuminpat = itp->second, temp;
		map<int, int>::iterator itm = ppsn[itp->first].begin();
		while (neednuminpat) {
			if (itm->second) {
				temp = rand() % (itm->second + 1);
				if (temp > neednuminpat)temp = neednuminpat;
				neednuminpat -= temp;
				ppsn[itp->first][itm->first] -= temp;
				selected_stus_[itp->first][itm->first] = pat_path_stus_num_[itp->first][itm->first] - ppsn[itp->first][itm->first];
			}
			itm++;
			if (itm == ppsn[itp->first].end())itm = ppsn[itp->first].begin();
		}
		itp++;
	}
	
}

void ClassUnit::AddStu2Path(map<Pattern*, int> patused) {
	//指定在现有已经有的pat的路径当中添加相应的人数
	map<Pattern*, int>::iterator itp = patused.begin();
	while (itp != patused.end()) {
		map<int, int>::iterator itm = pat_path_stus_num_[itp->first].begin();
		itp->first->IncreaseStuNum(itm->first, itp->second);
		itp++;
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
	//fout << course_.course_name_ << string(' ', 11 - course_.course_name_.length()) << teacher_.teacher_name_ << endl;
	fout << course_.course_name_ << "                     " << teacher_.teacher_name_ << endl;
	fout << unit_time_.first << "  " << unit_time_.second << "    " << students_.size() << endl;
	for (int i = 0; i < students_.size(); i++) {
		//fout << students_[i]->student_name_ << string(' ', 11 - students_[i]->student_name_.length()) << students_[i]->student_id_ << endl;
		fout << students_[i]->student_name_ << "                    " << students_[i]->student_id_ << endl;
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

void ClassUnit::GetAllAvlStus() {
	//获得所有能够在这个班级的学生
	for (auto& p : patterns_) {
		for (auto&s : p.first->stu_que_) {
			//allavlnotinstus_.push_back(s);
			//allavlnotinstus_.insert(s);
			stunotin_.insert(s);
			stunotinsex_[s->sex_].insert(s);
		}
	}
}

void ClassUnit::PutStuIntoCls(Student* stu) {
	//将学生放入到班级当中去
	//也就是需要修改当前班级当中相关的所有的数据
	//注意由于不再使用延迟操作，所以不必再去更新学生信息，只需要更新相应进出班级的学生信息即可
	stunotinsex_[stu->sex_].erase(stu);
	stunotin_.erase(stu);

	stu->IntoCls(this);

	sum_points_in_sex_[stu->sex_] += stu->GetCouPoints(this);
	stu_num_in_sex_[stu->sex_]++;
	stuinitsex_[stu->sex_].insert(stu);
	stuinit_.insert(stu);
	stu_num_++;
}

void ClassUnit::GetStuOutCls(Student * stu) {
	stunotinsex_[stu->sex_].insert(stu);
	stunotin_.insert(stu);

	stu->OutCls(this);

	sum_points_in_sex_[stu->sex_] -= stu->GetCouPoints(this);
	stu_num_in_sex_[stu->sex_]--;
	stuinitsex_[stu->sex_].erase(stu);
	stuinit_.erase(stu);
	stu_num_--;
}

int ClassUnit::GetDvaInSex() {
	int res = 0;
	for (int i = 0; i < 2; i++) {
		auto s = static_cast<Sex>(i);
		if (stuinitsex_[s].size() > course_.sex_upper_[s]) {
			res += stuinitsex_[s].size() - course_.sex_upper_[s];
			taginsex_[s] = stuinitsex_[s].size() - course_.sex_upper_[s];
		}
		else if (stuinitsex_[s].size() < course_.sex_lower_[s]) {
			res += course_.sex_lower_[s] - stuinitsex_[s].size();
			taginsex_[s] = stuinitsex_[s].size() - course_.sex_lower_[s];
		}
	}

	return res;
}

void ClassUnit::ModifySexRatio(vector<Pattern> patternque) {
	//每个学生在进出的时候都会有进出的两个影响值，单个班级有利的+1，有害的-1，无影响的为0
	//并最终统计所有的班级的进出总分，进出两个都是正的才能进行交换
	int inpoint, outpoint;
	for (int i = 0; i < 2; i++) {
		auto s = static_cast<Sex>(i);
		if (taginsex_[s]) {
			if (taginsex_[s] > 0) {
				//该性别人数过多，逐个查看学生是否能够被调整
				for (auto& stu : stuinitsex_[s]) {
					auto val = JudgeStuVal4SexRationIn(stu, patternque);
					if (val.first > 0) {
						for (auto c : stu->clsset_) {
							c->GetStuOutCls(stu);
						}
						for (auto c : patternque[stu->patp_].path_[val.second]) {
							c->PutStuIntoCls(stu);
						}
					}
				}
			}
			else {
				//该性别人数过少,逐个查看每个学生
				for (auto& stu : stunotinsex_[s]) {
					auto val = JudgeStuVal4SexRationOut(stu, patternque);
				}
			}
		}
	}
}

pair<int, int> ClassUnit::JudgeStuVal4SexRationIn(Student* s, vector<Pattern> patternque) {
	//计算该学生是否值得出这个班级来平衡这个班级的性别
	int v = 1;
	for (auto c : s->clsset_) {
		if (c != this) {
			v += c->JudgeClsLoseStuInSex(s->sex_);
		}
	}
	int pid = -1, pv = 0, t;
	//if (v > 0) {
	//查询哪个路径最适合去放这个学生,如果不存在这样子的路线那返回-1
	for (auto p : patternque[s->patp_].not_in_table_[this]) {
		t = 0;
		for (auto c : patternque[s->patp_].path_[p]) {
		//for (auto i = 0; i < patternque[s->patp_].path_.size(); i++) {
			t += c->JudgeClsGetStuInSex(s->sex_);
			//t += patternque[s->patp_].path_[p][i]->JudgeClsGetStuInSex(s->sex_);
		}
		if (t > pv) {
			pv = t;
			pid = p;
		}
	}
	//}
	return pair<int, int>(v + pv, pid);
}

pair<int, int> ClassUnit::JudgeStuVal4SexRationOut(Student * s, vector<Pattern> patternque) {
	int v = 1;
	for (auto c : s->clsset_) {
		v += c->JudgeClsLoseStuInSex(s->sex_);
	}
	
	int pid = -1, pv = 0, t;
	return pair<int, int>();
}

int ClassUnit::JudgeClsLoseStuInSex(Sex sex) {
	//if (stuinitsex_[sex].size() > course_.stu_upper_)return 1;
	if (stuinitsex_[sex].size() <= course_.sex_lower_[sex])return -1;
	else return 1;
	//return 0;
}

int ClassUnit::JudgeClsGetStuInSex(Sex sex) {
	if (stuinitsex_[sex].size() >= course_.sex_upper_[sex])return -1;
	else return 1;
	//return 0;
}

void ClassUnit::ModifyTotAmount(vector<Pattern> patternque) {

}

void ClassUnit::ModifyAvgPoints(vector<Pattern> patternque) {

}

void ClassUnit::GetAvlPatQue(vector<Pattern*>& avlpatque) {
	map<Pattern*, bool>::iterator it = patterns_.begin();
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
}
