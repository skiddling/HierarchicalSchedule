#include "ga.h"
#include <gtest\gtest.h>

int rooms, groups;
map<string, int> coursemap;
map<string, Course> courses;
vector<Student> stuque;
vector<Teacher> teacherque;

void StudentsIn() {
	ifstream fin("students.txt");
	int stunum;
	fin >> stunum;
	stuque = vector<Student>(stunum);
	string stuname, stuid;
	int cnum;
	Course *cp = NULL;
	Student *sp = NULL;
	for (int i = 0; i < stunum; i++) {
		fin >> stuname >> stuid >> cnum;
		vector<string> courseque = vector<string>(cnum);
		vector<Course> stucourses = vector<Course>(cnum);
		for (int j = 0; j < cnum; j++) {
			fin >> courseque[j];
			if (coursemap.find(courseque[j]) == coursemap.end()) {
				coursemap[courseque[j]] = coursemap.size();//学科序号产生
				courses[courseque[j]] = *(new Course(coursemap[courseque[j]], courseque[j])); 
			}
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
	fin >> rooms >> groups;
	fin.close();
}

void Input() {
	BasicInput();
	StudentsIn();
	TeachersIn();
}

int main() {
	srand((unsigned int)time(0));
	Input();
	GA ga(rooms, groups, stuque, teacherque, coursemap, courses);
	system("PAUSE");
	return 0;
}