#include "schedule.h"

//��schedule���������Ĳ������Ƕ�scheduleֱ�ӵ����Խ��и����޸�
const double kMXMP = 0.1;
const double kMXMPG = 0.2;
const double kMXCP = .035;
const double kSTEP = 1.3;

int Schedule::stu_lower_ = 0;
int Schedule::stu_upper_ = 0;
int Schedule::rooms_ = 0;
int Schedule::groups_ = 0;
double Schedule::po_cross_ = 0.2;
double Schedule::po_mutate_gene_ = 0.1;
double Schedule::po_mutate_ = 0.01;
const double Schedule::mx_pmutate_ = 0.05;
const double Schedule::mx_pmutate_gene_ = 0.1;
const double Schedule::con_pcross_ = 0.2;
const double Schedule::con_pmutate = 0.01;
const double Schedule::con_pmutate_gene_ = 0.1;
const int Schedule::kTableTimeOut = 1000000;

Schedule::Schedule() {
}

Schedule::Schedule(vector<Course> cou_que, vector<Student> stu_que, vector<Teacher> tea_que,
	map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, map<vector<Course>, int> prefix_map,
	vector<Prefix> prefixes, vector<int> topo_sorted):
	cou_que_(cou_que), stu_que_(stu_que), tea_que_(tea_que), pattern_map_(pattern_map),
	pattern_que_(pattern_que), prefix_map_(prefix_map), prefixes_(prefixes), topo_sorted_(topo_sorted){
	table_ = vector<Group>(groups_, *(new Group(rooms_, rooms_)));
}

//��ÿ���α�������Ӧ�ĳ�ʼ���������ص�������ɿα�
void Schedule::Init() {
	//new method for new version
	//get local stus address for pattern
	GetStusAddrs();

	//1.������ÿһ�ڿ�
	GetTeaCls();
	//2.�����α�Ҳ���ǽ���ʦ�����еĿζ����䵽�α���ȥ
	//int t1 = clock(), t2;
	auto t1 = chrono::steady_clock::now(), t2 = t1;
	//5��������ǿ����޸ĵģ�Ӧ�õ���һ���������������˴���ʱ����Ϊ5mi��n
	chrono::duration<int, ratio<60, 1>> dur(5);
	while (1) {
		Schedule temps(*this);
		if (!temps.GetRanTab()) {
			*this = temps;
			break;
		}
		//t2 = clock();
		t2 = chrono::steady_clock::now();
		if (t2 - t1 > dur) {
			success_falg_ = 0;
			return;
		}
	}
	success_falg_ = 1;
}

//��ÿ����ʦ��ÿһ�ڿζ����д���
void Schedule::GetTeaCls() {
	map<Course, int> ::iterator it;
	int cid;
	ClassUnit *cp;
	for (int i = 0; i < tea_que_.size(); i++) {
		it = tea_que_[i].courses_num_.begin();
		while (it != tea_que_[i].courses_num_.end()) {
			//����һ�ſ����еİ༶������
			cid = it->first.course_id_;
			for (int j = 0; j < it->second; j++) {
				//�����µĿΣ����ź�λ��
				cls_nuit_que_.push_back(*(new ClassUnit(tea_que_[i], cou_que_[cid], cls_nuit_que_.size())));
			}
			//������һ�ſ�Ŀ�ĿεĲ���
			it++;
		}
	}
	int tid;
	//�����еĿζ��Ѿ����ź�λ��֮����ܼ�¼�����ε�pos����Ȼ������ӿε�;����vetcot�޸�λ�ö����¼�¼ʧЧ
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		tid = cls_nuit_que_[i].teacher_.teacher_id_;
		cid = cls_nuit_que_[i].course_.course_id_;
		tea_que_[tid].units_que_.push_back(&cls_nuit_que_[i]);
		cou_que_[cid].units_.push_back(&cls_nuit_que_[i]);
	}
	//cout << "end of get units" << endl;
}

vector<vector<int> > Schedule::GetAvlTime(int cid) {
	int pid;
	//0���б�ʾ�ܵĺͣ�1��ʾ�����Ѿ�����֮��ĺ�
	vector<vector<int> > avl = vector<vector<int> >(2, vector<int>(groups_, 0));
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		pid = cou_que_[cid].prefixes_[i];
		for (int j = 0; j < groups_; j++) {
			if (prefixes_[pid].avl_time_[j]) {
				avl[0][j] = 1;
			}
			if (!cou_que_[cid].satisfied_[i])
			//if(!prefixes_[pid].satisfied_)
				//������ڲ�������ô1���вŻ�������֣��������0
				avl[1][j] += prefixes_[pid].avl_time_[j];
		}
	}
	//cout << "end of get avl time" << endl;
	return avl;
}

