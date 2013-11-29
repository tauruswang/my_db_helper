// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MY_DB_HELPER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MY_DB_HELPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

#ifdef MY_DB_HELPER_EXPORTS
#define MY_DB_HELPER_API __declspec(dllexport)
#else
#define MY_DB_HELPER_API __declspec(dllimport)
#endif


#include "CmdHdr.h"
#include "Optex.h"



// This class is exported from the my_db_helper.dll
//class MY_DB_HELPER_API Cmy_db_helper {
//public:
//	Cmy_db_helper(void);
//	// TODO: add your methods here.
//};
//
//extern MY_DB_HELPER_API int nmy_db_helper;
//
//MY_DB_HELPER_API int fnmy_db_helper(void);
//
//




struct MY_DB_HELPER_API my_field_type
{
	std::string fieldName;
	INT         fieldType;	
	std::string			 vString;
	LONG				 vInteger;
	SQL_TIMESTAMP_STRUCT vDateTime;

	std::vector<byte> vBinaryData;

	BOOL bIsSetValue;
	BOOL isPK;
	BOOL isOrderBy;
	

	my_field_type()
	{
		fieldName = "";
		fieldType = -1;
		vString   = "";
		vInteger  = -1;
		vBinaryData.clear();

		bIsSetValue = FALSE;
		isPK        = FALSE;
		isOrderBy   = FALSE;
		memset(&vDateTime,0,sizeof(SQL_TIMESTAMP_STRUCT));
	}
};




struct MY_DB_HELPER_API my_table_type_base 
{
	//int bFilledData; //= FALSE;
	std::string tableName;
	std::vector<my_field_type> fieldsList;
	int iDefaultOrderFiled;

	virtual bool IsEmpty()
	{
		for(size_t i=0;i< fieldsList.size();i++)
		{
			if( fieldsList[i].bIsSetValue == TRUE )
				return FALSE;
		}
		return TRUE; 
	}

	void featchDataFromCursor(SQLHSTMT sql_hStmt)
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{

			char szName[88]={0};
			strcpy(szName,fieldsList[i].fieldName.c_str());

			switch( fieldsList[i].fieldType )
			{
			case SQL_CHAR:
				{
					SQLINTEGER id1;
					SQLCHAR _strValue[4096]={0};
					SQLGetData(sql_hStmt, i+1, SQL_CHAR, _strValue ,4096, &id1);
					fieldsList[i].vString = (const char *) _strValue;
					fieldsList[i].bIsSetValue = TRUE;
				}
				break;
			case SQL_INTEGER:
				{
					SQLINTEGER id1;
					SQLINTEGER _iIntegerValue=-1;
					SQLGetData(sql_hStmt, i+1, SQL_INTEGER, &_iIntegerValue, 0 , &id1);
					fieldsList[i].vInteger = _iIntegerValue;
					fieldsList[i].bIsSetValue = TRUE;
				}
				break;
			case SQL_TIMESTAMP:
				{
					SQLINTEGER id1;
					SQL_TIMESTAMP_STRUCT _timestamp ={0};
					SQLGetData(sql_hStmt, i+1, SQL_TIMESTAMP, &_timestamp, 255 , &id1);
					memcpy(&fieldsList[i].vDateTime,&_timestamp,sizeof(SQL_TIMESTAMP_STRUCT));
					fieldsList[i].bIsSetValue = TRUE;
				}
			case SQL_BINARY:
				{
					SQLINTEGER idl = 0;
					const size_t byteCount = 4096*1024*50;
					byte *pBinary = new byte[byteCount];

					memset(pBinary,0,byteCount);
					
					SQLGetData(sql_hStmt,i+1,SQL_BINARY,pBinary,byteCount,& idl);

					if( idl != SQL_NO_DATA && idl!= SQL_NULL_DATA )
					{
						for(SQLINTEGER iByteIndex = 0; iByteIndex < idl; iByteIndex++)
						{
							fieldsList[i].vBinaryData.push_back(pBinary[iByteIndex]);
						};
					}
					delete[] pBinary;
				}
				break;
			}

		}
	}

	bool setStrValueByName(std::string fieldName , std::string value)
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{

				fieldsList[i].fieldType = SQL_CHAR; 
				fieldsList[i].vString = value;
				fieldsList[i].bIsSetValue = TRUE;
				return true;

			}

		}

		//std::string str = CMySQLDB::Util_format("没有找到字段 %s !\n",fiedName.c_str());
		MessageBox(NULL,"没有找到字段"," ",MB_OK);
		OutputDebugString(fieldName.c_str());
		//OutputDebugString("\r\n");

		return false;
	}

	bool setIntValueByName(std::string fieldName,LONG iValue)
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{

				fieldsList[i].fieldType = SQL_INTEGER ;
				fieldsList[i].vInteger=iValue;
				fieldsList[i].bIsSetValue = TRUE;
				return true;

			}
		} 
		MessageBox(NULL,"没有找到字段"," ",MB_OK);
		OutputDebugString(fieldName.c_str());
		OutputDebugString("\r\n");
		return false;
	}


	bool setDateTimeValueByName(std::string fieldName,SQL_TIMESTAMP_STRUCT& datetimeStruct)
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{

				fieldsList[i].fieldType = SQL_TIMESTAMP;//)
				fieldsList[i].vDateTime=datetimeStruct;
				fieldsList[i].bIsSetValue = TRUE;
				return true;
			}
		}

		MessageBox(NULL,"没有找到字段"," ",MB_OK);
		OutputDebugString(fieldName.c_str());
		OutputDebugString("\r\n");
		return false;
	}
	
	std::vector<byte>&  getBinaryValueRefByName(std::string fieldName) 
	{
		static std::vector<byte> nullRef;
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{
				return  fieldsList[i].vBinaryData;
			}
		}
		MessageBox(NULL,"没有找到字段"," ",MB_OK);
		OutputDebugString(fieldName.c_str());
		OutputDebugString("\r\n");
		return nullRef;
		
	}

	std::string getStrValueByName(std::string filedname) const
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( filedname == fieldsList[i].fieldName )
			{
				if( fieldsList[i].fieldType == SQL_CHAR )
					return  fieldsList[i].vString;
				else
				{
					OutputDebugString(filedname.c_str());
					OutputDebugString("字段类型不正确！\n");
					return "";
				}
			}

		}
		//my_field_type null_value;
		OutputDebugString(filedname.c_str());
		MessageBox(NULL,"没有找到字段！\n"," ",MB_OK);

		return "";
	}

	LONG getIntValueByName(std::string fieldName) const 
	{
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{
				if( fieldsList[i].fieldType == SQL_INTEGER )
					return fieldsList[i].vInteger;
				else
				{
					OutputDebugString(fieldName.c_str());
					OutputDebugString("字段类型不正确！\n");
					return -1;
				}
			}
		}
		OutputDebugString(fieldName.c_str());
		MessageBox(NULL,"没有找到字段！\n"," ",MB_OK);
		return -1;
	}

	SQL_TIMESTAMP_STRUCT getDateTimeValueByName(std::string fieldName) const
	{
		SQL_TIMESTAMP_STRUCT s = {0};
		for(size_t i=0;i<fieldsList.size();i++)
		{
			if( fieldName == fieldsList[i].fieldName )
			{
				if( fieldsList[i].fieldType == SQL_TIMESTAMP )
					return fieldsList[i].vDateTime;
				else
				{
					OutputDebugString(fieldName.c_str());
					OutputDebugString("字段类型不正确！\n");
					return s;
				}
			}
		}
		OutputDebugString(fieldName.c_str());
		MessageBox(NULL,"没有找到字段！\n"," ",MB_OK);
		return s;
	}




	std::string toString()
	{
		char sz[88888]={0};
		std::string s = sz;

		for(size_t i=0;i< fieldsList.size();i++)
		{
			if( fieldsList[i].fieldType == SQL_CHAR)
			{
				std::string s1 =  getStrValueByName(fieldsList[i].fieldName);
				sprintf(sz," %s=[%s] ",fieldsList[i].fieldName.c_str(), s1.c_str());
			}
			else if( fieldsList[i].fieldType == SQL_INTEGER)
			{
				LONG ii = getIntValueByName(fieldsList[i].fieldName);
				sprintf(sz," %s=[%ld] ",fieldsList[i].fieldName.c_str(), ii);
			}
			else if( fieldsList[i].fieldType == SQL_TIMESTAMP)
			{
				SQL_TIMESTAMP_STRUCT s  = getDateTimeValueByName(fieldsList[i].fieldName);
				sprintf(sz," %s=[%d-%d-%d %d-%d-%d] ",fieldsList[i].fieldName.c_str(), s.year, s.month, s.day, s.hour, s.minute, s.second);
			}
			s += sz;
		}

		s+= "\n";
		return s;
	}

	bool operator==(const my_table_type_base& ref) ;

};




