#pragma once
#include "group.h"

class InterruptibleThread;

class Schedule
{
public:
	static const double mx_pcross_;
	static const double mx_pmutate_;
	static const double mx_pmutate_gene_;
	static const double con_pmutate ;
	static const double con_pmutate_gene_;
	static const double con_pcross_;
	static const int kTableTimeOut;//����һ�ž��п����ԵĿα��ʱ������

	double fitness;//��Ӧֵ
	static double po_mutate_;//��������ʱ��ı������
	static double po_mutate_gene_;//��������ʱ����������
	static double po_cross_;//��������ʱ��Ľ������
	static int stu_upper_, stu_lower_;//ѧ���༶��������
	double crash_;//��ͻֵ��������������
	//int crash_;//��ͻֵ
	int success_falg_;
	static int rooms_, groups_;
	int outtime_;//����ʱ��

	vector<Course> cou_que_;//���ڴ�����еĿ�Ŀ
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;

	map<vector<Course>, int> pattern_map_;//��¼����ѧ���Ŀγ�ģʽ
	vector<Pattern> pattern_que_;

	vector<ClassUnit> cls_nuit_que_;//��ǰ���п��γɵĶ���
	vector<Group> table_;//�α�

	map<vector<Course>, int> prefix_map_;//���ں�prefix_����
	vector<Prefix> prefixes_;//���е�ǰ׺ģʽ

	vector<int> topo_sorted_;//��Ŀ�����������

	Schedule();
	Schedule(vector<Course> cou_que, vector<Student> stu_que, vector<Teacher> tea_que, 
		map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, 
		map<vector<Course>, int> prefix_map, vector<Prefix> prefixes, vector<int> topo_sorted);
	void Init(int outtime);//���ɿα�
	void GetAllPath();//�������ģʽ���Ͽ�·��
	void StuAssign();//��ʼ������ѧ������
	void CalCrashFitness();//�����ͻֵ
	void Mutate(double mxfit);
	void Cross(double mxfit);
	void Modify();
	void GetResult();
	void OutPutResult();
	void ResetStuNum();
	//new method for new version
	void GetAllAvlStus();
	void GetStusAddrs();
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	//new method for debug
	void Test();

private:
	void GetTeaCls();//������ʦ�Ŀγ�ָ��
	//�����α�
	bool GetRanTab();
	//���ĳ����Ŀ�Ŀɰ���ʱ��
	vector<vector<int> > GetAvlTime(int cid);
	int GetUnitTime(int cid, int uid, vector<vector<int> > avl);
	void AssignUnit(int gid, ClassUnit *cup);
	bool GetTimeList(int tag, map<GroupUnit, vector<int> > &timelist, vector<vector<int> > avl, vector<bool> trtimes);
	bool CheckPrefix(int cid);
	void GetStuNum();

	//new method for new version	
	typedef void(Schedule::*FPtr)();
	vector<FPtr> modifyfuncs;
	vector<FPtr> calfitfuncs;
	void GetFunctions();
	void ModifySexRatio();
	void ModifyTotAmount();
	void ModifyAvgPoints();
	void CalSexRatio();
	void CalTotAmount();
	void CalAvgPoints();
	void CalFitnessInMixedMode();
	void MutateInMixedMode();
	void CrossInMixedMode();
	void ModifyInMixedMode();
	//new obj for mixed mode
	default_random_engine e_;
};