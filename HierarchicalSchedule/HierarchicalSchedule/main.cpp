//#include "ga.h"
#include "dbutil.h"

int rooms, groups, stuupper, stulower;
map<string, Course> courses;
vector<Course> couque;
vector<Student> stuque;
vector<Teacher> teacherque;

void YuErGaoInput() {
	ifstream fin("students.txt");
	int recordnum;
	fin >> recordnum;
	string stuname, stuid, sex, cname;
	double point;
	map<string, int> stumap;
	for (auto i = 0; i < recordnum; i++) {
		fin >> stuid >> stuname >> point >> cname;
		if (stumap.find(stuid) == stumap.end()) {
			auto stuptr = new Student(stuid, stuname);
			stuptr->student_no = stuque.size();
			stuque.push_back(*stuptr);
			//stuque.push_back(*(new Student(stuid, stuname));
			//stumap[stuid]->student_no = stuque.size() - 1;
			stumap.insert(make_pair(stuid, stuptr->student_no));
		}
		//stumap[stuid]->courses_.push_back(courses[cname]);
		//stumap[stuid]->points_[courses[cname]] = point;
		stuque[stumap[stuid]].courses_.push_back(courses[cname]);
		stuque[stumap[stuid]].points_[courses[cname]] = point;
		couque[courses[cname].course_id_].num_of_stus_in_sex_[stuque[stumap[stuid]].sex_]++;
		couque[courses[cname].course_id_].total_scores_in_sex_[stuque[stumap[stuid]].sex_] += point;
	}
	for (auto& c : couque) {
		c.tot_avg_points_ = (c.total_scores_in_sex_[male] + c.total_scores_in_sex_[female]) /
			(c.num_of_stus_in_sex_[male] + c.num_of_stus_in_sex_[female]);
		c.avg_lower_ = c.tot_avg_points_ - c.dva_avg_points_;
		c.avg_upper_ = c.tot_avg_points_ + c.dva_avg_points_;
	}
	fin.close();
}

void StudentsIn() {
	ifstream fin("students.txt");
	int stunum;
	fin >> stunum;
	stuque = vector<Student>(stunum);
	string stuname, stuid, sex;
	int cnum;
	double point;
	for (int i = 0; i < stunum; i++) {
		fin >> stuname >> stuid >> sex >> cnum;
		vector<string> courseque = vector<string>(cnum);
		vector<Course> stucourses = vector<Course>(cnum);
		map<Course, double> points;
		for (int j = 0; j < cnum; j++) {
			fin >> courseque[j];
			stucourses[j] = courses[courseque[j]];
			//fin >> stucourses[j].points_;
			fin >> point;
			points[stucourses[j]] = point;
			
		}
		stuque[i] = *(new Student(stuid, stuname, stucourses, sex));
		stuque[i].points_ = points;
		stuque[i].student_no = i;
		for (auto c : stucourses) {
			/*if (sex == "男")couque[c.course_id_].male_stu_num_++;
			else couque[c.course_id_].female_stu_num_++;*/
			if (sex == "男") {
				couque[c.course_id_].num_of_stus_in_sex_[male]++;
				couque[c.course_id_].total_scores_in_sex_[male] += stuque[i].points_[c];
			}
			else {
				couque[c.course_id_].num_of_stus_in_sex_[female]++;
				couque[c.course_id_].total_scores_in_sex_[female] += stuque[i].points_[c];
			}
		}
	}
	for (auto& c : couque) {
		/*for (int i = 0; i < 2; i++) {
			auto s = static_cast<Sex>(i);
			c.avg_scores_in_sex_[s] = c.total_scores_in_sex_[s] / c.num_of_stus_in_sex_[s];
		}*/
		c.tot_avg_points_ = (c.total_scores_in_sex_[male] + c.total_scores_in_sex_[female]) /
			(c.num_of_stus_in_sex_[male] + c.num_of_stus_in_sex_[female]);
		c.avg_lower_ = c.tot_avg_points_ - c.dva_avg_points_;
		c.avg_upper_ = c.tot_avg_points_ + c.dva_avg_points_;
	}

	fin.close();
}