// 此类是从 db_for_mysql.dll 导出的
class MY_DB_HELPER_API CMySQLDB {
public:
	static CMySQLDB& getInstance()
	{
		static CMySQLDB refObj;
		return refObj;
	}
	
	static bool prepareMDBODBC(std::string strFullPathMDBFile)
	{
		char    strAll[4000]={0};

		int  iTempStrlen=strFullPathMDBFile.size();
		if(iTempStrlen==0)
		{
			OutputDebugString("路径不正确sorry ,must input the path");
			return FALSE;
		}

		strcpy(strAll,"DSN=DIVEO_Decoder_Manager");
		strcat(strAll,"?Description=DIVEO_Decoder_Manager");
		strcat(strAll,"?DBQ=");
		strcat(strAll,strFullPathMDBFile.c_str());
		strcat(strAll,"\\");
		strcat(strAll,"decoder_manager.mdb");

		strcat(strAll,"\0");
		int   iTempStrLength=strlen(strAll);
		int   iTemp;
		for(iTemp=0;iTemp<iTempStrLength;iTemp++)
		{

			if(strAll[iTemp]=='?')
			{
				strAll[iTemp]='\0';
			}
		}

		if(SQLConfigDataSource(NULL,ODBC_ADD_DSN, "Microsoft Access Driver (*.mdb)", strAll)==TRUE)
		{
			//AfxMessageBox("odbc create success");

		}
		else
		{
			OutputDebugString("odbc create fail");
		}
		return TRUE;
	}

	static bool deleteMDBODBC(std::string strFullPathMDBFile)
	{
		return TRUE;
	}

	static void removeODBC(const char *szDsnName1,const char *szIpaddress,const char *szName,const char *szDBName,const char* szPassword)
	{
		char sSvr[MAX_PATH] = {0};

		//if( strlen(szPassword) == 0 )
		//	sprintf(sSvr,"DSN=%s;UID=%s;SERVER=%s;DATABASE=%s;charset=GBK;",
		//		szDsnName1, szName, szIpaddress, szDBName);
		//else
		sprintf(sSvr,"DSN=%s;UID=%s;PWD=%s;SERVER=%s;DATABASE=%s;charset=GBK;",
			szDsnName1, szName, szPassword, szIpaddress, szDBName);
		BOOL B = SQLConfigDataSource(NULL,ODBC_REMOVE_SYS_DSN,"MySQL ODBC 5.1 Driver",sSvr);

		if(B ==TRUE)
		{
			//db.OutZ1_SameFile("FeatchLogApp.log",__FILE__,__LINE__,"【info】成功建立ODBC数据源 %s= 【%s】",szName,sSvr);
			char sz[MAX_PATH]={0};
			sprintf(sz,"FeatchLogApp.log",__FILE__,__LINE__,"【info】成功建立ODBC数据源 %s= 【%s】",szName,sSvr);
		}
		else
		{
			char sz[MAX_PATH]={0};
			sprintf(sz,"FeatchLogApp.log",__FILE__,__LINE__,"【error】建立ODBC数据源 %s=  【%s】 失败",szName,sSvr);
			chMB(sz);
		}
	}

