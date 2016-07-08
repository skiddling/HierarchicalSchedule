#include "classunit.h"

ClassUnit::ClassUnit(int unit_id) : unit_id_(unit_id){
	alterable_ = 1;
	init();
}

ClassUnit::ClassUnit(int unit_id, pair<int, int> unit_time) :
	unit_id_(unit_id), unit_time_(unit_time) {
	alterable_ = 0;
	init();
}

void ClassUnit::init() {
	teacher_ = NULL;
	course_ = NULL;
	students_ = vector<Student *>(0);
}