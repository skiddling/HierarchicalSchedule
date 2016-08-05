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
	//0��ʾ������������Ҫ�ӵ�ǰ������ģʽ���г�ȡһ��������ѧ����ƽ������༶������
	//1��ʾ�������࣬��Ҫ�ѵ�ǰ��ѧ��������һ���ֵ������İ༶����
	//����ÿ��ģʽ���ԣ�����һ�����ڵ�ǰ�İ༶�ı�ֻ�е��ð༶����Щģʽ���д��ڿ����滻�İ༶�б���ܽ����ģʽѡ��
	int neednum = tag ? (stu_num_ - stu_upper_) : (stu_lower_ - stu_num_);
	vector<Pattern* > avlpatque;
	map<Pattern*, bool>::iterator it = patterns_.begin();
	map<int, int> pid2id;
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
	//���ݾ�����������������ǳ��������з�������
	//map<Pattern*, int> avlstunum;
	vector<int> avlnumpat = vector<int>(avlpatque.size(), 0);
	int temp, avlstusum = 0;
	for (int i = 0; i < avlpatque.size(); i++) {
		temp = avlpatque[i]->GetAvlStuNum(this, tag);
		if (temp) {
			//avlstunum[avlpatque[i]] = temp;
			avlnumpat[i] = temp;
			avlstusum += temp;
		}
	}
	//�����Ӧ��������ô�����ѡ���������������������ȫ����������������
	/*if (tag)DecreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);
	else IncreaseStuNum(neednum, avlstusum, avlstunum, avlnumpat);*/
	int i = 0;
	vector<int> needused = vector<int>(avlpatque.size(), 0);
	if (avlstusum > neednum) {
		while (neednum) {
			if (avlnumpat[i]) {
				temp = rand() % (avlnumpat[i] + 1);
				avlnumpat[i] -= temp;
				needused[i] = temp;
				neednum -= temp;
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
	
	//��ÿ��ģʽ������Ա�ķ���
	it = patterns_.begin();
	for (i = 0; i < avlpatque.size(); i++, it++) {
		if (needused[i]) {
			it->first->ModifyStuNum(tag, this, needused[i]);
		}
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
	fout << course_.course_name_ << string(' ', 11 - course_.course_name_.length()) << teacher_.teacher_name_ << endl;
	fout << unit_time_.first << "  " << unit_time_.second << endl;
	for (int i = 0; i < students_.size(); i++) {
		fout << students_[i]->student_name_ << string(' ', 11 - students_[i]->student_name_.length()) << "  " << students_[i]->student_id_ << endl;
	}
	fout << endl << endl;
}

void ClassUnit::GetAvlPatQue(vector<Pattern*>& avlpatque) {
	map<Pattern*, bool>::iterator it = patterns_.begin();
	for (; it != patterns_.end(); it++) {
		if (it->first->not_in_table_.find(this) != it->first->not_in_table_.end()) {
			avlpatque.push_back(it->first);
		}
	}
}