	static void prepareODBC(const char *szDsnName1,const char *szIpaddress,const char *szName,const char *szDBName,const char* szPassword)
	{

		char sSvr[MAX_PATH] = {0};

		//if( strlen(szPassword) == 0 )
		//	sprintf(sSvr,"DSN=%s;UID=%s;SERVER=%s;DATABASE=%s;charset=GBK;",
		//		szDsnName1, szName, szIpaddress, szDBName);
		//else
			sprintf(sSvr,"DSN=%s;UID=%s;PWD=%s;SERVER=%s;DATABASE=%s;charset=GBK;",
				szDsnName1, szName, szPassword, szIpaddress, szDBName);
		BOOL B = SQLConfigDataSource(NULL,ODBC_ADD_SYS_DSN,"MySQL ODBC 5.1 Driver",sSvr);

		if(B ==TRUE)
		{
			//db.OutZ1_SameFile("FeatchLogApp.log",__FILE__,__LINE__,"【info】成功建立ODBC数据源 %s= 【%s】",szName,sSvr);
			char sz[MAX_PATH]={0};
			sprintf(sz,"FeatchLogApp.log",__FILE__,__LINE__,"【info】成功建立ODBC数据源 %s= 【%s】",szName,sSvr);
		}
		else
		{
			char sz[MAX_PATH]={0};
			sprintf(sz,"FeatchLogApp.log",__FILE__,__LINE__,"【error】建立ODBC数据源 %s=  【%s】 失败",szName,sSvr);
			chMB(sz);
		}
	}



	static void getCurrentPath(std::string& strCurrentPath)
	{
		TCHAR szPath[MAX_PATH+1]={0};
		GetModuleFileName(NULL,szPath,MAX_PATH);
		(_tcsrchr(szPath,_T('\\')))[1]=0;
		strCurrentPath = szPath;
	}


	static void getMySqlLocation(std::string& strValue)
	{
		static std::string strMySQlLocation =  "";

		if( strMySQlLocation == "")
		{
		//	CRegKey regKey;

			HKEY hkResult;

			BOOL bOk = RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\MySQL AB\\MySQL Server 5.5\\",&hkResult);
			//if( regKey.Open(HKEY_LOCAL_MACHINE,) != ERROR_SUCCESS )
			if ( bOk != ERROR_SUCCESS )
			{
				OutputDebugString("无法打开 注册表 SOFTWARE\\MySQL AB\\MySQL Server 5.5 中的信息");
				return;
			}

			DWORD nChars = MAX_PATH;
			char sz[MAX_PATH]={0};
			nChars = 128;

			DWORD Type;

		//	regKey.QueryStringValue("Location",sz,&nChars);
			LONG rRetVal = RegQueryValueEx(hkResult,"Location",NULL,&Type,(LPBYTE)sz,&nChars);

			//OutputDebugString(sz);

			//regKey.Close();
			RegCloseKey(hkResult);

			strMySQlLocation = sz;
		}
		strValue = strMySQlLocation;
		return;
	}


	std::string strUserName;
	std::string strPassword;
	std::string strHostIP;
	std::string strPortNumber;
	std::string strDatabase;
	
	std::string strDsnName;


	SQLHDBC sql_hEnv;
	SQLHDBC sql_hDBC;
	//volatile LONG g_fResourceInUse_ForSQLExecute;
	SQLHSTMT sql_hStmt; // = 0;


	inline static void OutZ(std::string strPrefix,std::string fileName,int iLineNumber, char *szFormat,...);
	static void OutZtoFile(std::string gstrPrefix,std::string fileName,int iLineNumber,char *szFormat,...);

	static void OutS( char *szFormat,...);


	BOOL    connectToDb(std::string strDsnName,std::string strHostIP,std::string strPortNumber,std::string strUserName,std::string strPassword,std::string strDatabaseName,SQLINTEGER& iSQLState0,std::string& strErrorStr);
	BOOL disconnectToDb(std::string strDsnName,std::string strHostIP,std::string strPortNumber,std::string strUserName,std::string strPassword,std::string strDatabaseName);

	template<typename TypeOfARecord>
	BOOL my_mdb_get_count_table(std::string strFullPathOFMDBFFile, std::string& strSQLError,size_t& iCount)
	{
		_ConnectionPtr m_pConnect;
		_RecordsetPtr m_pRecordset;

		TypeOfARecord aRecordType;

	/*	std::string strOut = aRecBeingDelete.toString();
		OutputDebugString(strOut.c_str());
		OutputDebugString("\n");*/
		try
		{	
			m_pConnect.CreateInstance(__uuidof(Connection));

			std::string connStr;
			connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			connStr += strFullPathOFMDBFFile;
			//connStr += "\"";

			if(!m_pConnect->Open(connStr.c_str(),"","",adModeUnknown))
			{
				m_pRecordset.CreateInstance(__uuidof(Recordset));

		
				char szQuerySQL[512]={0};
				std::string strTableName = aRecordType.tableName.c_str();
		
				sprintf(szQuerySQL,
						"select count(*)  as all_row_counts from %s", 
						strTableName.c_str()
						);

				
				m_pRecordset->Open(szQuerySQL,m_pConnect.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText );

				//OutputDebugString(szMsgDbg);
				m_pRecordset->MoveFirst();
				
				while(!m_pRecordset->adoEOF)
				{
					const size_t iSizeFieldCount = aRecordType.fieldsList.size();
					//if( iSizeFieldCount == 1 )
					//{

						_variant_t TheValue;


						TheValue = m_pRecordset->GetCollect("all_row_counts");
						if( TheValue.vt != VT_NULL )
						{

							std::string szValueOfInteger = (LPCSTR)_bstr_t(TheValue);
							iCount = _ttoi(szValueOfInteger.c_str());
							//newRecValue.fieldsList[iIndexOfField].bIsSetValue = TRUE;
						}
					//}
						m_pRecordset->MoveNext();
				}

				
				m_pConnect->Close();
			}

			return TRUE;

		}
		catch(_com_error& e)
		{
			OutputDebugString(e.ErrorMessage());
			OutputDebugString(e.Description());
			OutputDebugString(e.Source());
	//		//			cout << cerr.Error () << "\n";
	//		//			> > > cout << cerr.ErrorMessage () << "\n";
	//		//>			 > > cout << cerr.Description () << "\n";
	//		//> > > cout << cerr.Source () << "\n";

			return FALSE;
		}
	}

