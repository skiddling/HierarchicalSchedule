#pragma once
#include "pattern.h"

class ClassUnit
{
public:
	int crash_;//该班级的冲突值
	int stu_num_;//该班级学生数量
	int unit_id_;
	//static int stu_upper_, stu_lower_;
	int stu_upper_, stu_lower_;
	bool alterable_;//是否需要进行排课，1代表需要，0代表不需要,不需要安排的班级时间和具体上课学生都被固定
	pair<int, int> unit_time_;//初始化的时候分配这个教室的具体的上课时间
	Teacher teacher_;//每个上课教室都会有一个老师，没有就是NULL
	Course course_;//每个上课的教室都会有一个上课的科目，没有就是NULL
	vector<Student*> students_;//用来保存这个班已经存在的学生，可能在新版本当中用不太上
	map<Pattern*, bool> patterns_;//用来存当前班级有多少个patterns，在getallpath的过程中采集到信息

	//以下三个数据是需要在modify之后被清空的，还有顶上的那个stunum
	map<Pattern*, int> patterns_stus_;//每个patterns有多少个学生
	map<Pattern*, map<int, int> > pat_path_stus_num_;//每个pat当中的每个路径下各有多少的学生，也就是记录了具体每个学生的来源于哪个pat的哪个路径当中
	map<Pattern*, map<int, int> > selected_stus_;//当需要将学生分出去的时候用来放选中的学生

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
	//vector<Student*> allavlnotinstus_;//所有能够在这个班级但是目前没有在这个班级的学生
	set<Student*> stunotin_;//所有能够在这个班级但是目前没有在这个班级的学生
	map <Sex, set<Student*> > stunotinsex_;
	set<Student*> stuinit_;//所有在这个教室的学生
	map<Sex, set<Student*> > stuinitsex_;
	map<Sex, int> taginsex_;//表示是否性别比不对
	int dvaintot_;//总人数的偏差
	/*int male_stu_num_;
	int female_stu_num_;*/
	map<Sex, int> stu_num_in_sex_;//其实也可以不用
	/*double avg_male_stu_num_;
	double avg_female_stu_num_;*/
	map<Sex, double> sum_points_in_sex_;//记录不同性别总分
	map<Sex, double> avg_points_in_sex_;
	double avg_sum_, sum_points_;//班级平均分和总分
	default_random_engine e_;

	void GetAllAvlStus();
	void PutStuIntoCls(Student* stu);
	void GetStuOutCls(Student* stu);
	int GetDvaInSex();
	int GetDavInTotAmount();
	double GetDavInAvgPoints();
	vector<Student*> GetRandStuQue(set<Student*> stuque);
	void ModifySexRatio(vector<Pattern> patternque, int flag);
	void ModifyTotAmount(vector<Pattern> patternque);
	void ModifyAvgPoints(vector<Pattern> patternque);
	//new method for mixed mode version
	int GetCrashInSexRatio();
	int GetCrashInTotAmount();
	int GetCrashInAvgPoints();
	void ModifyInMixedMode(vector<Pattern> patternque);

private:
	void GetAvlPatQue(vector<Pattern* > &avlpatque);
	void IncreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat);
	void DecreaseStuNum(int neednum, int avlstusum, map<Pattern*, int> avlstunum, vector<int> avlnumpat);
	void GetSelectedStus(map<Pattern*, int> patused);
	void AddStu2Path(map<Pattern*, int> patused);
	//new method for new version
	pair<int, int> JudgeStuVal4SexRationIn(Student* s, vector<Pattern> patternque, int flag);
	pair<int, int> JudgeStuVal4SexRationOut(Student* s, vector<Pattern> patternque, int flag);
	int JudgeClsLoseStuInSex(Sex sex);
	int JudgeClsGetStuInSex(Sex sex);
	int JudgeClsLoseStuInTotAmount();
	int JudgeClsGetStuInTotAmount();
	int JudgeClsGetOrLoseStuInAvgPoints(double points, int tag);
	//int JudgeClsGetStuInAvgPoints(double points);
	int JudgeInOrOutInSex(Sex s);
	inline int GetTempVal(int flag, int temp);
	//new method for mixed mode
	pair<int, int> JudgeStuVal4Out(Student* s, Pattern pattern);
	pair<int, int> JudgeStuVal4In(Student* s, Pattern pattern);
	vector<Student*> GetAllStuRandQue();
};

inline int ClassUnit::GetCrash() {
	//if (stu_num_ < stu_lower_)return stu_lower_ - stu_num_;
	//if (stu_num_ > stu_upper_)return stu_num_ - stu_upper_;
	if (stu_num_ > course_.stu_upper_)return stu_num_ - course_.stu_upper_;
}

inline int ClassUnit::GetTempVal(int flag, int temp) {
	if (flag) {
		if (temp == 2)return 1;
		else return -1;
	}
	else return temp;
}
