#include "ga.h"
#include <gtest\gtest.h>

int rooms, groups;
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
	int coursesnum, coursetimes, k = 0;
	while (fin >> teacherid) {
		fin >> teachername >> coursesnum;
		map<Course, int> coursestable;
		vector<Course> courseque = vector<Course> (coursesnum);
		for (int i = 0; i < coursesnum; i++) {
			fin >> coursename >> coursetimes;
			courseque[i] = courses[coursename];
			coursestable[courseque[i]] = coursetimes;
		}
		teacherque.push_back(*(new Teacher(teacherid, teachername, courseque ,coursestable)));
		(teacherque.end() - 1)->teacher_no = k++;
	}
	fin.close();
}

void BasicInput() {
	ifstream fin("basic.txt");
	int cnum;
	fin >> rooms >> groups >> cnum;
	string cname;
	for(int i = 0; i < cnum; i++){
		fin >> cname;
		courses[cname] = *(new Course(cname, cnum));
	}
	map<string, Course>::iterator it = courses.begin();
	for (int i = 0; it != courses.end(); it++, i++) {
		it->second.course_id_ = i;
		couque.push_back(it->second);
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
		for (int j = 0; j < stuque[i].courses_.size(); j++)
			fout << stuque[i].courses_[j].course_name_ << "\t";
		fout << endl;
	}
	fout.close();
}

int main() {
	srand((unsigned int)time(0));
	Input();
	//OutPut();
	//return 0;
	GA ga(rooms, groups, stuque, teacherque, couque);
	
	system("PAUSE");
	return 0;
}