	template<typename TypeOfARecord>
	BOOL my_mdb_delete_table_all(
		std::string strFullPathOFMDBFFile, std::string& strSQLError)
	{
		_ConnectionPtr m_pConnect;
		_RecordsetPtr m_pRecordset;

		TypeOfARecord aRecordType;

		/*std::string strOut = aRecBeingDelete.toString();
		OutputDebugString(strOut.c_str());
		OutputDebugString("\n");*/
		try
		{	
			m_pConnect.CreateInstance(__uuidof(Connection));

			//if(!m_pConnect->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=decoder_manager.mdb","","",adModeUnknown))
			//adOpenDynamic,adLockOptimistic,adCmdText

			std::string connStr;
			connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			connStr += strFullPathOFMDBFFile;
			//connStr += "\"";

			if(!m_pConnect->Open(connStr.c_str(),"","",adModeUnknown))
			{
				m_pRecordset.CreateInstance(__uuidof(Recordset));

				char szQuerySQL[512]={0};
			
				sprintf(szQuerySQL,
						"delete from %s ", 
						aRecordType.tableName.c_str()
						);

				m_pRecordset->Open(szQuerySQL,m_pConnect.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText );

			}
			
			m_pConnect->Close();
			return TRUE;

		}
		catch(_com_error& e)
		{
			OutputDebugString(e.ErrorMessage());
			OutputDebugString(e.Description());
			OutputDebugString(e.Source());
			//			cout << cerr.Error () << "\n";
			//			> > > cout << cerr.ErrorMessage () << "\n";
			//>			 > > cout << cerr.Description () << "\n";
			//> > > cout << cerr.Source () << "\n";

			return FALSE;
		}

	}
	

	template<typename TypeOfARecord>
	BOOL my_mdb_delete_table(TypeOfARecord& aRecBeingDelete, 
		std::string strFullPathOFMDBFFile, std::string& strSQLError)
	{
		_ConnectionPtr m_pConnect;
		_RecordsetPtr m_pRecordset;

		//TypeOfARecord aRecordType;

		std::string strOut = aRecBeingDelete.toString();
		OutputDebugString(strOut.c_str());
		OutputDebugString("\n");
		try
		{	
			m_pConnect.CreateInstance(__uuidof(Connection));

			//if(!m_pConnect->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=decoder_manager.mdb","","",adModeUnknown))
			//adOpenDynamic,adLockOptimistic,adCmdText

			std::string connStr;
			connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			connStr += strFullPathOFMDBFFile;
			//connStr += "\"";
 
			if(!m_pConnect->Open(connStr.c_str(),"","",adModeUnknown))
			{
				m_pRecordset.CreateInstance(__uuidof(Recordset));
				
				char szQuerySQL[512]={0};
				std::string strTableName = aRecBeingDelete.tableName.c_str();
				if ( strTableName == "GroupEncoder" ) 
				{
					std::string strRecId     = aRecBeingDelete.getStrValueByName("encoder_id");

					sprintf(szQuerySQL,
					"delete from %s where encoder_id=\"%s\"", 
					strTableName.c_str(),
					strRecId.c_str()
					);
				}
				else
				{
					std::string strRecId     = aRecBeingDelete.getStrValueByName("decode_id");

					sprintf(szQuerySQL,
					"delete from %s where decode_id=\"%s\"", 
					strTableName.c_str(),
					strRecId.c_str()
					);
				
				}
				m_pRecordset->Open(szQuerySQL,m_pConnect.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText );
				
			}

			m_pConnect->Close();
			return TRUE;

		}
		catch(_com_error& e)
		{
			OutputDebugString(e.ErrorMessage());
			OutputDebugString(e.Description());
			OutputDebugString(e.Source());
//			cout << cerr.Error () << "\n";
//			> > > cout << cerr.ErrorMessage () << "\n";
//>			 > > cout << cerr.Description () << "\n";
//> > > cout << cerr.Source () << "\n";

			return FALSE;
		}
	}

	template<typename TypeOfARecord>
	BOOL my_mdb_insert_table(TypeOfARecord& newRecordToInsert, std::string strFullPathOfMDBFile, std::string& strSQLError)
	{

		_ConnectionPtr m_pConnect;
		_RecordsetPtr m_pRecordset;

		TypeOfARecord aRecordType;
		
		try
		{	
			m_pConnect.CreateInstance(__uuidof(Connection));

			//if(!m_pConnect->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=decoder_manager.mdb","","",adModeUnknown))
			//adOpenDynamic,adLockOptimistic,adCmdText

			std::string connStr;
			connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			connStr += strFullPathOfMDBFile;
			//connStr += "\"";
 
			if(!m_pConnect->Open(connStr.c_str(),"","",adModeUnknown))
			{
				m_pRecordset.CreateInstance(__uuidof(Recordset));

				
				
				char szSQL[4096]={0};
				sprintf(szSQL,"INSERT INTO %s(",newRecordToInsert.tableName.c_str());
				for(size_t i=0;i< newRecordToInsert.fieldsList.size();i++)
				{
					if( i!=0 )
						strcat(szSQL,", ");
					strcat(szSQL, newRecordToInsert.fieldsList[i].fieldName.c_str());
				}
				strcat(szSQL,") values( ");

				for(size_t i=0;i< newRecordToInsert.fieldsList.size();i++)
				{


					char szV[999]={0};
					if( newRecordToInsert.fieldsList[i].fieldType == SQL_CHAR )
					{
						sprintf(szV,"'%s'", newRecordToInsert.fieldsList[i].vString.c_str() );
					}
					else if( newRecordToInsert.fieldsList[i].fieldType == SQL_INTEGER )
					{
						sprintf(szV,"%d", newRecordToInsert.fieldsList[i].vInteger );
					}
					else if( newRecordToInsert.fieldsList[i].fieldType == SQL_TIMESTAMP )
					{
						sprintf(szV,"'%04d/%02d/%02d--%02d:%02d:%02d'", newRecordToInsert.fieldsList[i].vDateTime.year, 
							newRecordToInsert.fieldsList[i].vDateTime.month, 
							newRecordToInsert.fieldsList[i].vDateTime.day, 
							newRecordToInsert.fieldsList[i].vDateTime.hour, 
							newRecordToInsert.fieldsList[i].vDateTime.minute, 
							newRecordToInsert.fieldsList[i].vDateTime.second);
					}

					if( i!= 0 )
						strcat(szSQL," , ");
					strcat(szSQL, szV);
				}
				strcat(szSQL," ); ");
				std::string strSQL = szSQL;
				
				OutputDebugString("\n----------------------\n");
				OutputDebugString(strSQL.c_str());
				OutputDebugString("\n----------------------\n");

				m_pRecordset->Open(strSQL.c_str() ,m_pConnect.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText );

			}

			m_pConnect->Close();
			return TRUE;
		}
		catch(_com_error& e)
		{
			OutputDebugString(e.ErrorMessage());
			OutputDebugString(e.Description());
			OutputDebugString(e.Source());
//			cout << cerr.Error () << "\n";
//			> > > cout << cerr.ErrorMessage () << "\n";
//>			 > > cout << cerr.Description () << "\n";
//> > > cout << cerr.Source () << "\n";

			return FALSE;
		}
		//return FALSE;
	}

