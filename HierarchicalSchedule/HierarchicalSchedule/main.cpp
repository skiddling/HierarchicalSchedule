//#include "ga.h"
#include "dbutil.h"

int rooms, groups, stuupper, stulower;
map<string, Course> courses;
vector<Course> couque;
vector<Student> stuque;
vector<Teacher> teacherque;

void StudentsIn() {
	ifstream fin("students.txt");
	int stunum;
	fin >> stunum;
	stuque = vector<Student>(stunum);
	string stuname, stuid;
	int cnum;
	for (int i = 0; i < stunum; i++) {
		fin >> stuname >> stuid >> cnum;
		vector<string> courseque = vector<string>(cnum);
		vector<Course> stucourses = vector<Course>(cnum);
		for (int j = 0; j < cnum; j++) {
			fin >> courseque[j];
			stucourses[j] = courses[courseque[j]];
		}
		stuque[i] = *(new Student(stuid, stuname, stucourses));
		stuque[i].student_no = i;
	}
	fin.close();
}

void TeachersIn() {
	ifstream fin("teachers.txt");
	string teacherid, teachername, coursename;
	int coursesnum, coursetimes, k = 0, tid;
	while (fin >> teacherid) {
		tid = atoi(teacherid.c_str());
		fin >> teachername >> coursesnum;
		map<Course, int> coursestable;
		vector<Course> courseque = vector<Course> (coursesnum);
		for (int i = 0; i < coursesnum; i++) {
			fin >> coursename >> coursetimes;
			courseque[i] = courses[coursename];
			cout << coursestable.size() << "  " << courseque[i].course_name_ << endl;
			//coursestable[courseque[i]] = coursetimes;
			coursestable.insert(make_pair(courseque[i], coursetimes));
			cout << coursestable.size() << endl;
		}
		teacherque.push_back(*(new Teacher(groups, teacherid, teachername, courseque ,coursestable)));
		(teacherque.end() - 1)->teacher_id_ = k++;
	}
	fin.close();
}

void BasicInput() {
	ifstream fin("basic.txt");
	int cnum, cls, notin, temp;
	fin >> rooms >> groups >> cnum >> stuupper >> stulower;
	string cname;
	for(int i = 0; i < cnum; i++){
		fin >> cname >> cls >> stuupper >> notin;
		set<int> notinset;
		for (int j = 0; j < notin; j++) {
			fin >> temp;
			notinset.insert(temp);
		}
		couque.push_back(*(new Course(cname, cls, cnum, stuupper, notinset)));
		//courses[cname] = *(new Course(cname, cls, cnum));
	}
	//确定了每门课的序号
	sort(couque.begin(), couque.end());
	for (int i = 0; i < couque.size(); i++) {
		couque[i].course_id_ = i;
		courses[couque[i].course_name_] = couque[i];
	}
	fin.close();
}

void Input() {
	BasicInput();
	StudentsIn();
	TeachersIn();
	for (int i = 0; i < stuque.size(); i++)
		sort(stuque[i].courses_.begin(), stuque[i].courses_.end());
	for (int i = 0; i < teacherque.size(); i++) {
		sort(teacherque[i].courses_.begin(), teacherque[i].courses_.end());
	}
}

void OutPut() {
	ofstream fout("temp.txt");
	for (int i = 0; i < stuque.size(); i++) {
		fout << stuque[i].student_name_ << "\t";
		for (int j = 0; j < stuque[i].courses_.size(); j++)
			fout << stuque[i].courses_[j].course_name_ << "\t";
		fout << endl;
	}
	fout.close();
}

void SetRunTime() {
	GA::rooms_ = rooms;
	GA::groups_ = groups;
	GA::stu_lower_ = stulower;
	GA::stu_upper_ = stuupper;
	Schedule::rooms_ = rooms;
	Schedule::groups_ = groups;
	Schedule::stu_lower_ = stulower;
	Schedule::stu_upper_ = stuupper;
	ClassUnit::stu_lower_ = stulower;
	ClassUnit::stu_upper_ = stuupper;
	Pattern::stu_lower_ = stulower;
	Pattern::stu_upper_ = stuupper;
}

int main(int argc, char* argv[]) {
	
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	//srand((unsigned int)time(0));
	//Input();
	//SetRunTime();
	////OutPut();
	////return 0;
	//GA ga(stuque, teacherque, couque);
	//if (ga.Generate()) {
	//	ga.GetResult();
	//	ga.OutPutResult();
	//}
	//else cout << "failed gernerate table" << endl;
	system("PAUSE");
	return 0;
}