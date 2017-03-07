//#include "ga.h"
#include "dbutil.h"

int rooms, groups, stuupper, stulower;
map<string, Course> courses;
vector<Course> couque;
vector<Student> stuque;
vector<Teacher> teacherque;

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
		for (int i = 0; i < 2; i++) {
			auto s = static_cast<Sex>(i);
			c.avg_scores_in_sex_[s] = c.total_scores_in_sex_[s] / c.num_of_stus_in_sex_[s];
		}
		c.tot_avg_points_ = (c.total_scores_in_sex_[male] + c.total_scores_in_sex_[female]) /
			(c.num_of_stus_in_sex_[male] + c.num_of_stus_in_sex_[female]);
	}

	fin.close();
}

void TeachersIn() {
	ifstream fin("teachers.txt");
	string teacherid, teachername, coursename;
	int coursesnum, coursetimes, k = 0, tid;
	while (fin >> teacherid) {
		tid = atoi(teacherid.c_str());
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
		teacherque.push_back(*(new Teacher(groups, teacherid, teachername, courseque ,coursestable)));
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

void Input() {
	BasicInput();
	StudentsIn();
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

int main(int argc, char* argv[]) {
	
	//testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();
	//srand((unsigned int)time(0));
	Input();
	SetRunTime();
	////OutPut();
	////return 0;
	cout << "before ga" << endl;
	GA ga(stuque, teacherque, couque);
	cout << "after ga" << endl;
	if (ga.Generate()) {
		ga.GetResult();
		//ga.OutPutResult();
	}
	//else cout << "failed gernerate table" << endl;
	system("PAUSE");
	return 0;
}