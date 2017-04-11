#include "dbutil.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

string DButil::StartPk(string pktaskid) {
	pktaskid_ = pktaskid;
	statement_ = "";
	GetDBInfo();

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

void DButil::GetDataFromTable(void(DButil::*)()) {
	cout << "�������ݿ⣬��SQL Server  ����PKTaskInfo��Ϣ" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr����ָ��ʵ����ʧ�ܣ�����" << endl;
		return;
	}
	else {
		try {
			//���ط����� ��PKTaskInfo���ݿ� ��¼��Ϊabc, ����Ϊ123
			/*string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
			+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";*/
			string s = SetDBProperties();
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "��¼������ָ��ʵ����ʧ�ܣ�" << endl;
			return;
		}
		try {
			string s = "SELECT * FROM T_PKTask where id=" + pk_task_id_;
			//string s = "SELECT * FROM T_PKTask where id=2";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}
