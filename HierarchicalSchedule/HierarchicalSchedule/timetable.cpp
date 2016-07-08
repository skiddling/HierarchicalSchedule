#include "timetable.h"

TimeTable::TimeTable(int rows, int cols, vector<Teacher> teachers, vector<ClassUnit> class_units_que) :
	teachers_(teachers), class_units_que_(class_units_que){
	time_table_ = vector<vector<ClassUnit *> >(rows, vector<ClassUnit *>(cols));
	GetRandTable(rows, cols);
}

void TimeTable::GetRandTable(int rows, int cols) {
	int quelens = rows * cols;
	vector<pair<int, int> > randque = vector<pair<int, int> > (quelens);
	for (int id, i = 0; i < quelens; i++) {
		id = rand() % quelens;
		if (id != i) swap(randque[id], randque[i]);
	}
}