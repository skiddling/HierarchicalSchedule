#include "group.h"

Group::Group() {
}

Group::Group(int rooms, int leave):rooms_(rooms), leave_(leave){
	group = vector<ClassUnit *>(rooms_);
}