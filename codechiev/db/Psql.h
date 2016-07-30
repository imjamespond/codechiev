#ifndef PSQL_H
#define PSQL_H

#include <stdio.h>
#include <stdint.h>
#include <libpq-fe.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <net/Internet.h>
#include "sql_manager.h"

namespace codechiev {
    namespace psql {
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
        static Result select(const char *sql,
                            int nParams,
                            //const Oid *paramTypes,
                            const char * const *paramValues,
                            const int *paramLengths,
                            const int *paramFormats,
                            int resultFormat);
        static Result queryById(const char *sql,int64_t id);
        static Result selectById(const char *sql,int64_t id);

        bool connect(const char *);
        void transactionBegin();
        void transactionEnd();
        bool isInUse(){return inUse_;}
        void setUsed(bool used){inUse_=used;}

        PGconn *conn;
    private:
        bool inUse_;

    public:

        class Param
        {
        public:

        };

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
    typedef boost::shared_ptr<PSql> PSqlPtr;

    template<class T>
    class Field
    {
    public:
        inline T& getValue(){return t_;};
        inline void setValue(const T& t){t_=t;};
        inline void toField( char * ptr){if(ptr) setValue(ptr);}
        inline int getSizeof(){return sizeof(t_);}
    protected:
    private:
        T t_;
    };


    template<>
    class Field<std::string>
    {
    public:
        inline std::string getValue(){return t_;};
        inline const char* getNetValue(){return t_.c_str();};
        inline void setValue(const std::string& t){t_=t;};
        inline int getSizeof()
        {return static_cast<int>(t_.length());}
        inline void toField( char * ptr){if(ptr) setValue(ptr);}
    protected:
    private:
        std::string t_;
    };

    template<>
    class Field<int64_t>
    {
    public:
        inline int64_t getValue(){return t_;};
        inline const char* getNetValue()
        {
            netval_ = hostToNetworkInt64(t_);
            return reinterpret_cast<const char*>(&netval_);
        }
        inline void setValue(const uint64_t& t){t_=t;};
        inline int getSizeof(){return sizeof(t_);}
        inline void toField(char * ptr)
        {
            if(ptr)
                setValue(hostToNetworkInt64(*(uint64_t *)ptr));
        }
    protected:
    private:
        int64_t t_;
        uint64_t netval_;
    };

    template<>
    class Field<int32_t>
    {
    public:
        inline int32_t getValue(){return t_;};
        inline const char* getNetValue()
        {
            netval_ = hostToNetworkInt32(t_);
            return reinterpret_cast<const char*>(&netval_);
        }
        inline void setValue(const uint32_t& t){t_=t;};
        inline int getSizeof(){return sizeof(t_);}
        inline void toField(char * ptr)
        {
            if(ptr)
                setValue(hostToNetworkInt32(*(uint32_t *)ptr));
        }
    protected:
    private:
        int32_t t_;
        uint32_t netval_;
    };

    typedef boost::shared_ptr<PSql> psql_ptr;
    typedef DBManager<PSql, 1> PSqlManager1;

    }
}


#endif // PSQL_H
