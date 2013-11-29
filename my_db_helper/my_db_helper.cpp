// my_db_helper.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "my_db_helper.h"
#include "user_tables.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif



// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 db_for_mysql.h
CMySQLDB::CMySQLDB()
{
	CoInitialize(NULL);
	return;
}

CMySQLDB::~CMySQLDB()
{

}


BOOL CMySQLDB::connectToDb(std::string strDsnName,std::string strHostIP,std::string strPortNumber,std::string strUserName,std::string strPassword,std::string strDatabaseName,SQLINTEGER& iSQLState0,std::string& strErrorStr)
{
	mutex.Enter();
 
	this->strHostIP = strHostIP;
	this->strPortNumber = strPortNumber;
	this->strUserName = strUserName;
	this->strPassword = strPassword;
	this->strDatabase = strDatabaseName;
	this->strDsnName  = strDsnName;//


	SQLRETURN sqlRet;
	sqlRet =
		SQLAllocHandle( SQL_HANDLE_ENV,
		SQL_NULL_HANDLE,
		&sql_hEnv );
	sqlRet =
		SQLSetEnvAttr( sql_hEnv,
		SQL_ATTR_ODBC_VERSION,
		(void*) SQL_OV_ODBC3,
		0 );
	sqlRet =
		SQLAllocHandle( SQL_HANDLE_DBC,
		sql_hEnv,
		&sql_hDBC );

	//std::stringstream ss;
	//ss<<"DSN="<<strDsnName<<";UID="<<strUserName<<";PWD="<<strPassWord<<";database="<<strDatabase<<endl;;

	char szConnectString[1024]={0};

	if( strPassword != "" )	
		sprintf(szConnectString,"DSN=%s;UID=%s;PWD=%s;database=%s",strDsnName.c_str(),strUserName.c_str(),strPassword.c_str(),strDatabase.c_str());
	else
		sprintf(szConnectString,"DSN=%s;UID=%s;database=%s",strDsnName.c_str(),strUserName.c_str(),strDatabase.c_str());

	//const char *p = ss.str().c_str();
	SQLPOINTER rgbValue;
	int i = 2;
	rgbValue = &i;

	//SQLSetConnectAttr(sql_hDBC, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(rgbValue), 0);


	SQLCHAR szOutConnectionString[1024]={0};
	SQLSMALLINT    nSize=0; 
	sqlRet = SQLDriverConnect( sql_hDBC,
		0,
		(SQLCHAR*)szConnectString,
		SQL_NTS, 
		szOutConnectionString,
		1024,
		&nSize,
		SQL_DRIVER_NOPROMPT );
	if( SQL_SUCCEEDED( sqlRet ) )
	{

		OutZ("_db_for_sql_:",__FILE__,__LINE__,"DNS 字符串是：[%s]",szConnectString);
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"成功连接数据库");
		return TRUE;
	}
	else 
	{
		//SQLRETURN	hr;
		SQLCHAR theDiagState[1024]={0};
		SQLCHAR  theMessageText[1024]={0};
		SQLINTEGER	theNativeState;

		short       iOutputNo;

		memset(theMessageText,0,1024);
		memset(theDiagState,0,1024);

		SQLGetDiagRec(SQL_HANDLE_DBC ,sql_hDBC,1,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,1024,&iOutputNo);

		if( strcmp((char*)theDiagState,"IM008")==0 )
		{

		}
		else if( strcmp((char*)theDiagState,"HY000") == 0 )
		{

		}

		//SQLGetDiagRec(SQL_HANDLE_DBC ,sql_hDBC,2,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,1024,&iOutputNo);

		OutZ("_db_for_sql_:",__FILE__,__LINE__,"连接数据库失败");
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"DNS 字符串是：[%s]",szConnectString);
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"%s,%s",theMessageText,theDiagState);

		iSQLState0  = theNativeState;
		strErrorStr = (const char*)theMessageText;
		//delete[] theDiagState;
		//delete[] theMessageText;
		strErrorStr += ((const char*)theDiagState);

		if( sql_hDBC !=0 )
		{
			SQLFreeHandle( SQL_HANDLE_DBC, sql_hDBC );
			sql_hDBC = 0;
		}	
		if( sql_hEnv !=0 )
		{		
			SQLFreeHandle( SQL_HANDLE_ENV, sql_hEnv );
			sql_hEnv = 0;	
		}

		mutex.Leave();
		return FALSE;
	}


	OutZ("ok",__FILE__,__LINE__,"ok connect to database \r\n");
	return FALSE;
}