	template<typename TypeOfARecord>
	BOOL my_mdb_read_table(std::vector<TypeOfARecord>& allRecordToSelect, 
		std::string strFullPathOFMDBFFile, std::string& strSQLError)
	{	
		_ConnectionPtr m_pConnect;
		_RecordsetPtr m_pRecordset;

		TypeOfARecord aRecordType;
		
		try
		{	
			m_pConnect.CreateInstance(__uuidof(Connection));

			//if(!m_pConnect->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=decoder_manager.mdb","","",adModeUnknown))
			//adOpenDynamic,adLockOptimistic,adCmdText

			std::string connStr;
			connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			connStr += strFullPathOFMDBFFile;
			//connStr += "\"";

			OutputDebugString(connStr.c_str());
			OutputDebugString("\r\n");
 
			if(!m_pConnect->Open(connStr.c_str(),"","",adModeUnknown))
			{
				m_pRecordset.CreateInstance(__uuidof(Recordset));


				char szQuerySQL[512]={0};
				
				if( aRecordType.tableName == "decode_info" )
					sprintf(szQuerySQL,"select * from %s order by decode_num", aRecordType.tableName.c_str());
				else
					sprintf(szQuerySQL,"select * from %s", aRecordType.tableName.c_str());

				OutputDebugString(szQuerySQL);
				OutputDebugString("\r\n");


				m_pRecordset->Open(szQuerySQL,m_pConnect.GetInterfacePtr(),adOpenDynamic,adLockBatchOptimistic,adCmdText );
				LONG iCount1 = m_pRecordset->RecordCount;
				//BOOL bEOF = m_pRecordset->adoEOF;
				//BOOL bBOF = m_pRecordset->BOF;

				char szMsgDbg[1024]={0};

				if( m_pRecordset->adoEOF == -1 && m_pRecordset->BOF == -1  && iCount1 == -1 )
				{
					//m_pConnect->Release();
					sprintf(szMsgDbg,"%s没有发现MDB文件记录\r\n",strFullPathOFMDBFFile.c_str());
					m_pConnect->Close();

					return TRUE;
				}

				//m_pRecordset->Move(0,0);
			
				m_pRecordset->MoveFirst();

				size_t iCount = 0;
				while(!m_pRecordset->adoEOF)
				{
					const size_t iSizeFieldCount = aRecordType.fieldsList.size();
					TypeOfARecord newRecValue;

					for(size_t iIndexOfField=0;  iIndexOfField < iSizeFieldCount ;iIndexOfField++)
					{

						char szName[88]={0};
						strcpy(szName,aRecordType.fieldsList[iIndexOfField].fieldName.c_str());
						

						_variant_t TheValue;
						switch( aRecordType.fieldsList[iIndexOfField].fieldType )
						{
						case SQL_CHAR:
							{
								TheValue = m_pRecordset->GetCollect(szName);
								if( TheValue.vt != VT_NULL )
								{

									std::string szValueOfString = (LPCSTR)_bstr_t(TheValue);
									newRecValue.fieldsList[iIndexOfField].vString =  szValueOfString;
									newRecValue.fieldsList[iIndexOfField].bIsSetValue = TRUE;
								}
								else
								{
				
								}
							}
							break;
						case SQL_INTEGER:
							{
								TheValue = m_pRecordset->GetCollect(szName);
								if( TheValue.vt != VT_NULL )
								{

									std::string szValueOfInteger = (LPCSTR)_bstr_t(TheValue);
									newRecValue.fieldsList[iIndexOfField].vInteger = _ttoi(szValueOfInteger.c_str());
									newRecValue.fieldsList[iIndexOfField].bIsSetValue = TRUE;
								}
								else
								{

								}
							}
						}
					}

					allRecordToSelect.push_back(newRecValue);
					m_pRecordset->MoveNext();
					iCount++;
				}

				sprintf(szMsgDbg,"发现MDB文件记录 %d\r\n",iCount);
				m_pConnect->Close();


				return TRUE;
			}
			else
			{
				//m_pConnect->Release();
				return FALSE;
			}

		}
		catch(_com_error& e)
		{
			OutputDebugString(e.ErrorMessage());
			OutputDebugString(e.Description());
			OutputDebugString(e.Source());

			CString strMsg;
			strMsg.Format(_T("错误描述：%s\n错误消息%s"), 
				(LPCTSTR)e.Description(),
				(LPCTSTR)e.ErrorMessage());

			
			AfxMessageBox(strMsg);
//			cout << cerr.Error () << "\n";
//			> > > cout << cerr.ErrorMessage () << "\n";
//>			 > > cout << cerr.Description () << "\n";
//> > > cout << cerr.Source () << "\n";
		}
		
		return FALSE;
	}