bool Schedule::GetTimeList(int tag, map<GroupUnit, vector<int>>& timelist, vector<vector<int>> avl, vector<bool> trtimes) {
	GroupUnit tgu;
	bool flag = 1;
	for (int i = 0; i < groups_; i++) {
		if (avl[tag][i] && trtimes[i]) {
			flag = 0;
			tgu.leave_ = table_[i].leave_;
			tgu.times_ = avl[tag][i];
			timelist[tgu].push_back(i);
		}
	}
	return flag;
}

int Schedule::GetUnitTime(int cid, int uid, vector<vector<int>> avl) {
	//�Ƚ����������ʦ����ʱ�����룬�ó�һ���ɰ���ʱ���б�	
	//avl�Ǹÿγ���Ҫ�����ŵ�ʱ��,trtimes����ʦ�͸����������ṩ��ʱ��
	vector<bool> trtimes = vector<bool>(groups_, 0);
	int tid = cou_que_[cid].units_[uid]->teacher_.teacher_id_;
	for (int i = 0; i < groups_; i++) {
		//trtimes[i] = tea_que_[tid].avl_time_[i] && table_[i].avl;
		if (tea_que_[tid].avl_time_[i] && table_[i].avl && (cou_que_[cid].not_in_set_.find(i) == cou_que_[cid].not_in_set_.end()))
			trtimes[i] = 1;
	}

	int flag = 1;//��ʾ���ѡ���ĸ�ʱ���
	//�ȶ�Ϊ����Ŀν������㣬�����Ϊ��ʦ�Ϳ�����Ĺ�ϵ�޷��ҵ�ʱ�䣬��ô���ٿ���������ʦ��������
	int k = 0, pid;
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		//pid = cou_que_[cid].prefixes_[i];
		//if (prefixes_[pid].satisfied_)k++;
		if (cou_que_[cid].satisfied_[i])k++;
	}
	cou_que_[cid].satis_num_ = k;
	int tag = k != cou_que_[cid].prefixes_.size() ? 1 : 0;
	//key��ʾ�����������ڶ��ٸ�ǰ׺��Ҫ���㣬vector<int>��ʾ�ھ�����Щʱ���
	map<GroupUnit, vector<int> > timelist;
	vector<int> tque;
	//�����Ƿ���ڲ������ǰ׺
	//ǰ׺���ڲ�����tag = 1, �����tag = 0
	flag = GetTimeList(tag, timelist, avl, trtimes);
	if (flag && tag == 1) GetTimeList(0, timelist, avl, trtimes);
	if (flag)return -1;
	tque = timelist.begin()->second;
	//cout << "end of get unit time" << endl;
	return tque[rand() % tque.size()];
}

void Schedule::AssignUnit(int gid, ClassUnit *cup) {
	//���¸���͸ýڿε���Ϣ
	int cpos = table_[gid].cpos_;
	//cup->unit_time_ = make_pair(gid, cpos);
	table_[gid].AddUnit(cup);
	//������ʦ��Ϣ
	//cup->teacher_.avl_time_[gid] = 0;
	tea_que_[cup->teacher_.teacher_id_].avl_time_[gid] = 0;
	//�Ըÿ�Ŀ��ǰ׺���и��²���
	Course cou = cup->course_;
	vector<Course> temppat;
	int pid, npid, cid = cup->course_.course_id_;
	map<vector<int>, bool>::iterator ita;
	Prefix tp;
	vector<int> temptime;
	for (int i = 0; i < cou.prefixes_.size(); i++) {
		pid = cou.prefixes_[i];
		tp = prefixes_[pid];
		//��ǰ׺�ܱ�����
		if (tp.avl_time_[gid] && cou_que_[cid].satisfied_[i] == 0) {
		//if (tp.avl_time_[gid] && tp.satisfied_ == 0){
			cou_que_[cid].satisfied_[i] = 1;
			//prefixes_[pid].satisfied_ = 1;
			cou_que_[cid].satis_num_++;
		}
		for (ita = tp.avl_tab_.begin(); ita != tp.avl_tab_.end(); ita++) {
			//�����и�ʱ����пգ���ô�Ͱ����ʱ����õ�
			if((ita->first)[gid] == 1){
				temppat = prefixes_[pid].pattern_;
				temppat.push_back(cou);//�γ��µĺ�׺					
				if (prefix_map_.find(temppat) != prefix_map_.end()) {
					//�����һ��ǰ׺�����
					npid = prefix_map_[temppat];
				}
				else npid = -1;
				//temptime��ǰ׺�Ŀ���ʱ��
				temptime = ita->first;
				//����ʱ����õ�
				temptime[gid] = 0;
				if (npid > 0 && prefixes_[npid].avl_tab_.find(temptime) == prefixes_[npid].avl_tab_.end()) {
					prefixes_[npid].avl_tab_[temptime] = 1;
					for (int k = 0; k < groups_; k++) {
						if (temptime[k] == 1)
							prefixes_[npid].avl_time_[k] = 1;
					}
				}
			}
		}
	}
	//cout << "end of assign time" << endl;
}

