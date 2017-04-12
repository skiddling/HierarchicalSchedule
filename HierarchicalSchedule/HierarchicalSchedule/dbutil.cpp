#include "dbutil.h" 

//#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") 

string DButil::StartPk(string pktaskid) {
	pktaskid_ = pktaskid;
	statement_ = "";
	GetDBInfo();
	GetDataFromTable(&Get_T_SPKTask, "T_SPKTask");
	GetDataFromTable(&Get_T_SPKClassRoom, "T_SPKClassRoom");
	GetDataFromTable(&Get_T_SPKCourse, "T_SPKCourse");
	GetDataFromTable(&Get_T_SPKStudent, "T_SPKStudent");
	GetDataFromTable(&Get_T_SPKStudentCourse, "T_SPKStudentCourse");
	GetDataFromTable(&Get_T_SPKTeacher, "T_SPKTeacher");
	GetDataFromTable(&Get_T_SPKTeacherCourseConfig, "T_SPKTeacherCourseConfig");
}

void DButil::GetDBInfo() {

	fstream fin("pkjdbc.properties");
	//fin >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	fin >> db_server_ >> db_port_ >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	CutString(db_server_);
	CutString(db_port_);
	CutString(dbname_);
	CutString(dbuser_name_);
	CutString(dbuser_pwd_);
	cout << db_port_ << " " << dbname_ << " " << dbuser_name_ << " " << dbuser_pwd_ << endl;
	fin.close();
}

void DButil::CutString(string & s) {
	s = s.substr(s.find("=") + 1, s.length() - s.find("=") - 1);
}

string DButil::SetDBProperties() {
	string s = "Provider=SQLOLEDB;Server=127.0.0.1," + db_port_ + ";Database=" + dbname_ + ";uid="
		+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
	return s;
}

void DButil::GetDataFromTable(void(DButil::*funcptr)(_RecordsetPtr& m_pRecordset), string tablename) {
	//cout << "连接数据库，从SQL Server  读入PKTaskInfo信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			/*string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
			+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";*/
			string s = SetDBProperties();
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + pktaskid_;
			string s = "SELECT * FROM " + tablename + " where id=" + pktaskid_;
			//string s = "SELECT * FROM T_PKTask where id=2";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			//m_pRecordset->MoveFirst();
			(this->*funcptr)(m_pRecordset);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}

void DButil::Get_T_SPKTask(_RecordsetPtr& m_pRecordset) {
	//m_pRecordset->MoveFirst();
	rooms = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classRoomNum"))->Value);
	groups = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("maxSectionNum"))->Value);
	coursenum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("maxCourseNum"))->Value);
	stunum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("studentNum"))->Value);
	teachernum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("teachersNum"))->Value);
	outtime = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("outTime"))->Value);
}

void DButil::Get_T_SPKCourse(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	string cname;
	int id, cls, stuupper, stulower, stuMinQty, femaleMaxQty, femaleMinQty, maleMaxQty, maleMinQty;
	double classMaxAverage, classMinAverage;
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		cname = static_cast<const char*>(static_cast<_bstr_t>(var));
		cls = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classQty"))->Value);
		stuupper = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("stuQty"))->Value);
		stulower = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("stuMinQty"))->Value);
		classMaxAverage = static_cast<double>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classMaxAverage"))->Value);
		classMinAverage = static_cast<double>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classMinAverage"))->Value);
		femaleMaxQty = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("femaleMaxQty"))->Value);
		femaleMinQty = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("femaleMinQty"))->Value);
		maleMaxQty = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("maleMaxQty"))->Value);
		maleMinQty = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("maleMinQty"))->Value);
		id = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id"))->Value);
		couque.push_back(*(new Course(cname, cls, coursenum, stuupper, stulower, classMaxAverage,
			classMinAverage, femaleMaxQty, femaleMinQty, maleMaxQty, maleMinQty, id)));
		m_pRecordset->MoveNext();
	}
	sort(couque.begin(), couque.end());
	for (auto i = 0; i < couque.size(); i++) {
		couque[i].course_id_ = i;
		courses[couque[i].course_name_] = couque[i];
		coursesid[couque[i].dbid_] = couque[i];
	}
}

void DButil::Get_T_SPKStudent(_RecordsetPtr & m_pRecordset) {
	stuque = vector<Student>(stunum);
	string stuname, stuno, stuGender;
	int stuid;
	_variant_t var;
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("stuName"))->Value;
		stuname = static_cast<const char*>(static_cast<_bstr_t>(var));
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("stuNO"))->Value;
		stuno = static_cast<const char*>(static_cast<_bstr_t>(var));
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("stuGender"))->Value;
		stuGender = static_cast<const char*>(static_cast<_bstr_t>(var));
		stuid = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("studentId"))->Value);
		sturecord[stuid] = stuque.size();
		stuque[sturecord.size()] = *(new Student(stuno, stuname, stuGender, stuid));
		m_pRecordset->MoveNext();
	}
}

void DButil::Get_T_SPKStudentCourse(_RecordsetPtr & m_pRecordset) {
	int spkStudentId, coursedbid;
	double spkCourseGrades;
	while (!m_pRecordset->adoEOF) {
		spkCourseGrades = static_cast<double>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("spkCourseGrades"))->Value);
		coursedbid = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("coursedbid"))->Value);
		spkStudentId = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("spkStudentId"))->Value);
		stuque[sturecord[spkStudentId]].points_[coursesid[coursedbid]] = spkCourseGrades;
		stuque[sturecord[spkStudentId]].courses_.push_back(coursesid[coursedbid]);
		m_pRecordset->MoveNext();
	}
}

void DButil::Get_T_SPKTeacher(_RecordsetPtr & m_pRecordset) {
	int teacherId;
	_variant_t var;
	string name;
	while (!m_pRecordset->adoEOF) {
		teacherId= static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("teacherId"))->Value);
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		name = static_cast<const char*>(static_cast<_bstr_t>(var));
		teacherrecord[teacherId] = teacherque.size();
		teacherque.push_back(*(new Teacher(groups, teacherId, name)));
		(teacherque.end() - 1)->teacher_id_ = teacherque.size() - 1;
		m_pRecordset->MoveNext();
	}
}

void DButil::Get_T_SPKTeacherCourseConfig(_RecordsetPtr & m_pRecordset) {
	int teacherId, clazzNum, spkCourse;
	while (!m_pRecordset->adoEOF) {
		teacherId = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("spkTeacher"))->Value);
		clazzNum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("clazzNum"))->Value);
		spkCourse = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("spkCourse"))->Value);
		teacherque[teacherrecord[teacherId]].courses_.push_back(coursesid[spkCourse]);
		teacherque[teacherrecord[teacherId]].courses_num_[coursesid[spkCourse]] = clazzNum;
		m_pRecordset->MoveNext();
	}
}

void DButil::Get_T_SPKClassRoom(_RecordsetPtr & m_pRecordset) {
	string name;
	_variant_t var;
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		name = static_cast<const char*>(static_cast<_bstr_t>(var));
		roomtable.push_back(name);
		m_pRecordset->MoveNext();
	}
}




