#pragma once
#include "timetable.h"

class Schedule
{
public:
	int rooms_, groups_;
	//map<string, bool> cou_id_map_;
	//map<string, Course> cou_name_map_;
	//set<Course> cou_set_;//���ڴ�����еĿ�Ŀ�����
	vector<Course> cou_que_;//���ڴ�����еĿ�Ŀ
	vector<Student> stu_que_;
	vector<Teacher> tea_que_;

	map<vector<Course>, int> pattern_map_;//��¼����ѧ���Ŀγ�ģʽ
	vector<Pattern> pattern_que_;

	vector<ClassUnit> cls_nuit_que_;//��ǰ���п��γɵĶ���
	TimeTable time_table_;//��ǰ��clsunitque�γɵĿα�

	map<vector<Course>, int> prefix_map_;//���ں�prefix_����
	vector<Prefix> prefixes_;//���е�ǰ׺ģʽ

	vector<int> topo_sorted_;//��Ŀ�����������

	Schedule();
	Schedule(int rooms, int groups, vector<Course> cou_que, vector<Student> stu_que, 
		vector<Teacher> tea_que, map<vector<Course>, int> pattern_map, vector<Pattern> pattern_que, 
		map<vector<Course>, int> prefix_map, vector<Prefix> prefixes, vector<int> topo_sorted);
	void Init();//���ɿα�

private:
	void GetTeaCls();//������ʦ�Ŀγ�ָ��
	//�����α�
	bool GetRanTab();
	//����������б�
	void MakeTabRand(vector<vector<int> > &table);
	//���ĳ����Ŀ�Ŀɰ���ʱ��
	vector<vector<int> > GetAvlTime(int cid);
	int GetUnitTime(int cid, int uid, vector<vector<int> > avl);
};