bool Schedule::CheckPrefix(int cid) {
	int pid;
	for (int i = 0; i < cou_que_[cid].prefixes_.size(); i++) {
		//pid = cou_que_[cid].prefixes_[i];
		//if (!prefixes_[pid].satisfied_)return true;
		if (!cou_que_[cid].satisfied_[i])return true;
	}
	return false;
}

bool Schedule::GetRanTab() {
	vector<vector<int> > avl;
	int pos;
	for (int i = 0; i < cou_que_.size(); i++){
		for (int j = 0; j < cou_que_[i].units_.size(); j++) {
			//�����ÿ�Ŀ�����е�ǰ׺�����������ʱ��
			avl = GetAvlTime(i);
			//��øýڿ�Ӧ�����ĸ�ʱ���group������
			pos = GetUnitTime(i, j, avl);
			cout << pos << "   ";
			if (pos < 0)return 1;
			AssignUnit(pos, cou_que_[i].units_[j]);
		}
		if (CheckPrefix(i))return 1;
		cout << "course  " << i << endl;
	}
	cout << "end of the rand table" << endl;
	return 0;
}

void Schedule::GetAllPath() {
	//�ǽ�ÿ���α�ľ������ݶ��ռ����������ÿ���α�����е�·��
	//�˴����ص��Ƕ�ÿ��pattern���в���
	for (int i = 0; i < groups_; i++) {
		table_[i].GetCouSet(i);
	}
	for (int i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].GetAllPath(table_);
	}
	/*map<ClassUnit*, vector<vector<int> > > ::iterator it;
	for (int i = 0; i < pattern_que_.size(); i++) {
		it = pattern_que_[i].notin_path_combos_.begin();
		for (int j = 0; j < it->second.size(); j++) {
			if (it->second[j].size() > 1)cout << i << " " << j << endl;
		}
	}*/
	cout << "end of get not in table" << endl;
}

void Schedule::StuAssign() {
	for (int i = 0; i < pattern_que_.size(); i++) {
		//pattern_que_[i].StuAssign();
		pattern_que_[i].AssignStus();//new method for new version
	}

}

void Schedule::CalCrashFitness() {
	crash_ = 0;
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		/*if (cls_nuit_que_[i].stu_num_ > stu_upper_)crash_ += cls_nuit_que_[i].stu_num_ - stu_upper_;
		else if (cls_nuit_que_[i].stu_num_ < stu_lower_)crash_ += stu_lower_ - cls_nuit_que_[i].stu_num_;*/
		if (cls_nuit_que_[i].stu_num_ > cls_nuit_que_[i].course_.stu_upper_)crash_ += cls_nuit_que_[i].stu_num_ - cls_nuit_que_[i].course_.stu_upper_;
	}
	fitness = 1.0 / static_cast<double>(1 + crash_);
	cout << "crash is                                                  " << crash_ << "            " << fitness << endl;
}

void Schedule::Mutate(double mxfit) {
	//����ÿ��pattern���в���
	double r, mp;
	int psize = pattern_que_.size();
	for (int i = 0; i < psize; i++) {
		r = static_cast<double> (rand() * rand()) / kRndPluRnd;
		mp = po_mutate_ * mxfit / fitness;
		mp = min(mp, mx_pmutate_);
		//r = 0;
		if (r < mp) {
			mp = po_mutate_gene_ * mxfit / fitness;
			mp = min(mp, mx_pmutate_gene_);
			pattern_que_[i].Mutate(mp);
		}
	}
}