	template<typename TypeOfARecord>
	BOOL my_tmp_read_table(std::vector<TypeOfARecord>& allRecordToSelect,std::string& strSQLError)
	{
		TypeOfARecord aRecordType;

		BOOL b= FALSE;
	
		char strSQL[4096]={0};

		if( aRecordType.tableName[aRecordType.tableName.length()-1]== ')' &&  
			aRecordType.tableName[aRecordType.tableName.length()-2]== '('
			)
		{
			sprintf(strSQL,"select %s", aRecordType.tableName.c_str());
		}
		else if( aRecordType.tableName == "SHOW DATABASES;")
		{
			sprintf(strSQL,"%s",aRecordType.tableName.c_str());
		}
		else
		{
			sprintf(strSQL,"select * from %s ", aRecordType.tableName.c_str());
					
		}


		b=StartExecuteSQL(strSQL,strSQLError);
		if( b == FALSE )
		{
			EndExecuteSQL();
			//	DisconnectToDB();
			return b;
		}

		b = my_tmp_FeatchAllQueryResultByTemplate<TypeOfARecord>(allRecordToSelect);

		EndExecuteSQL();

		//DisconnectToDB();

		return b;
	}
	
	template<typename TypeOfARecord>
	BOOL my_tmp_read_table_by_index(std::vector<TypeOfARecord>& recordToSelect,int iStart,int iCount,std::string& strSQLError,std::string strStartDayTime="",std::string strEndDayTime="",std::string strDeviceId = "")
	{
		BOOL b = FALSE;
		char strSQL[4096]={0};

		TypeOfARecord aRecordType;
		std::string strOrderByFiled = aRecordType.fieldsList[aRecordType.iDefaultOrderFiled].fieldName;
		if( strStartDayTime!= "" && strEndDayTime !="" && strDeviceId =="" )
		{
	
			sprintf(strSQL,"select * from %s where UNIX_TIMESTAMP(log_time) >= UNIX_TIMESTAMP('%s') AND UNIX_TIMESTAMP(log_time) <= UNIX_TIMESTAMP('%s') ORDER BY %s DESC LIMIT %d,%d ", 
				aRecordType.tableName.c_str(),
				strStartDayTime.c_str(), strEndDayTime.c_str(),
				strOrderByFiled.c_str(), iStart, iCount
				);

		}
		else if (strStartDayTime!= "" && strEndDayTime !="" && strDeviceId != "") 
		{
			sprintf(strSQL,"select * from %s where UNIX_TIMESTAMP(log_time) >= UNIX_TIMESTAMP('%s') AND UNIX_TIMESTAMP(log_time) <= UNIX_TIMESTAMP('%s') AND device_id='%s' ORDER BY %s DESC LIMIT %d,%d ", 
				aRecordType.tableName.c_str(),
				strStartDayTime.c_str(), strEndDayTime.c_str(),
				strDeviceId.c_str(),
				strOrderByFiled.c_str(), iStart, iCount
				);
		}
		else
		{
			sprintf(strSQL,"select * from %s ORDER BY %s DESC LIMIT %d,%d", aRecordType.tableName.c_str(),
																		 strOrderByFiled.c_str(), iStart, iCount,
																		 strStartDayTime.c_str(), strEndDayTime.c_str()
			);
		}


		b = StartExecuteSQL(strSQL,strSQLError);
		if( b == FALSE )
		{
			EndExecuteSQL();
			return b;
		}
		
		my_tmp_FeatchAllQueryResultByTemplate(recordToSelect);
		//my_tmp_FeatchAllQueryResultByTemplateToMap(recordToSelect,iStart);
		//b = my_tmp_FeatchAllQueryResultByTemplateToMap<TypeOfARecord>(recordToSelect,iStart);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             )
		return  b;
	}
	

	template<typename TypeOfARecord>
	BOOL my_tmp_get_row_count(size_t& rowsCount,std::string strStartDayTime="",std::string strEndDayTime="",std::string strDeviceID ="")
	{
		TypeOfARecord aRecordType;
		BOOL b = FALSE;
		//BOOL b = ConnectToDB(strLocalDB_DSN,strLocalDB_UserName,strLocalDB_Password,strLocalDB_Database);
		/*
		connectToDb()
		if( b == FALSE )
		{
			DisconnectToDB();
			return b;
		}*/
		std::string strErrorString;

		char strSQL[4096]={0};

		if( strStartDayTime!= "" && strEndDayTime !="" && strDeviceID =="" )
		{

			sprintf(strSQL,"select COUNT(*) from %s  where log_time > '%s' AND log_time < '%s' ", aRecordType.tableName.c_str(),
					strStartDayTime.c_str(), strEndDayTime.c_str()
				);

		}
		else if(  strStartDayTime!= "" && strEndDayTime !="" && strDeviceID !="" )
		{
			sprintf(strSQL,"select COUNT(*) from %s  where log_time > '%s' AND log_time < '%s' AND device_id='%s' ", aRecordType.tableName.c_str(),
				strStartDayTime.c_str(), strEndDayTime.c_str(),
				strDeviceID.c_str()
				);

		}
		else 
		{

			sprintf(strSQL,"select COUNT(*) from %s", aRecordType.tableName.c_str());

		}
		b= StartExecuteSQL(strSQL,strErrorString);
		if( b == FALSE )
		{
			EndExecuteSQL();
			//DisconnectToDB();
			return b;
		}
		//assert(g_fResourceInUse_ForConnection == TRUE);
		SQLRETURN rc = SQLFetch(sql_hStmt);//移动光标
		if (rc == SQL_ERROR|| rc == SQL_SUCCESS_WITH_INFO ) {

			SQLCHAR	theDiagState[1024]={0};
			SQLINTEGER	theNativeState;
			SQLCHAR	theMessageText[1024]={0};
			SQLSMALLINT	iOutputNo;

			memset(theMessageText,0,255);
			memset(theDiagState,0,50);
			SQLGetDiagRec(SQL_HANDLE_STMT,sql_hStmt,1,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,1024,&iOutputNo);
			OutZ("_db_for_sql_:",__FILE__,__LINE__,"FetchAllQueryResult失败 : [%s]",(LPCSTR)theMessageText);

		/*	delete[] theDiagState;
			delete[] theMessageText;*/

			b= FALSE;

		}
		SQLINTEGER id1;
		SQLUINTEGER iFinnalData=-1;
		SQLGetData(sql_hStmt, 1, SQL_INTEGER, &iFinnalData ,0, &id1);

		rowsCount = iFinnalData;
		EndExecuteSQL();
		//DisconnectToDB();
		return b;

	}
	template<typename TypeOfARecord>
	BOOL my_tmp_delete_all_rec(std::string& strErrorString)
	{

		TypeOfARecord dummyRec;

		char szSQL[4096]={0};

		sprintf(szSQL,"truncate table %s",dummyRec.tableName.c_str());
		std::string strSQL = szSQL;
		BOOL b = StartExecuteSQL(strSQL,strErrorString);
		if( b == FALSE )
		{
			EndExecuteSQL();

			return b;
		}
		EndExecuteSQL();
		return TRUE;
	}

