#include "classunit.h"

//int ClassUnit::stu_lower_ = 0;
//int ClassUnit::stu_upper_ = 0;

ClassUnit::ClassUnit(int unit_id, int stu_num) : 
	unit_id_(unit_id), stu_num_(stu_num){
	alterable_ = 1;
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
	e_ = default_random_engine(time(NULL));
}

ClassUnit::ClassUnit(int unit_id, pair<int, int> unit_time, int stu_num) {
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
	e_ = default_random_engine(time(NULL));
}

ClassUnit::ClassUnit(Teacher teacher, Course course, int unit_id, int stu_num):
	teacher_(teacher), course_(course), unit_id_(unit_id), stu_num_(stu_num){
	sum_points_in_sex_[male] = sum_points_in_sex_[female] = 0.0;
	stu_num_in_sex_[male] = stu_num_in_sex_[female] = 0;
	stu_lower_ = course.stu_lower_;
	stu_upper_ = course.stu_upper_;
	e_ = default_random_engine(time(NULL));
}

void ClassUnit::init() {

}

void ClassUnit::Modify(bool tag) {
	//0��ʾ������������Ҫ�ӵ�ǰ������ģʽ���г�ȡһ��������ѧ����ƽ������༶������
	//1��ʾ�������࣬��Ҫ�ѵ�ǰ��ѧ��������һ���ֵ������İ༶����
	//����ÿ��ģʽ���ԣ�����һ�����ڵ�ǰ�İ༶�ı�ֻ�е��ð༶����Щģʽ���д��ڿ����滻�İ༶�б���ܽ����ģʽѡ��
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
	//���ݾ�����������������ǳ��������з�������
	//1.ѡ�����Թ��ϳ����˵�ȥ��
	int temp, avlstusum = 0;
	for (int i = 0; i < avlpatque.size(); i++) {
		temp = avlpatque[i]->GetAvlStuNum(this, tag);
		//����������࣬��Ҫ���ߣ�Ҳֻ�ܰ����ڸ�pat��ѧ��������
		if (tag)temp = min(temp, patterns_stus_[avlpatque[i]]);
		if (temp) {
			avlinpat[avlpatque[i]] = temp;
			avlstusum += temp;
		}
	}
	cout << "end of get avl stu num" << endl;
	cout << avlstusum << "                      " << neednum << endl;
	//2.ѡ����Щѧ����Ҫ���ӳ�ȥ
	//�����Ӧ��������ô�����ѡ���������������������ȫ����������������
	/*if (tag)DecreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);
	else IncreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);*/
	int i = 0;
	vector<int> needused = vector<int>(avlpatque.size(), 0);
	map<Pattern*, int>::iterator ita = avlinpat.begin();
	map<Pattern*, int> patused;
	if (tag) {
		if (avlstusum > neednum) {
			//Ҫ��ѡ����Щѧ����Ҫ������ȥ
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
		//��Ȼ��selected��װÿ��pat�ܴ�ŵ�������Ա����
		map<Pattern*, bool>::iterator itp = patterns_.begin();
		while (itp != patterns_.end()) {
			selected_stus_[itp->first].insert(itp->first->GetMxStuNum(this));
		}
	}
	cout << "end of get needused" << endl;
	//3.���ӳ�ȥ���˽������ݽ���
	//��ÿ��ģʽ������Ա�ķ���
	map<Pattern*, int>::iterator itp = patused.begin();
	for (; itp != patused.end(); itp++) {
		if(itp->second)itp->first->ModifyStuNum(tag, this, itp->second);
	}
	cout << "end of modify stu num" << endl;
	//������������Ͱ������ӽ���
	if (!tag)AddStu2Path(patused);
	//�������������Ҫ�ѵ�ǰ���������ȥ
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
	//�����Ҫ�����pat����ѡ����Ӧ������ѧ��
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
	//ָ���������Ѿ��е�pat��·�����������Ӧ������
	map<Pattern*, int>::iterator itp = patused.begin();
	while (itp != patused.end()) {
		map<int, int>::iterator itm = pat_path_stus_num_[itp->first].begin();
		itp->first->IncreaseStuNum(itm->first, itp->second);
		itp++;
	}
}

void ClassUnit::IncreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat) {
	//ѧ��������������Ҫ���������ҵ��а�ѧ��������
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
	//ѧ������̫�࣬��Ҫ�����༶��ѧ��������ɢ�������Ľ��ҵ���ȥ
	/*int neednum = stu_num_ - stu_upper_;
	vector<Pattern* > avlpatque;
	GetAvlPatQue(avlpatque);*/
}

