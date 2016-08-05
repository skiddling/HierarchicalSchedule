#include "ga.h"

const int kTimeOut = 3650000;
const int kOnceTimeOut = 1800000;
const int kLocTimeOut = 180000;

int GA::rooms_ = 0;
int GA::groups_ = 0;
int GA::stu_lower_ = 0;
int GA::stu_upper_ = 0;
double GA::step = 1.3;

GA::GA(vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que) :
	stu_que_(stu_que), tea_que_(tea_que), cou_que_(cou_que){

	mxfit_ = 0.0;
	fits = vector<double>(kScheduleSize_, 0.0);
	topo_sorted_ = vector<int>(cou_que_.size());
	prefixes_.push_back(*(new Prefix(groups_)));
	InitSort();

	//1.�������ѧ����ģʽ����
	GetStuPat();
	GetPrefixes();
	//��Ϊ�Ѿ��Կ�Ŀ������ˣ����Բ���Ҫ�������������������
	//TopoSort();
}

//������ѧ�����л�����е�ģʽ������ͳ�Ƴ����е�ģʽ���ж��ٵ�ѧ��
void GA::GetStuPat() {
	for (int i = 0; i < stu_que_.size(); i++) {
		if (patterns_map_.find(stu_que_[i].courses_) == patterns_map_.end()) {
			patterns_.push_back(*(new Pattern(stu_que_[i].courses_)));
			patterns_map_[stu_que_[i].courses_] = patterns_.size() - 1;
		}
		else patterns_[patterns_map_[stu_que_[i].courses_]].stu_num_++;
		stu_que_[i].patp_ = patterns_map_[stu_que_[i].courses_];
		//patterns_[patterns_map_[stu_que_[i].courses_]].stu_que_.push_back(stu_que_[i].student_no);
		patterns_[patterns_map_[stu_que_[i].courses_]].stu_que_.push_back(&stu_que_[i]);
	}
	//�Եõ������е�ģʽ���������Ա��ڽ��л��ǰ׺�Ĳ���
	sort(patterns_.begin(), patterns_.end());
	for (int i = 0; i < patterns_.size(); i++) {
		patterns_[i].pattern_id_ = i;
	}
}

void GA::GetPrefixes() {
	for (int i = 0; i < patterns_.size(); i++) {
		vector<Course> cque;
		int len = patterns_[i].course_que_.size(), iid, oid, cid;
		for (int j = 0; j < len - 1; j++) {
			cque.push_back(patterns_[i].course_que_[j]);
			cid = patterns_[i].course_que_[j + 1].course_id_;
			if (prefix_map_.find(cque) == prefix_map_.end()) {
				prefixes_.push_back(*(new Prefix(cque, prefixes_.size(), groups_)));
				prefix_map_[cque] = prefixes_.size() - 1;
			}
			//���ÿ����Ŀ������ǰ׺
			if (cou_que_[cid].prefix_set_.find(prefix_map_[cque]) == cou_que_[cid].prefix_set_.end()) {
				cou_que_[cid].prefixes_.push_back(prefix_map_[cque]);
				cou_que_[cid].prefix_set_.insert(prefix_map_[cque]);
			}
		}
		for (int j = 1; j < len; j++) {
			iid = patterns_[i].course_que_[j - 1].course_id_;
			oid = patterns_[i].course_que_[j].course_id_;
			if (!cou_que_[oid].pre_node_[iid]) {
				cou_que_[oid].pre_node_[iid] = 1;
				cou_que_[oid].pre_node_que_.push_back(&cou_que_[iid]);
			}
		}
	}
	for (int i = 0; i < cou_que_.size(); i++) {
		if (!cou_que_[i].prefixes_.size()) {
			cou_que_[i].prefixes_.push_back(0);
		}
		cou_que_[i].satisfied_ = vector<bool>(cou_que_[i].prefixes_.size(), 0);
	}
	//����Щ��Ŀ����Ϣ�����µ�ÿ����ʦ����ȥ
	int cid;
	for (int i = 0; i < tea_que_.size(); i++) {
		for (int j = 0; j < tea_que_[i].courses_.size(); j++) {
			cid = tea_que_[i].courses_[j].course_id_;
			tea_que_[i].courses_[j] = cou_que_[cid];
		}
	}
	//��Ҫ������ǰ׺���ݸ��Ե�ǰ׺���Ƚ�������Ҳ���ǿ�Ŀ��ǰ׺
	cout << "end of get prefix" << endl;
}

//��ѧ������ʦ�Ŀ�Ŀ��������
void GA::InitSort() {
	//sort(cou_que_.begin(), cou_que_.end());
	for (int i = 0; i < tea_que_.size(); i++) {
		tea_que_[i].GetCouSort();
	}
	for (int i = 0; i < stu_que_.size(); i++) {
		stu_que_[i].GetCouSort();
	}
}

void GA::DFS(int k) {
	cou_que_[k].visited_ = 1;
	for (int i = 0; i < cou_que_[k].pre_node_que_.size(); i++) {
		if (!cou_que_[k].pre_node_que_[i]->visited_)
			DFS(cou_que_[k].pre_node_que_[i]->course_id_);
	}
	topo_sorted_[cnt_++] = k;
}

void GA::TopoSort() {
	for (int i = 0; i < cou_que_.size(); i++) {
		if (!cou_que_[i].visited_) {
			DFS(i);
		}
	}
}

