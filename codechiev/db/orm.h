#ifndef ORM_H_INCLUDED
#define ORM_H_INCLUDED

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/cat.hpp>

//BOOST_PP_TUPLE_ELEM(num of elems, elem pos, elem tuple)
#define TABLE_MEMBER(R, data, TYPE_AND_NAME) \
    Field<BOOST_PP_TUPLE_ELEM(3, 0, TYPE_AND_NAME) > BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME);

#define TABLE_ASSIGN(R, row, TYPE_AND_NAME) \
    BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).toField(result.getFieldValOfRow( BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 2, TYPE_AND_NAME)), row));
#define TABLE_ROW_ASSIGN(R, row, TYPE_AND_NAME) \
    clz->BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).toField(result.getFieldValOfRow( BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 2, TYPE_AND_NAME)), row));

#define PSQL_ASSIGN( r, data, i, TYPE_AND_NAME) \
    val[i] = BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).getNetValue();\
    len[i] = BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).getSizeof();\
    format[i] = 1;\

#define PSQL_PLACE_HOLDER( r, data, i, TYPE_AND_NAME) \
    ",$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,i))

#define PSQL_UPDATE( r, data, i, TYPE_AND_NAME) \
    ", " BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 2, TYPE_AND_NAME)) "=$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,i))

#include <net/Internet.h>
namespace codechiev {
    namespace db {
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
                netval_ = net::hostToNetworkInt64(t_);
                return reinterpret_cast<const char*>(&netval_);
            }
            inline void setValue(const uint64_t& t){t_=t;};
            inline int getSizeof(){return sizeof(t_);}
            inline void toField(char * ptr)
            {
                if(ptr)
                    setValue(net::hostToNetworkInt64(*(uint64_t *)ptr));
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
                netval_ = net::hostToNetworkInt32(t_);
                return reinterpret_cast<const char*>(&netval_);
            }
            inline void setValue(const uint32_t& t){t_=t;};
            inline int getSizeof(){return sizeof(t_);}
            inline void toField(char * ptr)
            {
                if(ptr)
                    setValue(net::hostToNetworkInt32(*(uint32_t *)ptr));
            }
        protected:
        private:
            int32_t t_;
            uint32_t netval_;
        };
    }
}
#endif // ORM_H_INCLUDED