	template<typename TypeOfARecord>
	BOOL my_tmp_insert(TypeOfARecord& newRecordToInsert,std::string& strErrorString)
	{
		
		char szSQL[4096]={0};

		sprintf(szSQL,"INSERT INTO %s(",newRecordToInsert.tableName.c_str());
		for(size_t i=0;i< newRecordToInsert.fieldsList.size();i++)
		{
			if( i!=0 )
				strcat(szSQL,", ");
			strcat(szSQL, newRecordToInsert.fieldsList[i].fieldName.c_str());
		}
		strcat(szSQL,") values( ");

		for(size_t i=0;i< newRecordToInsert.fieldsList.size();i++)
		{


			char szV[999]={0};
			if( newRecordToInsert.fieldsList[i].fieldType == SQL_CHAR )
			{
				sprintf(szV,"'%s'", newRecordToInsert.fieldsList[i].vString.c_str() );
			}
			else if( newRecordToInsert.fieldsList[i].fieldType == SQL_INTEGER )
			{
				sprintf(szV,"%d", newRecordToInsert.fieldsList[i].vInteger );
			}
			else if( newRecordToInsert.fieldsList[i].fieldType == SQL_TIMESTAMP )
			{
				sprintf(szV,"'%04d/%02d/%02d--%02d:%02d:%02d'", newRecordToInsert.fieldsList[i].vDateTime.year, 
					newRecordToInsert.fieldsList[i].vDateTime.month, 
					newRecordToInsert.fieldsList[i].vDateTime.day, 
					newRecordToInsert.fieldsList[i].vDateTime.hour, 
					newRecordToInsert.fieldsList[i].vDateTime.minute, 
					newRecordToInsert.fieldsList[i].vDateTime.second);
			}

			if( i!= 0 )
				strcat(szSQL," , ");
			strcat(szSQL, szV);
		}
		strcat(szSQL," ); ");

		std::string strSQL = szSQL;
		BOOL b = StartExecuteSQL(strSQL,strErrorString);
		if( b == FALSE )
		{
			EndExecuteSQL();
			
			return b;
		}
		EndExecuteSQL();
	
		return TRUE;
	}
	
