#include "student.h"
#include "pattern.h"
#include "classunit.h"

Student::Student() {
}

Student::Student(string student_id, string student_name, vector<Course> courses, string sex):
	student_id_(student_id), student_name_(student_name), courses_(courses){
	sex_ = (sex == "ÄĞ" ? male : female);
}

void Student::GetCouSort() {
	sort(courses_.begin(), courses_.end());
}

void Student::IntoCls(ClassUnit * clsptr) {
	clsset_.insert(clsptr);
}

void Student::OutCls(ClassUnit * clsptr) {
	clsset_.erase(clsptr);
}

double Student::GetCouPoints(ClassUnit * cls) {
	return points_[cls->course_];
}
