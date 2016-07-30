#include "Psql.h"
#include "orm-psql.h"
#include <net/Internet.h>
#include <base/Singleton.h>

#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>

using namespace codechiev::base;
using namespace codechiev::db;

PSql::PSql():inUse_(false)
{}
PSql::~PSql()
{   /* close the connection to the database and cleanup */
    PQfinish(conn);
}

bool
PSql::connect(const char *conninfo)
{
    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf( "psql connection failed: \n");
        return false;
    }
    return true;
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
    psql_ptr psql = Singleton<PSqlManager1 >::get()->getDB();

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
        PQclear(result.res);
        return result;
    }

    psql->transactionEnd();

    Singleton<PSqlManager1 >::get()->returnDB(psql);

    return result;
}

PSql::Result
PSql::select(const char *sql,
            int nParams,
            //const Oid *paramTypes,
            const char * const *paramValues,
            const int *paramLengths,
            const int *paramFormats,
            int resultFormat)
{
    psql_ptr psql = Singleton<PSqlManager1 >::get()->getDB();

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

    if (PQresultStatus(result.res) != PGRES_TUPLES_OK)
    {
        printf("PQexecParams failed: %s\n", PQerrorMessage(psql->conn));
        PQclear(result.res);
        return result;
    }

    psql->transactionEnd();

    Singleton<PSqlManager1 >::get()->returnDB(psql);

    return result;
}

PSql::Result
PSql::queryById(const char *sql,int64_t id)
{
    psql_ptr psql = Singleton<PSqlManager1 >::get()->getDB();

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
        PQclear(result.res);
        return result;
    }

    psql->transactionEnd();

    return result;
}

PSql::Result
PSql::selectById(const char *sql,int64_t id)
{
    psql_ptr psql = Singleton<PSqlManager1 >::get()->getDB();

    psql->transactionBegin();

    Result result;
    const char *paramValues[1];
    int paramLengths[1];
    int paramFormats[1];
    uint64_t binaryIntId = hostToNetworkInt64(id);
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

    if (PQresultStatus(result.res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "PQexecParams failed: %s", PQerrorMessage(psql->conn));
        PQclear(result.res);
        return result;
    }

    psql->transactionEnd();

    return result;
}

PSql::Result::Result():res(nullptr){}
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
    return nullptr;
}
