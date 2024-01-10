#pragma once
#include "mysql_conn.h"
class SqlConnUtils
{
private:
        static mysql_conn *m_SqlConn;


public:

    static mysql_conn* GetConnection()
    {
        return m_SqlConn;
    }
    static void SetConnection(mysql_conn* p_Connection)
    {
        m_SqlConn = p_Connection;
    }
};
