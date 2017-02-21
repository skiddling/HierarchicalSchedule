#include "student.h"
#include "pattern.h"

Student::Student() {
}

Student::Student(string student_id, string student_name, vector<Course> courses, string sex):
	student_id_(student_id), student_name_(student_name), courses_(courses){
	sex_ = (sex == "ÄĞ" ? male : female);
}

void Student::GetCouSort() {
	sort(courses_.begin(), courses_.end());
}