	template<typename TypeOfARecord>
	BOOL my_tmp_delete(TypeOfARecord& recCondistion,std::string& strErrorString)
	{
		std::string strSQL;
		char sz[9999]={0};
		sprintf(sz,"DELETE  FROM %s ",recCondistion.tableName.c_str());

		strSQL += sz;

		BOOL bSetStart = FALSE;
		
		for(size_t j=0;j<recCondistion.fieldsList.size();j++)
		{
			if( recCondistion.fieldsList[j].isPK == TRUE )
			{
				if( recCondistion.fieldsList[j].bIsSetValue )
				{
					if(  bSetStart== FALSE )
					{
						sprintf(sz," WHERE  %s=",recCondistion.fieldsList[j].fieldName.c_str());
						strSQL += sz;
						bSetStart= TRUE;
					}
					else 
					{
						sprintf(sz," and %s=",recCondistion.fieldsList[j].fieldName.c_str());
						strSQL += sz;
					}

					if( recCondistion.fieldsList[j].fieldType == SQL_CHAR )
					{
						sprintf(sz,"'%s'",recCondistion.fieldsList[j].vString.c_str());		
						strSQL += sz;
					}
					else if ( recCondistion.fieldsList[j].fieldType == SQL_INTEGER )
					{
						sprintf(sz,"%d",recCondistion.fieldsList[j].vInteger);		
						strSQL += sz;
					}
					else if ( recCondistion.fieldsList[j].fieldType == SQL_TIMESTAMP )
					{
						sprintf(sz,"'%04d/%02d/%02d--%02d:%02d:%02d'", 
							recCondistion.fieldsList[j].vDateTime.year, 
							recCondistion.fieldsList[j].vDateTime.month, 
							recCondistion.fieldsList[j].vDateTime.day, 
							recCondistion.fieldsList[j].vDateTime.hour, 
							recCondistion.fieldsList[j].vDateTime.minute, 
							recCondistion.fieldsList[j].vDateTime.second);
					}
				}
				

			}
		}
		//OutputDebugString(strSQL.c_str());

		std::string strSQL1 = strSQL;
		BOOL b = StartExecuteSQL(strSQL1,strErrorString);
		if( b == FALSE )
		{
			EndExecuteSQL();

			return b;
		}
		EndExecuteSQL();

		return TRUE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	template<typename TypeOfARecord>
	BOOL testReadBinaryData(std::vector<TypeOfARecord>& allRecordToSelect,std::string& strSQLError)
	{
		BOOL b = FALSE;
		char strSQL[4096]={0};

		strcpy(strSQL,"select * from config_group;");

		std::string strSQL1 = strSQL;
		BOOL b = StartExecuteSQL(strSQL1,strErrorString);
		if( b == FALSE )
		{
			EndExecuteSQL();
			return b;
		}
		EndExecuteSQL();
		b = my_tmp_FeatchAllQueryResultByTemplate<TypeOfARecord>(allRecordToSelect);
		return b;
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// statuc help file

	static std::wstring CMySQLDB::Util_format(const wchar_t *fmt, ...); 
	static	std::string CMySQLDB::Util_format(const char *fmt, ...);

	HANDLE Util_startProcess(const char * szCmd);
	VOID   Util_stopProcess(const char * szCmd,HANDLE& processHandle);


	static std::vector<std::string> Util_split0(std::string str,std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str+=pattern;//扩展字符串以方便操作
		size_t size=str.size();

		for(int i=0; i<size; i++)
		{
			pos=str.find(pattern,i);
			if(pos<size)
			{
				std::string s=str.substr(i,pos-i);
				result.push_back(s);
				i=pos+pattern.size()-1;
			}
		}
		return result;
	}


	inline std::string CTimeToStdString(SYSTEMTIME& st)
	{
		std::string strTime;
		strTime = CMySQLDB::Util_format("%02d-%02d-%02d",st.wHour, st.wMinute, st.wSecond);
		return strTime;
	}
	
	inline SYSTEMTIME StdStringToCTime(std::string strTime)
	{
		char szH[3]={0};
		char szM[3]={0};
		char szS[3]={0};

		szH[0] = strTime[0];
		szH[1] = strTime[1];

		szM[0] = strTime[3];
		szM[1] = strTime[4];

		szS[0] = strTime[6];
		szS[1] = strTime[7];


		int h = atoi(szH);
		int m = atoi(szM);
		int s = atoi(szS);

		SYSTEMTIME ST;
		ST.wYear   = 1990;
		ST.wMonth  = 1;
		ST.wDay    = 1;
		ST.wHour   = h;
		ST.wMinute = m;
		ST.wSecond = s;
		
		return ST;
	}


		static const char* CMySQLDB::newGUID()  
		{  
			static char buf[64] = {0};  
			GUID guid;  
			if (S_OK == ::CoCreateGuid(&guid))  
			{  
				_snprintf(buf, sizeof(buf)  
					, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"  
					, guid.Data1  
					, guid.Data2  
					, guid.Data3  
					, guid.Data4[0], guid.Data4[1]  
				, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]  
				, guid.Data4[6], guid.Data4[7]  
				);  
			}  
			return (const char*)buf;  
		}  


		const char* CMySQLDB::getString()
		{
			return 0;
		}

protected:


	BOOL CMySQLDB::StartExecuteSQL(std::string strSQL, std::string& strSQlErrorString2 /*= strErrorSqlDefault*/);
	BOOL CMySQLDB::EndExecuteSQL();



	template <typename T>
	BOOL my_tmp_FeatchAllQueryResultByTemplate(std::vector<T>& t)
	{
		int i=0;
		BOOL bOk = TRUE;
		OutZ("_db_for_sql_:",__FILE__,__LINE__," 开始获取光标   ");
		//assert(g_fResourceInUse_ForConnection == TRUE);

		while (TRUE) 
		{//循环得到所有行

			SQLRETURN rc = SQLFetch(sql_hStmt);//移动光标

			if (rc == SQL_ERROR|| rc == SQL_SUCCESS_WITH_INFO ) {

				SQLCHAR*	theDiagState = new SQLCHAR[1024];
				SQLINTEGER	theNativeState;
				SQLCHAR*	theMessageText  = new SQLCHAR[1024];
				SQLSMALLINT	iOutputNo;

				memset(theMessageText,0,255);
				memset(theDiagState,0,50);
				SQLGetDiagRec(SQL_HANDLE_STMT,sql_hStmt,1,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,1024,&iOutputNo);
				OutZ("_db_for_sql_:",__FILE__,__LINE__,"FetchAllQueryResult失败 : [%s]",(LPCSTR)theMessageText);

				delete[] theDiagState;
				delete[] theMessageText;

				bOk= FALSE;

			}


			if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){

				T aType;
				aType.featchDataFromCursor(sql_hStmt);
				t.push_back(aType);
				i++;
			}
			else {
				break;
				bOk= FALSE;
			}
		}

		if( bOk)
			OutZ("_db_for_sql_:",__FILE__,__LINE__," 结束获取光标 成功 ");
		else
			OutZ("_db_for_sql_:",__FILE__,__LINE__," 结束获取光标 失败 ");
		return TRUE;
	}


	
	template <typename T>
	BOOL my_tmp_FeatchAllQueryResultByTemplateToMap(std::map<INT,T>& t,INT iStartFrom)
	{
		int i=0;
		BOOL bOk = TRUE;
		OutZ("_db_for_sql_:",__FILE__,__LINE__," 开始获取光标   ");
	//	assert(g_fResourceInUse_ForConnection == TRUE);

		while (TRUE) 
		{//循环得到所有行

			SQLRETURN rc = SQLFetch(sql_hStmt);//移动光标

			if (rc == SQL_ERROR|| rc == SQL_SUCCESS_WITH_INFO ) {

				SQLCHAR	theDiagState [1024]={0};
				SQLINTEGER	theNativeState;
				SQLCHAR	theMessageText[1024]={0};
				SQLSMALLINT	iOutputNo;

				memset(theMessageText,0,255);
				memset(theDiagState,0,50);
				SQLGetDiagRec(SQL_HANDLE_STMT,sql_hStmt,1,(SQLCHAR*)theDiagState,&theNativeState,(SQLCHAR*)theMessageText,1024,&iOutputNo);
				OutZ("_db_for_sql_:",__FILE__,__LINE__,"FetchAllQueryResult失败 : [%s]",(LPCSTR)theMessageText);

				/*delete[] theDiagState;
				delete[] theMessageText;*/

				bOk= FALSE;

			}


			if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO){

				T aType;
				aType.featchDataFromCursor(sql_hStmt);
				t[iStartFrom+i]=aType;
				i++;
			}
			else {
				break;
				bOk= FALSE;
			}
		}

		if( bOk)
			OutZ("_db_for_sql_:",__FILE__,__LINE__," 结束获取光标 成功 ");
		else
			OutZ("_db_for_sql_:",__FILE__,__LINE__," 结束获取光标 失败 ");
		return TRUE;
	}




	//COptex optexSingle;   // Create a single-process optex
	CFastMutex mutex;

	CMySQLDB(void);
	virtual ~CMySQLDB();
	// TODO: 在此添加您的方法。
};

extern MY_DB_HELPER_API int ndb_for_mysql;

MY_DB_HELPER_API int fndb_for_mysql(void);
