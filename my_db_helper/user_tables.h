#pragma once

#include "stdafx.h"
#include "my_db_helper.h"

struct MY_DB_HELPER_API tbl_Decoders : public my_table_type_base
{
	tbl_Decoders()
	{
		tableName = "_420_Decoders";

		my_field_type afield;
		afield.fieldName = "name";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);
		
		
		afield.fieldName = "ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "xpos";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "ypos";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "wpos";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "hpos";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		for(size_t i=0;i< fieldsList.size(); i++)
		{
			if( fieldsList[i].fieldName == "name" )
			{
				iDefaultOrderFiled = i;
			}
		}
	}
};



struct MY_DB_HELPER_API my_tbl_device_tree : public my_table_type_base
{
	my_tbl_device_tree()
	{
		tableName = "device_tree";
		my_field_type afiled;
		afiled.fieldName = "id";
		afiled.fieldType = SQL_INTEGER;
		fieldsList.push_back(afiled);

		afiled.fieldName = "pid";
		afiled.fieldType = SQL_INTEGER;
		fieldsList.push_back(afiled);

		afiled.fieldName = "device_id";
		afiled.fieldType = SQL_CHAR;
		fieldsList.push_back(afiled);

		afiled.fieldName = "name";	
		afiled.fieldType = SQL_CHAR;
		fieldsList.push_back(afiled);

		afiled.fieldName = "node_type";
		afiled.fieldType = SQL_INTEGER;
		fieldsList.push_back(afiled);
	}

	bool operator==(const my_table_type_base& ref)
	{
		if( 
			ref.getIntValueByName("id")        == getIntValueByName("id") &&
			ref.getIntValueByName("pid")       == getIntValueByName("pid") &&
			ref.getStrValueByName("device_id") == getStrValueByName("device_id") &&
			ref.getStrValueByName("name")      == getStrValueByName("name") && 
			ref.getIntValueByName("node_type") == getIntValueByName("node_type") 
			) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

struct MY_DB_HELPER_API my_tbl_device_detail: public my_table_type_base
{
	my_tbl_device_detail()
	{
		tableName = "device_detail";

		my_field_type afield;

		afield.fieldName = "device_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "name";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);


		afield.fieldName = "type";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);

	

		afield.fieldName = "ip";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);


		afield.fieldName = "port";
		afield.fieldType = SQL_INTEGER;
		fieldsList.push_back(afield);


		afield.fieldName = "channel";
		afield.fieldType = SQL_INTEGER;
		fieldsList.push_back(afield);

		afield.fieldName = "idx";
		afield.fieldType = SQL_INTEGER;
		fieldsList.push_back(afield);

		afield.fieldName = "tag";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);

	}

	bool operator==(const my_table_type_base& ref)
	{

		
		if( 
			ref.getStrValueByName("device_id") == getStrValueByName("device_id") &&
			ref.getStrValueByName("name") == getStrValueByName("name") &&
			ref.getStrValueByName("type") == getStrValueByName("type") &&
			ref.getStrValueByName("ip") == getStrValueByName("ip") &&
			ref.getIntValueByName("port") == getIntValueByName("port") &&
			ref.getIntValueByName("channel") == getIntValueByName("channel") &&
			ref.getIntValueByName("idx") == getIntValueByName("idx") &&
			ref.getStrValueByName("tag") == getStrValueByName("tag") 

			)
		{
			return true;
		}
		else
		{	
			return false;
		}
	}

};

struct MY_DB_HELPER_API my_tbl_device_type: public my_table_type_base
{
	my_tbl_device_type()
	{
		tableName = "device_type";
		my_field_type afield;
		
		afield.fieldName = "type_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "typename";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);

		afield.fieldName = "name";
		afield.fieldType = SQL_CHAR;
		fieldsList.push_back(afield);

		afield.fieldName = "ocx_type";
		afield.fieldType = SQL_INTEGER;
		fieldsList.push_back(afield);
	}


	static my_tbl_device_type* findTypeByName(std::vector<my_tbl_device_type>& typeLists, std::string& strTypeName)
	{
		for(size_t i=0; i<typeLists.size(); i++)
		{
			std::string strNameInDb = typeLists[i].getStrValueByName("typename");
			if( strNameInDb  == strTypeName )
			{
				return &typeLists[i];
			}
		}

		return NULL;
	}
};


struct MY_DB_HELPER_API my_tble_auto_switch : public my_table_type_base
{
	my_tble_auto_switch()
	{
		tableName = "_420_auto_switch";
		my_field_type afield;

		afield.fieldName = "switch_plan_name";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "weekofday";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);


		afield.fieldName = "start_exec_time";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "end_exec_time";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

	
		afield.fieldName = "decoder_ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "decoder_name";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "switch_order";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "encoder_ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_channel";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);
		setIntValueByName("encoder_channel", -1);


		afield.fieldName = "encoder_device_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);
	}
};


