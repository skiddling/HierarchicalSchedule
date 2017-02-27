#pragma once
#include "pattern.h"

class ClassUnit
{
public:
	//int crash_;//�ð༶�ĳ�ͻֵ
	int stu_num_;//�ð༶ѧ������
	int unit_id_;
	//static int stu_upper_, stu_lower_;
	int stu_upper_, stu_lower_;
	bool alterable_;//�Ƿ���Ҫ�����ſΣ�1������Ҫ��0������Ҫ,����Ҫ���ŵİ༶ʱ��;����Ͽ�ѧ�������̶�
	pair<int, int> unit_time_;//��ʼ����ʱ�����������ҵľ�����Ͽ�ʱ��
	Teacher teacher_;//ÿ���Ͽν��Ҷ�����һ����ʦ��û�о���NULL
	Course course_;//ÿ���ϿεĽ��Ҷ�����һ���ϿεĿ�Ŀ��û�о���NULL
	vector<Student*> students_;//��������������Ѿ����ڵ�ѧ�����������°汾�����ò�̫��
	map<Pattern*, bool> patterns_;//�����浱ǰ�༶�ж��ٸ�patterns����getallpath�Ĺ����вɼ�����Ϣ

	//����������������Ҫ��modify֮����յģ����ж��ϵ��Ǹ�stunum
	map<Pattern*, int> patterns_stus_;//ÿ��patterns�ж��ٸ�ѧ��
	map<Pattern*, map<int, int> > pat_path_stus_num_;//ÿ��pat���е�ÿ��·���¸��ж��ٵ�ѧ����Ҳ���Ǽ�¼�˾���ÿ��ѧ������Դ���ĸ�pat���ĸ�·������
	map<Pattern*, map<int, int> > selected_stus_;//����Ҫ��ѧ���ֳ�ȥ��ʱ��������ѡ�е�ѧ��

	ClassUnit(int unit_id, int stu_num = 0);
	ClassUnit(int unit_id, pair<int, int> unit_time, int stu_num = 0);
	ClassUnit(Teacher teacher, Course course, int unit_id, int stu_num = 0);
	void init();
	void Modify(bool tag);
	inline int GetCrash();
	void OutPutStu(ofstream &fout);
	void GetPatStusNum();
	void ResetStuData();
	//new obj and method for new version
	//vector<Student*> allavlnotinstus_;//�����ܹ�������༶����Ŀǰû��������༶��ѧ��
	set<Student*> stunotin_;//�����ܹ�������༶����Ŀǰû��������༶��ѧ��
	map <Sex, set<Student*> > stunotinsex_;
	set<Student*> stuinit_;//������������ҵ�ѧ��
	map<Sex, set<Student*> > stuinitsex_;
	/*int male_stu_num_;
	int female_stu_num_;*/
	map<Sex, int> stu_num_in_sex_;//��ʵҲ���Բ���
	/*double avg_male_stu_num_;
	double avg_female_stu_num_;*/
	map<Sex, double> sum_points_in_sex_;//��¼��ͬ�Ա��ܷ�
	double avg_sum_;

	void GetAllAvlStus();
	void PutStuIntoCls(Student* stu);

private:
	void GetAvlPatQue(vector<Pattern* > &avlpatque);
	void IncreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat);
	void DecreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat);
	void GetSelectedStus(map<Pattern*, int> patused);
	void AddStu2Path(map<Pattern*, int> patused);
};

inline int ClassUnit::GetCrash() {
	//if (stu_num_ < stu_lower_)return stu_lower_ - stu_num_;
	//if (stu_num_ > stu_upper_)return stu_num_ - stu_upper_;
	if (stu_num_ > course_.stu_upper_)return stu_num_ - course_.stu_upper_;
}