void Schedule::Cross(double mxfit) {
	//����ÿ��pattern���в���
	double r, cp;
	int psize = pattern_que_.size();
	for (int i = 0; i < psize; i++) {
		r = static_cast<double>(rand() * rand()) / kRndPluRnd;
		cp = po_cross_ * mxfit / fitness;
		cp = min(cp, kMXCP);
		//r = 0;
		cout << i << endl;
		if (r < cp) {
			cout << "cross " << i << " pattern" << endl;
			//r = static_cast<double>(rand() * rand()) / kRndPluRnd;
			pattern_que_[i].Cross();
		}
	}
}

void Schedule::Modify() {
	ResetStuNum();
	GetStuNum();
	int snum = 0;
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		snum += cls_nuit_que_[i].stu_num_;
	}
	cout << snum << endl;
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		cls_nuit_que_[i].GetPatStusNum();
	}
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		snum = cls_nuit_que_[i].stu_num_;
		//if (snum < stu_lower_)cls_nuit_que_[i].Modify(0);
		//if (snum > stu_upper_)cls_nuit_que_[i].Modify(1);
		if (snum > cls_nuit_que_[i].course_.stu_upper_)cls_nuit_que_[i].Modify(1);
		/*if (snum < stu_lower_)cls_nuit_que_[i].IncreaseStuNum();
		if (snum > stu_upper_)cls_nuit_que_[i].DecreaseStuNum();*/
	}
}

void Schedule::GetStuNum() {
	for (int i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].PutStuDown2Cls();
	}
}



void Schedule::GetResult() {
	//��ÿ��pattern���е�ѧ�����䵽ÿ���༶����
	for (int i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].AssignStuDown2Cls(cls_nuit_que_);
	}
}

void Schedule::OutPutResult() {
	ofstream fout("result.txt");
	int sum = 0;
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		cls_nuit_que_[i].OutPutStu(fout);
		sum += cls_nuit_que_[i].students_.size();
	}
	fout << sum;
	fout.close();
}

void Schedule::ResetStuNum() {
	for (int i = 0; i < cls_nuit_que_.size(); i++) {
		cls_nuit_que_[i].ResetStuData();
	}
}

void Schedule::GetAllAvlStus() {
	//��ÿ���඼��������п����ܹ�����ð༶��ѧ�����������ȥ��ѧ�����з����Լ������Ĺ���
	/*for (auto i = 0; i < table_.size(); i++) {
		table_[i].GetAllAvlStus();
	}*/
	for (auto& t : table_) {
		t.GetAllAvlStus();
	}
}

void Schedule::GetStusAddrs() {
	//�����֮ǰ�ļ�¼
	for (auto i = 0; i < pattern_que_.size(); i++) {
		pattern_que_[i].stu_que_.clear();
	}
	//�ٸ��µ�ǰѧ����¼
	for (auto i = 0; i < stu_que_.size(); i++) {
		pattern_que_[stu_que_[i].patp_].stu_que_.push_back(&stu_que_[i]);
	}
}

void Schedule::GetSchedule() {
	auto t1 = chrono::system_clock::now(), t2 = t1;
	chrono::duration<int, ratio<60, 1> > dur(5);
	for (auto i = 0; i < modifyfuncs.size(); i++) {
		while (true) {
			if (t2 - t1 > dur)return;
			this->calfitfuncs[i];
			//Mutate();
			if (crash_ == 0)break;
			this->modifyfuncs[i];
		}
	}
}

void Schedule::GetFunctions() {
	//����function����������ʵ�ִ���
	modifyfuncs.push_back(&Schedule::ModifySexRatio);
	modifyfuncs.push_back(&Schedule::ModifyTotAmount);
	modifyfuncs.push_back(&Schedule::ModifyAvgPoints);
	calfitfuncs.push_back(&Schedule::CalSexRatio);
	calfitfuncs.push_back(&Schedule::CalTotAmount);
	calfitfuncs.push_back(&Schedule::CalAvgPoints);
}

void Schedule::ModifySexRatio() {
	for (auto i = 0; i < cls_nuit_que_.size(); i++) {
		if (cls_nuit_que_[i].taginsex_[male]|| cls_nuit_que_[i].taginsex_[female])
			cls_nuit_que_[i].ModifySexRatio(pattern_que_);
	}
}

void Schedule::ModifyTotAmount() {

}

void Schedule::ModifyAvgPoints() {

}

void Schedule::CalSexRatio() {
	crash_ = 0;
	for (auto i = 0; i < cls_nuit_que_.size(); i++) {
		crash_ += cls_nuit_que_[i].GetDvaInSex();
	}
	fitness = 1.0 / (1 + crash_);
}

void Schedule::CalTotAmount() {

}

void Schedule::CalAvgPoints() {

}
