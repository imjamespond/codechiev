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
        static void query(Result& ,
                            const char *,
                            int ,
                            //const Oid *paramTypes,
                            const char * const *,
                            const int *,
                            const int *,
                            int );
        static void select(Result& result,
                            const char *sql,
                            int nParams,
                            //const Oid *paramTypes,
                            const char * const *paramValues,
                            const int *paramLengths,
                            const int *paramFormats,
                            int resultFormat);
        static void queryById( Result& ,const char *, int64_t );
        static void selectById(Result& ,const char *, int64_t );

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
    typedef DBManager<PSql, 1> PSqlManager;
    }
}


#endif // PSQL_H
