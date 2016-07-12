#include "teacher.h"

Teacher::Teacher(string teacher_id, string teacher_name, vector<Course> courses, map<Course, int> courses_num) :
	teacher_id_(teacher_id), teacher_name_(teacher_name), courses_(courses), courses_num_(courses_num){
}