void ClassUnit::OutPutStu(ofstream &fout) {
	//fout << course_.course_name_ << string(' ', 11 - course_.course_name_.length()) << teacher_.teacher_name_ << endl;
	fout << course_.course_name_ << "                     " << teacher_.teacher_name_ << endl;
	fout << unit_time_.first << "  " << unit_time_.second << "    " << students_.size() << endl;
	fout << "male " << stuinitsex_[male].size() << " " << "female " << stuinitsex_[female].size() << endl;
	fout << "avg points " << avg_sum_ << endl;
	//for (int i = 0; i < students_.size(); i++) {
	//	//fout << students_[i]->student_name_ << string(' ', 11 - students_[i]->student_name_.length()) << students_[i]->student_id_ << endl;
	//	fout << students_[i]->student_name_ << "    " << students_[i]->student_id_ << endl;
	//}
	for (auto stu : stuinit_) {
		fout << stu->student_name_ << "    " << stu->sex_ << "    " << stu->points_[this->course_] << endl;
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
	//��������ܹ�������༶��ѧ��
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
	//��ѧ�����뵽�༶����ȥ
	//Ҳ������Ҫ�޸ĵ�ǰ�༶������ص����е�����
	//ע�����ڲ���ʹ���ӳٲ��������Բ�����ȥ����ѧ����Ϣ��ֻ��Ҫ������Ӧ�����༶��ѧ����Ϣ����
	stunotinsex_[stu->sex_].erase(stu);
	stunotin_.erase(stu);

	stu->IntoCls(this);

	sum_points_in_sex_[stu->sex_] += stu->GetCouPoints(this);
	stu_num_in_sex_[stu->sex_]++;
	stuinitsex_[stu->sex_].insert(stu);
	stuinit_.insert(stu);
	stu_num_++;

	sum_points_ += stu->GetCouPoints(this);
	avg_sum_ = sum_points_ / stuinit_.size();
	//GetDvaInSex();
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

	sum_points_ -= stu->GetCouPoints(this);
	avg_sum_ = sum_points_ / stuinit_.size();
	//GetDvaInSex();
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

int ClassUnit::GetDavInTotAmount() {
	int res = 0;
	dvaintot_ = 0; 
	if (stuinit_.size() > course_.stu_upper_) {
		res += stuinit_.size() - course_.stu_upper_;
		dvaintot_ = res;
	}
	else if (stuinit_.size() < course_.stu_lower_) {
		res += course_.stu_lower_ - stuinit_.size();
		dvaintot_ = 0 - res;
	}
	return res;
}

double ClassUnit::GetDavInAvgPoints() {
	dvaintot_ =  0;
	double sum = sum_points_in_sex_[male] + sum_points_in_sex_[female], avg = sum / stu_num_;
	double upper = course_.tot_avg_points_ + course_.dva_avg_points_, lower = course_.tot_avg_points_ - course_.dva_avg_points_;
	if (avg > upper)dvaintot_ = avg - upper;
	else if (avg < lower)dvaintot_ = lower - avg;
	return dvaintot_;
}

vector<Student*> ClassUnit::GetRandStuQue(set<Student*> stuque) {

	uniform_int_distribution<int> u(0, stuque.size() - 1);
	vector<Student*> res;
	for (auto s : stuque) {
		res.push_back(s);
	}
	int p;
	for (auto i = 0; i < res.size(); i++) {
		p = u(e_);
		if (p != i)swap(res[i], res[p]);
	}
	return res;
}

void ClassUnit::ModifySexRatio(vector<Pattern> patternque, int flag) {
	//ÿ��ѧ���ڽ�����ʱ�򶼻��н���������Ӱ��ֵ�������༶������+1���к���-1����Ӱ���Ϊ0
	//������ͳ�����еİ༶�Ľ����ܷ֣����������������Ĳ��ܽ��н���
	//int inpoint, outpoint;
	for (int i = 0; i < 2; i++) {
		auto s = static_cast<Sex>(i);
		if (taginsex_[s] || (flag == 1 && dvaintot_)) {
			if (taginsex_[s] > 0 || (flag == 1 && dvaintot_ > 0)) {
				int num = taginsex_[s];
				if (flag == 1) {
					int sexlow = static_cast<int>(stuinitsex_[s].size() - course_.sex_lower_[s]);
					num = min(sexlow, dvaintot_);
					if (num <= 0)continue;
				}
				//���Ա��������࣬����鿴ѧ���Ƿ��ܹ�������
				auto randque = GetRandStuQue(stuinitsex_[s]);
				//for (auto& stu : stuinitsex_[s]) {
				for(auto stu : randque){
					auto val = JudgeStuVal4SexRationIn(stu, patternque, flag);
					if (val.first > 0) {
						for (auto c : stu->clsset_) {
							c->GetStuOutCls(stu);
						}
						for (auto c : patternque[stu->patp_].path_[val.second]) {
							c->PutStuIntoCls(stu);
						}
						num--;
						if (flag == 1)dvaintot_--;
					}
					if (!num)break;
				}
			}
			else {
				//��Ҫ��������
				int num = 0 - taginsex_[s];
				if (flag == 1) {
					int sexup = static_cast<int>(course_.sex_upper_[s] - stunotinsex_[s].size());
					num = min(abs(dvaintot_), sexup);
					if (num <= 0)continue;
				}
				//���Ա���������,����鿴ÿ��ѧ��
				for (auto& stu : stunotinsex_[s]) {
					auto val = JudgeStuVal4SexRationOut(stu, patternque, flag);
					if (val.first > 0) {
						for (auto c : stu->clsset_) {
							c->GetStuOutCls(stu);
						}
						for (auto c : patternque[stu->patp_].path_[val.second]) {
							c->PutStuIntoCls(stu);
						}
						num--;
						if (flag == 1)dvaintot_++;
					}
					if (!num)break;
				}
			}
		}
	}
}

pair<int, int> ClassUnit::JudgeStuVal4SexRationIn(Student* s, vector<Pattern> patternque, int flag) {
	//�������Ҫע�����ֵ�ǰ����·����notint�����Ƿ����ͬ��������ͬ���൱�ڲ�����������0��
	int mxval = 0, tempval, mxpid = -1, temp;
	for (auto path : patternque[s->patp_].not_in_table_[this]) {
		/*vector<bool> intag(s->clsset_.size(), 0);
		vector<bool> notintag(s->clsset_.size(), 0);*/
		set<ClassUnit*> samecls;
		//�����ж��Ƿ��������ͬ�İ༶�Ĵ���
		for (auto i = 0; i < s->clsset_.size(); i++) {
			auto cptr = patternque[s->patp_].path_[path][i];
			auto it = s->clsset_.find(cptr);
			if (it != s->clsset_.end()) {
				//��ʾ�ҵ��˴�����ͬ�Ŀ�
				samecls.insert(*it);
			}
		}
		//�ڻ����������ͬ�γ�֮������������еİ༶����ѧ���Ľ����Ŀ���
		//�ȿ��쵱ǰ�����еİ༶
		tempval = 0;
		//��Ϊÿ����Ҫȥ���Ե�path�����ܻ�͵�ǰ���·���в�ͬ���ظ��İ༶����������ֻ��ÿ�ζ�����Щ�༶������һ��
		for (auto c : s->clsset_) {
			if (c == this)tempval += 1;
			else if (samecls.find(c) == samecls.end()) {
				temp = 0;
				temp += c->JudgeClsLoseStuInSex(s->sex_);
				if (flag)temp += c->JudgeClsLoseStuInTotAmount();
				tempval += GetTempVal(flag, temp);
			}
		}
		for (auto c : patternque[s->patp_].path_[path]) {
			if (samecls.find(c) == samecls.end()) {
				temp = 0;
				temp += c->JudgeClsGetStuInSex(s->sex_);
				if (flag)temp += c->JudgeClsGetStuInTotAmount();
				tempval += GetTempVal(flag, temp);
			}
		}
		if (tempval > mxval) {
			mxval = tempval;
			mxpid = path;
		}
	}
	return pair<int, int>(mxval, mxpid);
}

pair<int, int> ClassUnit::JudgeStuVal4SexRationOut(Student * s, vector<Pattern> patternque, int flag) {
	//�˴�Ҳ��Ҫ�԰༶���в���
	int mxval = 0, tempval, mxpid = -1, temp;
	for (auto path : patternque[s->patp_].in_unit_table_[this]) {
		set<ClassUnit*> samecls;
		for (auto c : patternque[s->patp_].path_[path]) {
			if (s->clsset_.find(c) != s->clsset_.end()) {
				samecls.insert(c);
			}
		}
		//������ظ��༶֮�������ٶ����еİ༶����ѧ�������Ŀ���
		tempval = 0;
		for (auto c : s->clsset_) {
			if (samecls.find(c) == samecls.end()) {
				temp = 0;
				temp += c->JudgeClsLoseStuInSex(s->sex_);
				if (flag)temp += c->JudgeClsLoseStuInTotAmount();
				tempval += GetTempVal(flag, temp);
			}
		}
		for (auto c : patternque[s->patp_].path_[path]) {
			if (c == this)tempval += 1;
			else if (samecls.find(c) == samecls.end()) {
				temp = 0;
				temp += c->JudgeClsGetStuInSex(s->sex_);
				if (flag)temp += c->JudgeClsGetStuInTotAmount();
				tempval += GetTempVal(flag, temp);
			}
		}
		if (tempval > mxval) {
			mxval = tempval;
			mxpid = path;
		}
	}
	return pair<int, int>(mxval, mxpid);
}

int ClassUnit::JudgeClsLoseStuInSex(Sex sex) {
	//�˴������õ������Զ����ķ�ʽ���Խ�����д���
	if (stuinitsex_[sex].size() > course_.sex_upper_[sex])return 1;
	if (stuinitsex_[sex].size() <= course_.sex_lower_[sex])return -1;
	return 0;
	
}

int ClassUnit::JudgeClsGetStuInSex(Sex sex) {
	if (stuinitsex_[sex].size() >= course_.sex_upper_[sex])return -1;
	if (stuinitsex_[sex].size() < course_.sex_lower_[sex])return 1;
	return 0;
}

int ClassUnit::JudgeClsLoseStuInTotAmount() {
	if (stuinit_.size() > course_.stu_upper_)return 1;
	if (stuinit_.size() <= course_.stu_lower_)return -1;
	else return 0;
}

int ClassUnit::JudgeClsGetStuInTotAmount() {
	if (stuinit_.size() >= course_.stu_upper_)return -1;
	if (stuinit_.size() < course_.stu_lower_)return 1;
	return 0;
}

int ClassUnit::JudgeClsGetOrLoseStuInAvgPoints(double points, int tag) {
	double newavg;
	if(tag == 0)newavg = (sum_points_ - points) / stuinit_.size();
	else newavg = (sum_points_ + points) / (stuinit_.size() + 1);
	double newdis = 0, olddis = 0;
	int ntag = 0, otag = 0;
	if (newavg > course_.avg_upper_) {
		newdis = newavg - course_.avg_upper_;
		ntag = 1;
	}
	else if (newavg < course_.avg_lower_) {
		newdis = course_.avg_lower_ - newavg;
		ntag = 1;
	}
	if (avg_sum_ > course_.avg_upper_) {
		olddis = avg_sum_ - course_.avg_upper_;
		otag = 1;
	}
	else if (avg_sum_ < course_.avg_lower_) {
		olddis = course_.avg_lower_ - avg_sum_;
		otag = 1;
	}
	if (ntag + otag == 0)return 0;
	if (olddis > newdis)return 1;
	else if (newdis > olddis)return -1;
}

//int ClassUnit::JudgeClsGetStuInAvgPoints(double points) {
//	double newavg = (sum_points_ + points) / (stuinit_.size() + 1);
//	double newdis = 0, olddis = 0;
//	int ntag = 0, otag = 0;
//	if (newavg > course_.avg_upper_) {
//		newdis = newavg - course_.avg_upper_;
//		ntag = 1;
//	}
//	else if (newavg < course_.avg_lower_) {
//		newdis = course_.avg_lower_ - newavg;
//		ntag = 1;
//	}
//	if (avg_sum_ > course_.avg_upper_) {
//		olddis = avg_sum_ - course_.avg_upper_;
//		otag = 1;
//	}
//	else if (avg_sum_ < course_.avg_lower_) {
//		olddis = course_.avg_lower_ - avg_sum_;
//		otag = 1;
//	}
//	if (ntag + otag == 0)return 0;
//	if (olddis > newdis)return 1;
//	else if (newdis > olddis)return -1;
//}

int ClassUnit::JudgeInOrOutInSex(Sex s) {
	//�ж������������Ա���Ӧ����ѡһ������༶���ǳ��༶��1��ʾѡ����룬0��ʾѡ���ȥ
	//-1��ʾ�����κβ���
	int tag = 0;
	if (stuinit_.size() >= course_.stu_lower_) {
		if (stuinitsex_[s].size() >= course_.sex_lower_[s])tag += 1;
	}
	if (stuinit_.size() <= course_.stu_upper_) {
		if (stuinitsex_[s].size() <= course_.sex_upper_[s])tag += 2;
	}
	if (tag == 3) {
		uniform_int_distribution<int> u(0, 1);
		return u(e_);
	}
	return tag - 1;
}

void ClassUnit::ModifyTotAmount(vector<Pattern> patternque) {

}

void ClassUnit::ModifyAvgPoints(vector<Pattern> patternque) {
	//���ݾ����ǳ�ƽ���ֻ��ǵ���ƽ�����������������˴������Ա�ֻ��ƽ����
	//���ֻ�ܼ�Ա�Ǿ�ͨ����Ա�����ֻ��ͨ�������Ǿͼ��ˣ�������ܼ�Ҳ�ܼ��Ǿ����ѡһ��
	//auto randinque = GetRandStuQue(stuinit_);
	//auto randoutque = GetRandStuQue(stunotin_);
	vector<Student*> randque;
	for (int i = 0; i < 2; i++) {
		auto s = static_cast<Sex>(i);
		auto key = JudgeInOrOutInSex(s);
		if (key == 1) {
			randque = GetRandStuQue(stunotinsex_[s]);
			
		}
		else if (key == 0) {
			randque = GetRandStuQue(stuinitsex_[s]);

		}
	}
}

int ClassUnit::GetCrashInSexRatio() {
	int res = 0;
	for (int i = 0; i < 2; i++) {
		auto s = static_cast<Sex>(i);
		if (stuinitsex_[s].size() > course_.sex_upper_[s])
			res += stuinitsex_[s].size() - course_.sex_upper_[s];
		else if (stuinitsex_[s].size() < course_.sex_lower_[s])
			res += course_.sex_lower_[s] - stuinitsex_[s].size();
	}
	//crash_ += res;
	return res;
}

int ClassUnit::GetCrashInTotAmount() {
	int res = 0;
	if (stuinit_.size() > course_.stu_upper_)
		res += stuinit_.size() - course_.stu_upper_;
	else if (stuinit_.size() < course_.stu_lower_)
		res += course_.stu_lower_ - stuinit_.size();
	//crash_ += res;
	return res;
}

double ClassUnit::GetCrashInAvgPoints() {
	int res = 0;
	/*double avg = (sum_points_in_sex_[male] + sum_points_in_sex_[female]) /
		(stuinitsex_[male].size() + stuinitsex_[female].size());*/
	//if (avg > course_.tot_avg_points_ + course_.dva_avg_points_)res += 1;
	//else if (avg < course_.tot_avg_points_ - course_.dva_avg_points_)res += 1;
	if (avg_sum_ > course_.avg_upper_) res += avg_sum_ - course_.avg_upper_;
	else if (avg_sum_ < course_.avg_lower_ )res += course_.avg_lower_ - avg_sum_;
	//crash_ += res;
	return res;
}

void ClassUnit::ModifyInMixedMode(vector<Pattern> patternque) {
	//�����˻��ģ�������жԵ����������ֱ��Ӧin��out���ߵĲ���
	//�������ܹ����������������˽��п��죬һֱ��������ϻ�����ȫ���������Ϊֹ
	//�ȶ��Ѿ����������˽��п���
	//auto randque = GetRandStuQue(stuinit_);
	auto randque = GetAllStuRandQue();
	pair<int, int> val;
	for (auto& stu : randque) {
		if (stuinit_.find(stu) != stuinit_.end()) {
			val = JudgeStuVal4Out(stu, patternque[stu->patp_]);
		}
		else {
			val = JudgeStuVal4In(stu, patternque[stu->patp_]);
		}
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

void ClassUnit::Mutate(vector<Pattern> patternque) {
	//���ѡ��һ��ѧ�������߳���ǰ����༶
	auto randque = GetAllStuRandQue();
	uniform_int_distribution<int> uq(0, randque.size() - 1);
	auto stu = randque[uq(e_)];
	if (stuinit_.find(stu) != stuinit_.end()) {		
		//ѧ�����ڸð༶��������Ҫ����ǰ�༶��
		//�ȴ�֮ǰ�İ༶��ȥ��
		StuOut2In(patternque, stu);
	}
	else {
		//ѧ���ڵ�ǰ�༶���У�Ȼ��Ҫ�������༶����ȥ
		//�ȴӵ�ǰ�İ༶��ȥ��
		StuIn2Out(patternque, stu);
	}
}

void ClassUnit::Cross(vector<Pattern> patternque) {
	//���ѡ��һ�����ڵ�ѧ����һ�������ѧ�����н���
	SelectStuIn2Out(patternque);
	SelectStuOut2In(patternque);
}

void ClassUnit::SelectStuIn2Out(vector<Pattern> patternque) {
	vector<Student*> randque = GetRandStuQue(stuinit_);
	uniform_int_distribution<int> us(0, randque.size() - 1);
	auto stu = randque[us(e_)];
	StuIn2Out(patternque, stu);
}

void ClassUnit::SelectStuOut2In(vector<Pattern> patternque) {
	vector<Student*> randque = GetRandStuQue(stunotin_);
	uniform_int_distribution<int> us(0, randque.size() - 1);
	auto stu = randque[us(e_)];
	StuOut2In(patternque, stu);
}

void ClassUnit::StuIn2Out(vector<Pattern> patternque, Student* stu) {
	for (auto c : stu->clsset_) {
		c->GetStuOutCls(stu);
	}
	uniform_int_distribution<int> up(0, patternque[stu->patp_].not_in_table_[this].size() - 1);
	auto pid = up(e_);
	for (auto c : patternque[stu->patp_].path_[pid]) {
		c->PutStuIntoCls(stu);
	}
}

void ClassUnit::StuOut2In(vector<Pattern> patternque, Student* stu) {
	for (auto c : stu->clsset_) {
		c->GetStuOutCls(stu);
	}
	uniform_int_distribution<int> up(0, patternque[stu->patp_].in_unit_table_[this].size() - 1);
	auto pid = up(e_);
	for (auto c : patternque[stu->patp_].path_[pid]) {
		c->PutStuIntoCls(stu);
	}
}

pair<int, int> ClassUnit::JudgeStuVal4Out(Student * stu, Pattern pattern) {
	//�������Ҫע�����ֵ�ǰ����·����notint�����Ƿ����ͬ��������ͬ���൱�ڲ�����������0��
	int mxval = 0, tempval, mxpid = -1;
	//for (auto path : patternque[s->patp_].not_in_table_[this]) {
	for(auto path : pattern.not_in_table_[this]){
		/*vector<bool> intag(s->clsset_.size(), 0);
		vector<bool> notintag(s->clsset_.size(), 0);*/
		set<ClassUnit*> samecls;
		//�����ж��Ƿ��������ͬ�İ༶�Ĵ���
		for (auto i = 0; i < stu->clsset_.size(); i++) {
			//auto cptr = patternque[s->patp_].path_[path][i];
			auto cptr = pattern.path_[path][i];
			auto it = stu->clsset_.find(cptr);
		//for(auto c : stu->clsset_){
			if (it != stu->clsset_.end()) {
				//��ʾ�ҵ��˴�����ͬ�Ŀ�
				samecls.insert(*it);
			}
		}
		//�ڻ����������ͬ�γ�֮������������еİ༶����ѧ���Ľ����Ŀ���
		//�ȿ��쵱ǰ�����еİ༶
		tempval = 0;
		//��Ϊÿ����Ҫȥ���Ե�path�����ܻ�͵�ǰ���·���в�ͬ���ظ��İ༶����������ֻ��ÿ�ζ�����Щ�༶������һ��
		for (auto c : stu->clsset_) {
			//if (c == this)tempval += 1;
			//else if (samecls.find(c) == samecls.end()) {
			if (samecls.find(c) == samecls.end()) {
				tempval += c->JudgeClsLoseStuInSex(stu->sex_);
				tempval += c->JudgeClsLoseStuInTotAmount();
				tempval += c->JudgeClsGetOrLoseStuInAvgPoints(stu->points_[c->course_], 0);
			}
		}
		//for (auto c : patternque[s->patp_].path_[path]) {
		for(auto c : pattern.path_[path]){
			if (samecls.find(c) == samecls.end()) {
				tempval += c->JudgeClsGetStuInSex(stu->sex_);
				tempval += c->JudgeClsGetStuInTotAmount();
				tempval += c->JudgeClsGetOrLoseStuInAvgPoints(stu->points_[c->course_], 1);
			}
		}
		if (tempval > mxval) {
			mxval = tempval;
			mxpid = path;
		}
	}
	return pair<int, int>(mxval, mxpid);
}

pair<int, int> ClassUnit::JudgeStuVal4In(Student* stu, Pattern pattern) {
	//�˴�Ҳ��Ҫ�԰༶���в���
	int mxval = 0, tempval, mxpid = -1;
	for (auto path : pattern.in_unit_table_[this]) {
		set<ClassUnit*> samecls;
		for (auto c : pattern.path_[path]) {
			if (stu->clsset_.find(c) != stu->clsset_.end()) {
				samecls.insert(c);
			}
		}
		//������ظ��༶֮�������ٶ����еİ༶����ѧ�������Ŀ���
		tempval = 0;
		for (auto c : stu->clsset_) {
			if (samecls.find(c) == samecls.end()) {
				tempval += c->JudgeClsLoseStuInSex(stu->sex_);
				tempval += c->JudgeClsLoseStuInTotAmount();
				tempval += c->JudgeClsGetOrLoseStuInAvgPoints(stu->points_[c->course_], 0);
			}
		}
		for (auto c : pattern.path_[path]) {
			//if (c == this)tempval += 1;
			//else if (samecls.find(c) == samecls.end()) {
			if (samecls.find(c) == samecls.end()) {
				tempval += c->JudgeClsGetStuInSex(stu->sex_);
				tempval += c->JudgeClsGetStuInTotAmount();
				tempval += c->JudgeClsGetOrLoseStuInAvgPoints(stu->points_[c->course_], 1);
			}
		}
		if (tempval > mxval) {
			mxval = tempval;
			mxpid = path;
		}
	}
	return pair<int, int>(mxval, mxpid);
}

vector<Student*> ClassUnit::GetAllStuRandQue() {
	vector<Student*> que;
	for (auto& stu : stuinit_) {
		que.push_back(stu);
	}
	for (auto& stu : stunotin_) {
		que.push_back(stu);
	}
	uniform_int_distribution<int> u(0, que.size() - 1);
	int p;
	for (auto i = 0; i < que.size(); i++) {
		p = u(e_);
		if (p != i)swap(que[i], que[p]);
	}
	return que;
}

void ClassUnit::GetAvlPatQue(vector<Pattern*>& avlpatque) {
	map<Pattern*, bool>::iterator it = patterns_.begin();
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
}
