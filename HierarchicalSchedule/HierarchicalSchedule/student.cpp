#include "student.h"

Student::Student() {
}

Student::Student(string student_id, string student_name, vector<Course> courses):
	student_id_(student_id), student_name_(student_name), courses_(courses){
}

void Student::GetCouSort() {
	sort(courses_.begin(), courses_.end());
}