struct MY_DB_HELPER_API tbl_config_group: public my_table_type_base
{
	tbl_config_group()
	{
		tableName = "config_group";
		my_field_type afield;

		afield.fieldName = "group_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = TRUE;
		fieldsList.push_back(afield);
		
		afield.fieldName = "name";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "data";
		afield.fieldType = SQL_BINARY;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data base for mdb
//

struct MY_DB_HELPER_API mdb_DecoderPlan_420 : public my_table_type_base
{
	mdb_DecoderPlan_420() : my_table_type_base()
	{
		fieldsList.clear();
		tableName = "DecoderPlan";
		
		my_field_type afield;

		afield.fieldName = "iPlanNum";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iIsUse";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		
		afield.fieldName = "iIsAuto";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


	}
};

struct MY_DB_HELPER_API mdb_decoder_info_420 : public my_table_type_base 
{
	mdb_decoder_info_420() :  my_table_type_base()
	{
		fieldsList.clear();

		tableName = "decoder_info";
		my_field_type afield;

		afield.fieldName = "iDataType";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iIndex";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iStartPosX";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iStartPosY";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "iWidth";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iHeight";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);
		
		afield.fieldName = "strDecoderIp";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "strDecoderName";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "iChannel";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "strDecoderType";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);
		
	}
};


struct MY_DB_HELPER_API mdb_encoder_info_420 : public my_table_type_base
{
	mdb_encoder_info_420()
	{
		tableName = "encoder_info";
		my_field_type afield;

		afield.fieldName = "decode_Pid";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);
		
		
		afield.fieldName = "decode_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = TRUE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_name";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_mask";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_locate";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_state";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_IsLeaf";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_iMovePort"; 
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_MoveAddress";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);
		

		afield.fieldName = "decode_iMoveSpeed";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_IsCoder";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_MatrixNo";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);
		

		afield.fieldName = "decode_IsTrans";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);
	
		afield.fieldName = "decode_TransIP";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_Type";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


	}
};

struct MY_DB_HELPER_API mdb_encoder_info : public my_table_type_base
{
	mdb_encoder_info()
	{
		tableName = "decode_info";
		my_field_type afield;
		
		afield.fieldName = "decode_Pid";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_name";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_mask";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_locate";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_state";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_IsLeaf";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "decode_iMovePort"; 
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_MoveAddress";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_iMoveSpeed";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_iChannel";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_IsTrans";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);
	
		afield.fieldName = "decode_TransIP";
		afield.fieldType = SQL_CHAR;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iKind";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "iType";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "decode_num";
		afield.fieldType = SQL_INTEGER;
		afield.isPK      = FALSE;
	
		fieldsList.push_back(afield);

	}

};



struct MY_DB_HELPER_API mdb_GroupEncoder_420 : public my_table_type_base
{
	bool operator==(const mdb_encoder_info_420& refRec)
	{
		std::string fd_encoder_id = this->getStrValueByName("encoder_id");
		std::string fd_encoder_id0 = refRec.getStrValueByName("decode_id");
		if( fd_encoder_id != fd_encoder_id0 )
			return false;

		std::string fd_encoder_ip = this->getStrValueByName("encoder_ip");
		std::string fd_encoder_ip0 = refRec.getStrValueByName("decode_ip");


		if( fd_encoder_ip!= fd_encoder_ip0)
			return false;


		int fd_encoder_channel = this->getIntValueByName("encoder_channel");
		int fd_encoder_channel0 = refRec.getIntValueByName("decode_iMoveSpeed");

		if( fd_encoder_channel!= fd_encoder_channel0)
			return false;

		int fd_encoder_IsTrans = this->getIntValueByName("encoder_IsTrans");
		int fd_encoder_IsTrans0 = refRec.getIntValueByName("decode_IsTrans");

		if( fd_encoder_IsTrans!= fd_encoder_IsTrans0)
			return false;

		std::string fd_encoder_TransIP = this->getStrValueByName("encoder_TransIP");
		std::string fd_encoder_TransIP0 = refRec.getStrValueByName("decode_TransIP");

		if( fd_encoder_TransIP!= fd_encoder_TransIP0)
			return false;

		std::string fd_encoder_Type = this->getStrValueByName("encoder_Type");
		std::string fd_encoder_Type0 = refRec.getStrValueByName("decode_Type");

		if( fd_encoder_Type!= fd_encoder_Type0)
			return false;


		return true;

	}


