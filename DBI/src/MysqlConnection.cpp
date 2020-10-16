#include "MysqlConnection.h"
#include "MysqlStatementStruct.h"
#include "ConfigCenter.h"
#include "Log.h"

using namespace DBI;
using namespace Config;
using namespace std;

//-----------------------------------------------------------------------------

MysqlLibHandler MysqlConnection::mysqlLibHandlerM;

//-----------------------------------------------------------------------------

MysqlConnection::MysqlConnection(const std::string& theCfgGroup)
    : mysqlM(NULL)
    , cfgGroupM(theCfgGroup)
{
}

//-----------------------------------------------------------------------------

MysqlConnection::~MysqlConnection()
{
    close();
}

//-----------------------------------------------------------------------------

int MysqlConnection::connect()
{
    if(NULL == (mysqlM = mysql_init(NULL)))
    {
        LOG_ERROR("Mysql_init fails!");
        return -1;
    }
    
    std::string host = g_cfg->get(cfgGroupM+".host", "127.0.0.1");
    std::string user = g_cfg->get(cfgGroupM+".user", "test");
    std::string pswd = g_cfg->get(cfgGroupM+".pswd", "test");
    std::string dbname = g_cfg->get(cfgGroupM+".db", "test");
    int dbport = g_cfg->get(cfgGroupM+".port", 3306);
    if (!mysql_real_connect(mysqlM, host.c_str(), user.c_str(), pswd.c_str(), 
                dbname.c_str(), dbport, NULL, 0))
    {
        LOG_ERROR("fail to connect mysql:" << user << "@" << host << ":" << dbport << " " << dbname
                << ". errmsg:" << mysql_error(mysqlM));
        close();
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

void MysqlConnection::close()
{
    if (mysqlM)
    {
        mysql_close(mysqlM);
        mysqlM = NULL;
    }
}

//-----------------------------------------------------------------------------

int MysqlConnection::processStatement(MysqlStatement* theStatement)
{
    if (NULL == mysqlM)
    {
        LOG_WARN("Connection is not connected!");
        return -1;
    }

    return theStatement->execute(mysqlM);
}

//-----------------------------------------------------------------------------

