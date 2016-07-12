#pragma once

#include "timetable.h"

class SummaryTable
{
public:
	map<Pattern, int> pattern_map_;//记录所有学生的课程模式
	vector<Pattern> pattern_que_;
	SummaryTable();
};