void GA::OutPutTable() {
	ofstream fout("randtable.txt");
	string cname, tname;
	for (int k = 0; k < kScheduleSize_; k++) {
		for (int i = 0; i < groups_; i++) {
			for (int j = 0; j < rooms_; j++) {
				cname = schedules_[0][k].table_[i].group[j]->course_.course_name_;
				fout << cname << string(11 - cname.length(), ' ');
			}
			fout << endl;
			for (int j = 0; j < rooms_; j++) {
				tname = schedules_[0][k].table_[i].group[j]->teacher_.teacher_name_;
				fout << tname << string(11 -  tname.length(), ' ');
			}
			fout << endl;
		}
		fout << endl << endl << endl;
	}
	fout.close();
}

void GA::Mutate() {
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[0][i].Mutate(mxfit_);
	}
}

void GA::Cross() {
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[0][i].Cross(mxfit_);
	}
}

void GA::Modify() {
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[0][i].Modify();
	}
}

void GA::Select() {
	mxfit_ = 0.0;
	fits[0] = schedules_[0][0].fitness;
	for (int i = 1; i < kScheduleSize_; i++) {
		fits[i] = schedules_[0][i].fitness + fits[i - 1];
		if (mxfit_ < schedules_[0][i].fitness)mxfit_ = schedules_[0][i].fitness;
		if (schedules_[0][i].crash_ < result_.crash_)result_ = schedules_[0][i];
	}
	for (int i = 0; i < kScheduleSize_; i++) {
		fits[i] /= fits.back();
	}
	double r = static_cast<double>(rand() * rand()) / kRndPluRnd;
	int id;
	for (int i = 0; i < kScheduleSize_; i++) {
		r = static_cast<double>(rand() * rand()) / kRndPluRnd;
		id = lower_bound(fits.begin(), fits.end(), r) - fits.begin();
		schedules_[1][i] = schedules_[0][id];
	}
	schedules_[0] = schedules_[1];
}

void GA::CalCrash() {
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[0][i].CalCrashFitness();
	}
}

bool GA::Init() {
	//�����������Ѿ���ɣ�ʣ�¾��Ƕ�ÿ��schedule��������
	//2.�Ȱ�ÿһ�ڿγ�ʼ����Ȼ�����ɿα�
	//��ʦ�����Ѿ����ˣ�����ÿ����ʦ�Ŀλ������ڣ�����Ҫ����ÿ����ʦ�Ŀ�
	result_ = *(new Schedule(cou_que_, stu_que_, tea_que_, patterns_map_, patterns_, prefix_map_, prefixes_, topo_sorted_));
	schedules_[0] = vector<Schedule>(kScheduleSize_, result_);
	schedules_[1] = schedules_[0];
	//result_.Init();
	for (int i = 0; i < kScheduleSize_; i++) {
		schedules_[0][i].Init();
	}
	cout << "end of the get rand table" << endl;
	//OutPutTable();
	int successnum = 0;
	for (int i = 0; i < kScheduleSize_; i++) {
		successnum += schedules_[0][i].success_falg_;
	}
	if (successnum == 0)return true;
	//��ʼ��ʽ����ѧ��
	//cout << successnum << endl;
	for (int i = 0; i < kScheduleSize_; i++) {
		if (schedules_[0][i].success_falg_) {
			schedules_[0][i].GetAllPath();
			cout << "end of get all path" << endl;
			schedules_[0][i].StuAssign();
			schedules_[0][i].CalCrashFitness();
			if (schedules_[0][i].fitness > mxfit_)mxfit_ = schedules_[0][i].fitness;
			//cout << i << endl;
		}
	}
	//׼��������ɣ���ʼ�Ŵ��㷨���岿��
	cout << "start for GA" << endl;
	result_.crash_ = INT_MAX;
	return false;
}

bool GA::Generate() {
	if (Init()) {
		cout << "failed to create table" << endl;
		return false;
	}
	//t1��ʾ�ʼʱ�䣬t2��ʾ��ǰʱ�䣬t3��ʾ�ϴν�չ��ʱ��,oncestart���ϴ�һ�ֵ�ʱ��
	int t1 = clock(), t2 = t1, t3 = t1, oncestart = t1;
	int mxoff = kOnceTimeOut;
	int precrash = INT_MAX;
	while (t2 - t1 < kTimeOut) {
		Mutate(), cout << "end of mutate" << endl;
		Cross(), cout << "end of cross" << endl;
		Modify(), cout << "end of modify" << endl;
		CalCrash();
		Select();
		//�õ���
		if (result_.crash_ == 0)break;
		t2 = clock();
		//����ʱ�䳬�涨
		if (t2 - t1 > kTimeOut)break;
		//�н�һ����ý�չ�����ø���
		if (precrash != result_.crash_) {
			precrash = result_.crash_;
			Schedule::po_mutate_ = Schedule::con_pmutate;
			Schedule::po_mutate_gene_ = Schedule::con_pmutate_gene_;
			t3 = clock();
		}
		//����ֲ����Ž�
		if (t2 - t3 > kLocTimeOut) {
			Schedule::po_mutate_ *= GA::step;
			Schedule::po_mutate_gene_ *= GA::step;
			Schedule::po_mutate_ = min(Schedule::mx_pmutate_, Schedule::po_mutate_);
			Schedule::po_mutate_gene_ = min(Schedule::mx_pmutate_gene_, Schedule::po_mutate_gene_);
			t3 = clock();
		}
		//��̫�����߳�����������ʱ�䣬����������Ⱥ
		if (t2 - oncestart > kOnceTimeOut) {
			if (Init()) {
				cout << "failed to create table" << endl;
				return false;
			}
			oncestart = clock();
			t3 = oncestart;
		}
	}
	return true;
}

void GA::OutPutResult() {
	//��ÿ�����ѧ�������������
	result_.OutPutResult();
}

void GA::GetResult() {
	//�˴�������res���в���
	result_.GetResult();
}