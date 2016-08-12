#include "Psql.h"
#include "orm-psql.h"
#include <net/Internet.h>
#include <base/Singleton.h>
#include <base/Logger.hpp>

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>

using namespace codechiev::base;
using namespace codechiev::db;

PSql::PSql():inUse_(false)
{}
PSql::~PSql()
{}

bool
PSql::connect(const char *conninfo)
{
    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);
    
    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        LOG_ERROR<<"Connection to database failed: "<<PQerrorMessage(conn);
        PQfinish(conn);
        return false;
    }
    return true;
}

void
PSql::close()
{
    LOG_DEBUG<<"close the connection to the database and cleanup";
    PQfinish(conn);
}

void
PSql::transactionBegin()
{
    /* Start a transaction block */
    PGresult *res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        printf( "BEGIN command failed: \n");
        return;
    }
    PQclear(res);
}

void
PSql::transactionEnd()
{
    /* end the transaction */
    PGresult *res = PQexec(conn, "END");
    PQclear(res);
}


PSql::Result
PSql::query(const char *sql,
            int nParams,
            //const Oid *paramTypes,
            const char * const *paramValues,
            const int *paramLengths,
            const int *paramFormats,
            int resultFormat)
{
    PSqlManager* manager = Singleton<PSqlManager >::get();
    psql_ptr psql = manager->getDB();

    psql->transactionBegin();

    //const char *conninfo;
    Result result;
    result.res = PQexecParams(psql->conn,
               sql,
               nParams,       /* The number of parameters supplied; */
               NULL,    /* let the backend deduce param type */
               paramValues,
               paramLengths,    /* actual data lengths of binary-format. */
               paramFormats,    /* 0 for text, 1 for binary in array */
               1);      /* 0 for text, or 1 for binary format */

    if (PQresultStatus(result.res) != PGRES_COMMAND_OK)
    {
        printf("PQexecParams failed: %s\n", PQerrorMessage(psql->conn));
        result.freeAll();
        return result;
    }

    psql->transactionEnd();

    manager->returnDB(psql);

    return result;
}

void
PSql::select(Result& result, const char *sql,
            int nParams,
            //const Oid *paramTypes,
            const char * const *paramValues,
            const int *paramLengths,
            const int *paramFormats,
            int resultFormat)
{
    PSqlManager* manager = Singleton<PSqlManager >::get();
    psql_ptr psql = manager->getDB();

    psql->transactionBegin();

    //const char *conninfo;
    result.res = PQexecParams(psql->conn,
                              sql,
                              nParams,       /* The number of parameters supplied; */
                              NULL,    /* let the backend deduce param type */
                              paramValues,
                              paramLengths,    /* actual data lengths of binary-format. */
                              paramFormats,    /* 0 for text, 1 for binary in array */
                              1);      /* 0 for text, or 1 for binary format */

    if (PQresultStatus(result.res) != PGRES_TUPLES_OK)
    {
        printf("PQexecParams failed: %s\n", PQerrorMessage(psql->conn));
        result.freeAll();
    }

    psql->transactionEnd();

    manager->returnDB(psql);
}

PSql::Result
PSql::queryById(const char *sql,int64_t id)
{
    PSqlManager* manager = Singleton<PSqlManager >::get();
    psql_ptr psql = manager->getDB();

    psql->transactionBegin();

    Result result;
    const char *paramValues[1];
    int paramLengths[1];
    int paramFormats[1];
    uint64_t binaryIntId = net::hostToNetworkInt64(id);
    paramValues[0] = (char *) &binaryIntId;
    paramLengths[0] = sizeof(binaryIntId);
    paramFormats[0] = 1;

    result.res = PQexecParams(psql->conn,
                      sql,
                      1,       /* one param */
                      NULL,    /* let the backend deduce param type */
                      paramValues,
                      paramLengths,    /* don't need param lengths since text */
                      paramFormats,    /* default to all text params */
                      1);      /* ask for binary results */

    if (PQresultStatus(result.res) != PGRES_COMMAND_OK)//PGRES_TUPLES_OK
    {
        fprintf(stderr, "PQexecParams failed: %s", PQerrorMessage(psql->conn));
        result.freeAll();
        return result;
    }

    psql->transactionEnd();

    return result;
}

void
PSql::selectById(Result& rt, const char *sql,int64_t id)
{
    //PGconn     *conn;
    //PGresult   *res;
    const char *paramValues[1];
    int         paramLengths[1];
    int         paramFormats[1];
    uint64_t    binaryIntVal;
    
    PSqlManager* manager = Singleton<PSqlManager >::get();
    psql_ptr psql = manager->getDB();

    /* Convert integer value "2" to network byte order */
    binaryIntVal = net::hostToNetworkInt64(id);
    
    /* Set up parameter arrays for PQexecParams */
    paramValues[0] = (char *) &binaryIntVal;
    paramLengths[0] = sizeof(binaryIntVal);
    paramFormats[0] = 1;        /* binary */
    
    rt.res = PQexecParams(psql->conn,
                       sql,
                       1,       /* one param */
                       NULL,    /* let the backend deduce param type */
                       paramValues,
                       paramLengths,
                       paramFormats,
                       1);      /* ask for binary results */
    
    if (PQresultStatus(rt.res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(psql->conn));
        PQclear(rt.res);
        psql->close();
        return;
    }
    
    manager->returnDB(psql);
}

PSql::Result::Result():res(NULL){}
void
PSql::Result::freeAll(){PQclear(res);}
int
PSql::Result::getRowSize()
{
    return PQntuples(res);
}
char*
PSql::Result::getFieldValOfRow(const char *field, int row)
{
    int fnum = PQfnumber(res, field);

    if (PQntuples(res)>row)
    {
        char *val = PQgetvalue(res, row, fnum);
        //printf("%s(%d bytes)\n", field,  PQgetlength(res, 0, fnum));
        return val;
    }
    return NULL;
}
