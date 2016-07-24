#include "group.h"

Group::Group() {
}

Group::Group(int rooms, int leave, bool av):rooms_(rooms), leave_(leave), avl(av){
	group = vector<ClassUnit *>(rooms_);
}

void Group::AddUnit(ClassUnit *up) {
	group[cpos_] = up;
	cpos_++;
	leave_--;
	if (leave_ == 0)avl = 0;
}
