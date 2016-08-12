#ifndef PSQL_H
#define PSQL_H

#include <stdio.h>
#include <stdint.h>
#include <libpq-fe.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "SqlManager.h"

namespace codechiev {
    namespace db {
    class PSql:boost::noncopyable
    {
    public:
        PSql();
        ~PSql();

        class Result;
        static Result query(const char *sql,
                            int nParams,
                            //const Oid *paramTypes,
                            const char * const *paramValues,
                            const int *paramLengths,
                            const int *paramFormats,
                            int resultFormat);
        static void select(Result& result, const char *sql,
                            int nParams,
                            //const Oid *paramTypes,
                            const char * const *paramValues,
                            const int *paramLengths,
                            const int *paramFormats,
                            int resultFormat);
        static Result queryById(const char *sql,int64_t id);
        static void selectById(Result& result, const char *sql, int64_t id);

        bool connect(const char *);
        void close();
        void transactionBegin();
        void transactionEnd();
        inline bool isInUse(){return inUse_;}
        inline void setUsed(bool used){inUse_=used;}

        PGconn *conn;
    private:
        bool inUse_;

    public:

        class Result
        {
        public:
            Result();

            char* getFieldValOfRow(const char*, int);
            int getRowSize();
            void freeAll();
            PGresult* res;
        };
    };
    typedef boost::shared_ptr<PSql> psql_ptr;
    typedef DBManager<PSql, 1> PSqlManager1;
    }
}


#endif // PSQL_H
