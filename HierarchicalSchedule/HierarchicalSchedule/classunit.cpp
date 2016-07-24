#include "classunit.h"

ClassUnit::ClassUnit(int unit_id) : unit_id_(unit_id){
	alterable_ = 1;
}

ClassUnit::ClassUnit(int unit_id, pair<int, int> unit_time) {
}

ClassUnit::ClassUnit(Teacher teacher, Course course, int unit_id):
	teacher_(teacher), course_(course), unit_id_(unit_id){
}

void ClassUnit::init() {
}
