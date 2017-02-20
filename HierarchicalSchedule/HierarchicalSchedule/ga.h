#pragma once
//#include "schedule.h"
#include "InterruptibleThread.h"

class GA
{
public:
	static const int kScheduleSize_ = 20;//������Ⱥ����
	//const int kCheckTimeOut = 1000000;//����һ�ž��п����ԵĿα��ʱ������
	static int rooms_, groups_;
	static double step;
	int cnt_;//cnt������������
	double mxfit_;//��һ�ֵ���������Ӧ��
	static int stu_upper_, stu_lower_;//һ���༶��ѧ������������
	vector<Course> cou_que_;//���ڴ�����еĿ�Ŀ
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;
	Schedule result_;//�����
	vector<Schedule> schedules_[2];//ÿ��schedule����һ������


	map<vector<Course>, int> patterns_map_;//����ģʽ���ܱ�����ģʽ��ѯ
	vector<Pattern> patterns_;//���е�ѡ��ģʽ���У���Ҫ��������

	map<vector<Course>, int> prefix_map_;//���ں�prefix_����,����ָ��vector<Prefix>�±�
	vector<Prefix> prefixes_;//���е�ǰ׺ģʽ

	vector<double> fits;
	//vector<ClassUnit> cls_units_;//���еĿ�

	vector<int> topo_sorted_;//����Ŀ������������֮��Ŀ�Ŀ��˳��

	GA(vector<Student> stu_que, vector<Teacher> tea_que, vector<Course> cou_que_);
	bool Generate();//���ɿα�
	void OutPutResult();
	void GetResult();

private:
	void InitSort();//����ʦ��ѧ���Ŀ�Ŀ��������
	void GetStuPat();//��õ�ǰ����ѧ����ģʽ���
	void GetPrefixes();
	void DFS(int k);//���������е�һ����
	void TopoSort();//�����еĿ�Ŀ������������
	void OutPutTable();
	void Mutate();
	void Cross();
	void Modify();
	void Select();
	void CalCrash();
	bool Init();
};