BOOL CMySQLDB::disconnectToDb(std::string strDsnName,std::string strHostIP,std::string strPortNumber,std::string strUserName,std::string strPassword,std::string strDatabase)
{


	this->strHostIP = "";
	this->strPortNumber = "";
	this->strUserName = "";
	this->strPassword = "";
	this->strDatabase = "";
	this->strDsnName = "";


	OutZ("_db_for_sql_:",__FILE__,__LINE__,"<---开始准备断开数据库，等待中。。。");

	if( sql_hDBC!= 0 )
	{
		SQLDisconnect( sql_hDBC ); 
		SQLFreeHandle( SQL_HANDLE_DBC, sql_hDBC );
		sql_hDBC = 0;
	}

	if( sql_hEnv != 0 )
	{
		SQLFreeHandle( SQL_HANDLE_ENV, sql_hEnv );
		sql_hEnv = 0;
	}

	OutZ("_db_for_sql_:",__FILE__,__LINE__,"成功断开数据库");

	//db.prepareODBC("","","","","");

	mutex.Leave();
	OutZ("ok",__FILE__,__LINE__,"ok disconnect to database \r\n");
	return FALSE;
}



BOOL CMySQLDB::StartExecuteSQL(std::string strSQL, std::string& strSQlErrorString2 /*= strErrorSqlDefault*/)
{
	OutZ("_db_for_sql_:",__FILE__,__LINE__,"\r\n------->开始执行SQL语句，等待中。。。\r\n");


	//while (InterlockedExchange (&g_fResourceInUse_ForSQLExecute, TRUE) == TRUE)
	//{
	//	Sleep(1);
	//	Sleep(1000);;
	//}
	//assert(g_fResourceInUse_ForConnection == TRUE);

	SQLRETURN sqlRet =
		SQLAllocHandle( SQL_HANDLE_STMT,
		sql_hDBC,
		&sql_hStmt );

	sqlRet =SQLExecDirect( sql_hStmt,(SQLCHAR*)strSQL.c_str(),SQL_NTS );
	if( SQL_SUCCEEDED( sqlRet))
	{
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"成功执行SQL语句 : [%s]\r\n",strSQL.c_str());
		return TRUE;
	}
	else if( sqlRet == SQL_SUCCESS_WITH_INFO )
	{
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句成功*SQL_SUCCESS_WITH_INFO : [%s]\r\n",strSQL.c_str());
		return TRUE;
	}
	else if ( sqlRet ==SQL_NEED_DATA )
	{
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败*SQL_NEED_DATA : [%s]",strSQL.c_str());
		return FALSE;	
	}
	else if ( sqlRet == SQL_STILL_EXECUTING )
	{

		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败*SQL_STILL_EXECUTING : [%s]",strSQL.c_str());
		return FALSE;
	}
	else if ( sqlRet == SQL_ERROR )
	{

		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败*SQL_ERROR : [%s]",strSQL.c_str());
		SQLCHAR	theDiagState[4096];
		SQLINTEGER	theNativeState;
		SQLCHAR	theMessageText[4096];
		SQLSMALLINT	iOutputNo;
		memset(theMessageText,0,4096);
		memset(theDiagState,0,4096);
		SQLGetDiagRec(SQL_HANDLE_STMT,sql_hStmt,1,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,4096,&iOutputNo);
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"%s,%s",theMessageText,theDiagState);
		//delete[] theDiagState;
		//delete[] theMessageText;

		strSQlErrorString2 =(const char*) theMessageText;

		return FALSE;
	}
	else if ( sqlRet ==  SQL_NO_DATA )
	{
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败*SQL_NO_DATA : [%s]",strSQL.c_str());
		return FALSE;
	}
	else if ( sqlRet ==  SQL_INVALID_HANDLE )
	{
		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败*SQL_INVALID_HANDLE : [%s]",strSQL.c_str());
		return FALSE;
	}
	else 	{

		OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败**未知错误 : [%s]",strSQL.c_str());
		return FALSE;
	}

	OutZ("_db_for_sql_:",__FILE__,__LINE__,"执行SQL语句失败**未知错误 : [%s]",strSQL.c_str());
	return FALSE;
}



BOOL CMySQLDB::EndExecuteSQL()
{

	//assert(g_fResourceInUse_ForConnection == TRUE);

	OutZ("_db_for_sql_:",__FILE__,__LINE__,"\r\n<------结束执行SQL语句，等待中。。。\r\n ");


	if( sql_hStmt != 0 )
	{
		SQLFreeHandle( SQL_HANDLE_STMT, sql_hStmt );
		sql_hStmt = 0;
	}

	OutZ("_db_for_sql_:",__FILE__,__LINE__,"\r\n 成功结束执行SQL语句\r\n ");

	//InterlockedExchange(&g_fResourceInUse_ForSQLExecute, FALSE);
	return TRUE;
}