	bool operator==(const mdb_GroupEncoder_420& refRec)
	{
		std::string fd_encoder_id = this->getStrValueByName("encoder_id");
		std::string fd_encoder_id0 = refRec.getStrValueByName("encoder_id");

		if( fd_encoder_id!= fd_encoder_id0)
			return false;

		std::string fd_encoder_name = this->getStrValueByName("encoder_name");
		std::string fd_encoder_name0 = refRec.getStrValueByName("encoder_name");

		if( fd_encoder_name!= fd_encoder_name0)
			return false;

		std::string fd_encoder_ip = this->getStrValueByName("encoder_ip");
		std::string fd_encoder_ip0 = refRec.getStrValueByName("encoder_ip");


		if( fd_encoder_ip!= fd_encoder_ip0)
			return false;


		int fd_igroupNum = this->getIntValueByName("groupNum");
		int fd_igroupNum0 = refRec.getIntValueByName("groupNum");


		if( fd_igroupNum!= fd_igroupNum0)
			return false;

		int fd_planNum = this->getIntValueByName("planNum");
		int fd_planNum0 = refRec.getIntValueByName("planNum");

		if( fd_planNum!= fd_planNum0)
			return false;

		int fd_encoder_channel = this->getIntValueByName("encoder_channel");
		int fd_encoder_channel0 = refRec.getIntValueByName("encoder_channel");

		if( fd_encoder_channel!= fd_encoder_channel0)
			return false;

		int fd_encoder_IsTrans = this->getIntValueByName("encoder_IsTrans");
		int fd_encoder_IsTrans0 = refRec.getIntValueByName("encoder_IsTrans");

		if( fd_encoder_IsTrans!= fd_encoder_IsTrans0)
			return false;

		std::string fd_encoder_TransIP = this->getStrValueByName("encoder_TransIP");
		std::string fd_encoder_TransIP0 = refRec.getStrValueByName("encoder_TransIP");

		if( fd_encoder_TransIP!= fd_encoder_TransIP0)
			return false;

		std::string fd_encoder_Type = this->getStrValueByName("encoder_Type");
		std::string fd_encoder_Type0 = refRec.getStrValueByName("encoder_Type");

		if( fd_encoder_Type!= fd_encoder_Type0)
			return false;

		return true;
	}

	mdb_GroupEncoder_420()
	{
		tableName = "GroupEncoder";
		my_field_type afield;

		afield.fieldName = "encoder_id";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_name";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_ip";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "groupNum";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);


		afield.fieldName = "planNum";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_channel";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_IsTrans";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

		afield.fieldName = "encoder_TransIP";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);


		afield.fieldName = "encoder_Type";
		afield.fieldType = SQL_CHAR;
		afield.isPK = FALSE;
		fieldsList.push_back(afield);

	}
};




/*
S_3387

*/


struct MY_DB_HELPER_API mdb_A_B_C : public my_table_type_base
{
	mdb_A_B_C()
	{
		tableName = "A_B_C";
		my_field_type afield;

		afield.fieldName = "AAAA";
		afield.fieldType = SQL_INTEGER;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName = "FFFF";
		afield.fieldType = SQL_CHAR;
		afield.isPK = TRUE;
		fieldsList.push_back(afield);
	}
};

struct MY_DB_HELPER_API mdb_SolutionNames : public my_table_type_base
{
	mdb_SolutionNames()
	{
		tableName = "SolutionNames";
		my_field_type afield;

		afield.fieldName  = "iSolutionIndex";
		afield.fieldType  = SQL_INTEGER;
		afield.isPK       = TRUE;
		fieldsList.push_back(afield);

		afield.fieldName  = "strSolutionName";
		afield.fieldType  = SQL_CHAR;
		afield.isPK       = FALSE;
		fieldsList.push_back(afield);
	}
	
};

struct MY_DB_HELPER_API mdb_SolutionTimeSpans : public my_table_type_base
{
	mdb_SolutionTimeSpans()
	{
		tableName = "SolutionTimeSpan";
		my_field_type afield;
		
		afield.fieldName  = "strSolutionString";
		afield.fieldType  = SQL_CHAR;
		afield.isPK       = TRUE;
		fieldsList.push_back(afield);
	}
};


///////////////////////////////////////////////////////////////////////////////////////

class MY_DB_HELPER_API Personal
{
public:
	Personal();
	Personal(char* ,char *, int, long);

	void writeToFile (std::fstream&) const;
	void readFromFile(std::fstream&);

	void readKey();



	size_t size() const {
		return nLength;	
	}

	size_t nLength;


	std::ostream& writeLegibly(std::ostream& );
	friend std::ostream& operator<<( std::ostream& out, Personal& pr)
	{
		return pr.writeLegibly(out);
	}	


	std::istream& readFromConsole(std::istream&);
	friend std::istream& operator>>( std::istream& in, Personal& pr) 
	{
		return pr.readFromConsole(in);
	}
};


template<class T>
class database01
{
public:
	database01();
	void run();

private:
	std::fstream database;
	char fName[20];
	std::ostream& print(std::ostream&);


public:
	void add(T&);
	bool find(const T&);
	void modify(const T&);

	friend std::ostream& operator<<(std::ostream& out, database01& db)
	{
		return db.print(out);
	}



};

template<class T>
database01<T>::database01()
{
	strcpy(fName,"autoswitch.dat");
}

template<class T>
void database01<T>::add(T& d)
{
	database.open(fName, std::ios::in | std::ios::out | std::ios::binary);
	database.seekp(0, std::ios::end);
	d.writeToFile(database);
	database.close();

}