void TeachersIn() {
	ifstream fin("teachers.txt");
	//string teacherid, teachername, coursename;
	string teachername, coursename;
	int coursesnum, coursetimes, k = 0, tid;
	while (fin >> tid) {
	//while (fin >> teacherid) {
		//tid = atoi(teacherid.c_str());
		fin >> teachername >> coursesnum;
		map<Course, int> coursestable;
		vector<Course> courseque = vector<Course> (coursesnum);
		for (int i = 0; i < coursesnum; i++) {
			fin >> coursetimes >> coursename;
			courseque[i] = courses[coursename];
			cout << coursestable.size() << "  " << courseque[i].course_name_ << endl;
			//coursestable[courseque[i]] = coursetimes;
			coursestable.insert(make_pair(courseque[i], coursetimes));
			cout << coursestable.size() << endl;
		}
		//teacherque.push_back(*(new Teacher(groups, teacherid, teachername, courseque ,coursestable)));
		teacherque.push_back(*(new Teacher(groups, teachername, courseque ,coursestable)));
		(teacherque.end() - 1)->teacher_id_ = k++;
	}
	fin.close();
}

void BasicInput() {
	ifstream fin("basic.txt");
	int cnum, cls, notin, temp, stuupper, stulower, dvainpoint;
	//fin >> rooms >> groups >> cnum >> stuupper >> stulower;
	fin >> rooms >> groups >> cnum;
	string cname;
	for(int i = 0; i < cnum; i++){
		//fin >> cname >> cls >> stuupper >> notin;
		fin >> cname >> cls >>  notin >> stuupper >> stulower >> dvainpoint;
		set<int> notinset;
		for (int j = 0; j < notin; j++) {
			fin >> temp;
			notinset.insert(temp);
		}
		couque.push_back(*(new Course(cname, cls, cnum, dvainpoint, stuupper, stulower, notinset)));
		//courses[cname] = *(new Course(cname, cls, cnum));
	}
	//确定了每门课的序号
	sort(couque.begin(), couque.end());
	for (int i = 0; i < couque.size(); i++) {
		couque[i].course_id_ = i;
		courses[couque[i].course_name_] = couque[i];
	}
	fin.close();
}


void TestInput() {
	cout << system("DIR") << endl;;
	ifstream fin("test.txt");
	string temp;
	fin >> temp;
	cout << temp << endl;
	fin.close();
}

void Input() {
	//TestInput();
	BasicInput();
	//StudentsIn();
	YuErGaoInput();
	TeachersIn();
	for (int i = 0; i < stuque.size(); i++)
		sort(stuque[i].courses_.begin(), stuque[i].courses_.end());
	for (int i = 0; i < teacherque.size(); i++) {
		sort(teacherque[i].courses_.begin(), teacherque[i].courses_.end());
	}
	//getchar();
}

void OutPut() {
	ofstream fout("temp.txt");
	for (int i = 0; i < stuque.size(); i++) {
		fout << stuque[i].student_name_ << "\t";
		for (int j = 0; j < stuque[i].courses_.size(); j++)
			fout << stuque[i].courses_[j].course_name_ << "\t";
		fout << endl;
	}
	fout.close();
}

void SetRunTime() {
	GA::rooms_ = rooms;
	GA::groups_ = groups;
	//GA::stu_lower_ = stulower;
	//GA::stu_upper_ = stuupper;
	Schedule::rooms_ = rooms;
	Schedule::groups_ = groups;
	//Schedule::stu_lower_ = stulower;
	//Schedule::stu_upper_ = stuupper;
	//ClassUnit::stu_lower_ = stulower;
	//ClassUnit::stu_upper_ = stuupper;
	//Pattern::stu_lower_ = stulower;
	//Pattern::stu_upper_ = stuupper;
}

void SetRoomsGroups(DButil db) {
	GA::rooms_ = db.rooms;
	GA::groups_ = db.groups;
	Schedule::rooms_ = db.rooms;
	Schedule::groups_ = db.groups;
}

int main(int argc, char* argv[]) {
	
	//testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();
	//srand((unsigned int)time(0));
	//Input();
	//SetRunTime();
	////OutPut();
	////return 0;
	cout << "before ga" << endl;
	DButil db;
	db.StartPk();
	SetRoomsGroups(db);
	//int outtime = 5;
	//GA ga(stuque, teacherque, couque, outtime);
	GA ga(db.stuque, db.teacherque, db.couque, db.outtime);
	cout << "after ga" << endl;
	if (ga.Generate()) {
		//ga.GetResult();//这个是旧版本的算法，已经抛弃不用
		ga.OutPutResult();
		db.result_ = ga.result_;
		db.OutPutResult();
	}
	else cout << "failed gernerate table" << endl;
	//system("PAUSE");
	return 0;
}