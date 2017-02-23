#pragma once
#include "student.h"

class ClassUnit;
class Group;

//单独一个模式的定义
//是进行学生分配的主题对象，要进行分学生的遗传算法的交叉变异等操作
class Pattern
{
public:
	default_random_engine e_;
	int pattern_id_;//该模式的id
	int stu_num_;//该模式下所有的学生人数
	static int stu_upper_, stu_lower_;
	vector<Course> course_que_;
	vector<Student* >stu_que_;

	map<ClassUnit*, vector<int> > in_unit_table_;//存在于某个班级的所有的序列
	map<ClassUnit*, vector<int> > not_in_table_;//不放在某个课的所有其他可能的课的序列
	map<ClassUnit*, vector<vector<int> > > notin_path_combos_;//每个combo当中组合的路径，也就是路径当中没有重复的班级的路径之和

	vector<int> stu_num_in_que_;//记载了每条路径当中学生人数,mutate和corss只会修改这个值
	//modify修改完之后其实也就是修改了这个值，而且cls有延迟操作，每次都还要重这里重新读取数据

	vector<vector<ClassUnit* > > path_;//在课表当中能够实现该模式的所有的路径
	vector<bool> chosen_path_tab_;//表示有哪些路径被选中了
	set<ClassUnit* > unit_set_;//记录所有可能经历过的教室
	vector<int> avl_num_each_path_;
	int avl_sum_, combosid;
	//map<pair<int, int>, vector<int> > not_in_table_;//没有经历过的路径形成的表

	Pattern();
	Pattern(vector<Course> course_que, int stu_num = 1);
	void GetAllPath(vector<Group> table);
	void StuAssign();
	void Mutate(double mp);
	void Cross();
	void PutStuDown2Cls();
	int GetAvlStuNum(ClassUnit* cp, bool tag);
	void ModifyStuNum(bool tag, ClassUnit* cp, int neednum);
	void AssignStuDown2Cls(vector<ClassUnit> &clsque);
	void DecreaseStuNum(int pid, int stunum);
	void IncreaseStuNum(int pid, int stunum);
	//new method for new version
	void AssignStus();
	void GetRandAry(vector<int>& ary);
	void PutStuIntoCls(int pid, int sid);

	pair<int, int> GetMxStuNum(ClassUnit* cp);

	bool operator == (const Pattern &a)const {
		return course_que_ == a.course_que_;
	}

	bool operator < (const Pattern &a)const {
		return course_que_ < a.course_que_;
	}
	
	bool operator != (const Pattern &a)const {
		return course_que_ != a.course_que_;
	}

private:
	void DFS(int gid, vector<bool> visited, vector<ClassUnit* > path, vector<Group> table);
	void GetRandTab(vector<int> &ary);
	inline int GetRandId(int pos);
	inline void Update(int oid, int iid, int num);
	void SwapStu(int oid);
	void GetNotInTable();
	void GetPathCombination();
	void GetEachCombo(ClassUnit* cp, set<ClassUnit* > units, int pos, vector<int> que);
	bool GetIsIn(ClassUnit *cp, set<ClassUnit* > units, int pid);
	int GetMaxAvlStus(ClassUnit *cp);
};

inline int Pattern::GetRandId(int pos) {
	int id, sz = path_.size();
	do {
		id = rand() % sz;
		cout << pattern_id_ << "  " << id << ' ' << sz << endl;
	} while (id == pos);
	cout << "end of get rand id" << endl;
	return id;
}

inline void Pattern::Update(int oid, int iid, int num) {
	stu_num_in_que_[oid] -= num;
	if (!stu_num_in_que_[oid])chosen_path_tab_[oid] = false;
	stu_num_in_que_[iid] += num;
	chosen_path_tab_[iid] = true;
}
