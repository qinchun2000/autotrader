#include "csvfile.hpp"


void    CSVFile::ReadCSVHead()
{

//	printf("ReadCSVHead ->m_nFileState   %d  \n",m_nFileState);
    char strHeadLine[4096];
    m_CSVFile.getline(strHeadLine, sizeof(strHeadLine));

    RowParse(strHeadLine, sizeof(strHeadLine), m_CSVHead);

//    for (auto &item:m_CSVHead)
//    {
//    	printf(" ReadCSVHead ----> m_CSVHead--> %s  \n ",item.c_str());
//    }
}

void    CSVFile::RowParse(const char* strRow, int nSize, ROWVEC& result)
{
    result.clear();

    bool    bIsInWord   = false;
    bool    bIsHaveSpace    = false;
    std::string strCurWorld;

    for (int i=0; i<nSize; i++)
    {
//    	printf(" %d    \n",i);

        char ch = strRow[i];
        if (ch == '\0')
        {
            if (i >= 1 && strRow[i-1] == ',')
            {
                strCurWorld = ' ';
            }

            break;
        }

        bool bIsAdd = true;

        switch (ch)
        {
        case ',':
            {
                if (!bIsInWord)
                {
                    // 一项结束

                    result.push_back(strCurWorld);
//                    printf("RowParse  --->item:   %s  \n",strCurWorld.c_str());

                    bIsInWord   = false;
                    bIsHaveSpace    = false;
                    strCurWorld = "";

                    bIsAdd  = false;
                }
            }
            break;
        case '"':
            {
                if (!bIsInWord)
                {
                    bIsInWord   = true;
                    bIsHaveSpace    = true;

                    bIsAdd  = false;
                }
                else
                {
                    if ('"' == strRow[i+1])
                    {
                        i++;
                    }
                    else if (bIsHaveSpace)
                    {
                        bIsInWord   = false;

                        bIsAdd  = false;
                    }
                    else
                    {
                        assert(0);
                    }

                }
            }
            break;
        default:
            //bIsInWord = true;
            break;
        };

        if (bIsAdd)
        {
            strCurWorld += ch;
        }

    }

    if (strCurWorld != "")
    {
        result.push_back(strCurWorld);
    }

}

int     CSVFile::FindField(const char* strRow)
{
	int ret;
    if (m_nFileState == FILE_STATE_NULL)
    {
//    	printf("FindField -->m_nFileState   %d  \n",m_nFileState);
        return -1;
    }
    else
    {
//    	printf("FindField -->m_nFileState   %d  \n",m_nFileState);
    }

    for (ROWVEC::iterator it = m_CSVHead.begin();
        it != m_CSVHead.end(); it++)
    {
    	std::string a =*it;
//    	printf("FindFiled head  %s  -- [%d]\n ",a.c_str(),int(it - m_CSVHead.begin()));

        if (*it == strRow)
        {
            return int(it - m_CSVHead.begin());
        }
    }



    std::string date ="日期";
    std::string time ="时间";
    std::string ms ="毫秒";
    std::string lastprice ="最新价";
    std::string dif_cjl ="成交差";
    std::string cjl ="成交量";
    std::string dif_opi ="持仓差";
    std::string opi ="持仓量";
    std::string buy1_p ="买1价";
    std::string buy1_v ="买1量";
    std::string sell1_p ="卖1价";
    std::string sell1_v ="卖1量";
    std::string average_price ="均价";

    if (strcmp(strRow,date.c_str())==0)
    {

    	return 0;
    }
    else if (strcmp(strRow,time.c_str())==0)
   {

	return 1;
   }
    else if (strcmp(strRow,ms.c_str())==0)
   {

	return 2;
   }
    else if (strcmp(strRow,lastprice.c_str())==0)
  {

return 3;
  }
    else if (strcmp(strRow,dif_cjl.c_str())==0)
	 {

	return 4;
	 }
    else if (strcmp(strRow,cjl.c_str())==0)
	 {

	return 5;
	 }
    else if (strcmp(strRow,dif_opi.c_str())==0)
	 {

	return 6;
	 }
    else if (strcmp(strRow,opi.c_str())==0)
	 {

	return 7;
	 }
    else if (strcmp(strRow,buy1_p.c_str())==0)
    	 {

    	return 8;
    	 }
    else if (strcmp(strRow,buy1_v.c_str())==0)
    	 {

    	return 9;
    	 }
    else if (strcmp(strRow,sell1_p.c_str())==0)
    	 {

    	return 10;
    	 }
    else if (strcmp(strRow,sell1_v.c_str())==0)
    	 {

    	return 11;
    	 }
    else if (strcmp(strRow,average_price.c_str())==0)
    	 {

    	return 12;
    	 }



    return -1;
}


bool    CSVFile::Open(bool bIsRead, const char* strPath, const char* strFilename)
{
    m_nFileState    = FILE_STATE_NULL;
    std::string strFullFileName(strPath);
    strFullFileName += strFilename;

    m_CSVFile.open(strFullFileName.c_str(), std::ios_base::in);
    if (!m_CSVFile.is_open())
    {
        return false;
    }

    ReadCSVHead();

    if (bIsRead)
    {
        m_nFileState    = FILE_STATE_READ;
    }
    else
    {
        m_nFileState    = FILE_STATE_WRITE;
        m_CSVFile.close();
        m_CSVFile.open(strFullFileName.c_str(), std::ios_base::out);
        if (!m_CSVFile.is_open())
        {
            return false;
        }

        m_CSVCurRow = m_CSVHead;
        CSVWriteNextRow();
    }

    return true;
}

// 读接口
bool    CSVFile::CSVReadNextRow()
{
//	printf("CSVReadNextRow ->    \n");
    if (m_nFileState != FILE_STATE_READ)
    {
    	printf("CSVReadNextRow ->  FILE_STATE_READ error  \n");
        return false;
    }

    char strHeadLine[4096];
    m_CSVFile.getline(strHeadLine, sizeof(strHeadLine));
    if (m_CSVFile.eof())
    {

//    	printf("CSVReadNextRow ->  eof  \n");
        return false;
    }

    RowParse(strHeadLine, sizeof(strHeadLine), m_CSVCurRow);

    return true;
}

int replace(std::string& src_str, const std::string& old_str, const std::string& new_str)
{
    int count   = 0;
    int old_str_len = int(old_str.length());
    int new_str_len = int(new_str.length());
    int pos = 0;
    while((pos=int(src_str.find(old_str,pos)))!=std::string::npos)
    {
        src_str.replace(pos,old_str_len,new_str);
        pos+=new_str_len;
        ++count;
    }
    return count;
}

// 写接口
void    CSVFile::CSVWriteNextRow()
{
    if (m_nFileState != FILE_STATE_WRITE)
    {
        return;
    }

    for (ROWVEC::iterator it    = m_CSVCurRow.begin();
        it != m_CSVCurRow.end(); it++)
    {
        std::string str = *it;
        *it = "";
        replace(str, "\"", "\"\"");

        m_CSVFile << '"' << str << '"';
        if (it + 1 != m_CSVCurRow.end())
        {
            m_CSVFile << ',';
        }
    }

    m_CSVFile << std::endl;
}
