#pragma once

#include "timetable.h"

class SummaryTable
{
public:
	map<Pattern, int> pattern_map_;//��¼����ѧ���Ŀγ�ģʽ
	vector<Pattern> pattern_que_;
	SummaryTable();
};