void CMySQLDB::OutZtoFile(std::string strPrefix,std::string fileName,int iLineNumber,char *szFormat,...)
{

	va_list argptr;
	va_start(argptr, szFormat);

	char      timebuff[80]={0};
	char szMsg[4096]={0};

	std::ofstream ofs;
	ofs.open("applog.txt", std::ios::app | std::ios::out);

	SYSTEMTIME st ={0};
	GetLocalTime(&st);


	sprintf(timebuff,"%04d-%02d-%02d--%02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	sprintf(szMsg,"%s: file: %s  line : %d  time : %s  process id %d  thread: %d \n",strPrefix.c_str(),fileName.c_str(),iLineNumber,timebuff,GetCurrentProcessId(), GetCurrentThreadId());
	OutputDebugString(szMsg);
	ofs << szMsg;


	vsprintf(szMsg,szFormat,argptr);
	OutputDebugString(szMsg);

	ofs << szMsg;

	ofs <<std::endl;

	ofs.close();	

	va_end(argptr);


	return;
}



//#define OUTZ_DBG_OUTPUT

void CMySQLDB::OutS( char *szFormat,...)
{
	va_list argptr;
	va_start(argptr, szFormat);
	char szMsg[4096]={0};

	vsprintf_s(szMsg,szFormat,argptr);
	OutputDebugString(szMsg);

	va_end(argptr);
}

void CMySQLDB::OutZ(std::string strPrefix,std::string fileName,int iLineNumber, char *szFormat,...)
{
#ifdef OUTZ_DBG_OUTPUT

	va_list argptr;
	va_start(argptr, szFormat);
	char szMsg[4096]={0};

	vsprintf_s(szMsg,szFormat,argptr);
	OutputDebugString(szMsg);

	va_end(argptr);

#endif

#ifdef OUTZ_

	va_list argptr;
	va_start(argptr, szFormat);

	char      timebuff[80]={0};
	char szMsg[4096]={0};

	ofstream ofs;
	ofs.open("c:/mylog_scribble.txt", ios::app | ios::out);

	SYSTEMTIME st ={0};
	GetLocalTime(&st);


	sprintf(timebuff,"%04d-%02d-%02d--%02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	sprintf(szMsg,"%s: file: %s  line : %d  time : %s  process id %d  thread: %d \n",strPrefix.c_str(),fileName.c_str(),iLineNumber,timebuff,GetCurrentProcessId(), GetCurrentThreadId());
	OutputDebugString(szMsg);
	ofs << szMsg;


	vsprintf(szMsg,szFormat,argptr);
	OutputDebugString(szMsg);

	ofs << szMsg;

	ofs <<std::endl;

	ofs.close();	

	va_end(argptr);

#endif
}	


//字符串格式化函数
std::string CMySQLDB::Util_format(const char *fmt, ...) 
{ 
	std::string strResult="";
	if (NULL != fmt)
	{
		va_list marker = NULL;            
		va_start(marker, fmt);                            //初始化变量参数 
		size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}            
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
}
//字符串格式化函数
std::wstring CMySQLDB::Util_format(const wchar_t *fmt, ...) 
{ 
	std::wstring strResult=L"";
	if (NULL != fmt)
	{
		va_list marker = NULL;            
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<wchar_t> vBuffer(nLength, L'\0');    //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker); 
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
} 



HANDLE CMySQLDB::Util_startProcess(const char * szCmd)
{
	/*std::string       strFullPath ;
	db.getCurrentPath(strFullPath);
	strFullPath += szCmd;
	*/

	std::string       strCurrentDir;
	db.getCurrentPath(strCurrentDir);
	char szCurrentDir[MAX_PATH] = {0};
	db.getCurrentPath(strCurrentDir);
	for(size_t i=0;i<strCurrentDir.size()-1; i++)
	{
		szCurrentDir[i] = strCurrentDir[i];
	}

	char szCMD0[MAX_PATH] = {0};
	strcpy(szCMD0, szCmd);

	STARTUPINFO         si = {0};
	PROCESS_INFORMATION pi = {0};

	si.cb = sizeof( STARTUPINFO);
	
	

	BOOL b = CreateProcess(NULL, szCMD0, NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,szCurrentDir,&si,&pi); 
	if( !b )
	{
		db.Util_format("无法启动进程%s", szCmd);
	}

	return pi.hProcess;
}

VOID CMySQLDB::Util_stopProcess(const char * szCmd,HANDLE& processHandle)
{
	return;
}
