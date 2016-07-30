#ifndef ORM_PSQL_H_INCLUDED
#define ORM_PSQL_H_INCLUDED

#include "orm.h"

#define PSQL_ASSIGN( r, data, i, TYPE_AND_NAME) \
    val[i] = BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).getNetValue();\
    len[i] = BOOST_PP_TUPLE_ELEM(3, 1, TYPE_AND_NAME).getSizeof();\
    format[i] = 1;\

#define PSQL_INSERT( r, data, i, TYPE_AND_NAME) \
    ",$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,i))

#define PSQL_UPDATE( r, data, i, TYPE_AND_NAME) \
    ", " BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, 2, TYPE_AND_NAME)) "=$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,i))


//replace BOOST_PP_ADD(1,num) with const static
#define DECLEAR_PSQL_TABLE( clazz, table, member_seq ) \
template<class ISql>\
class clazz\
{\
public:\
typedef typename ISql::Result result_type;\
typedef boost::shared_ptr<clazz > ptr_type;\
typedef std::vector<ptr_type > vec_type;\
void assemble(result_type result){\
    id.toField(result.getFieldValOfRow("id",0));\
    BOOST_PP_SEQ_FOR_EACH( TABLE_ASSIGN, 0, member_seq)\
    result.freeAll();\
}\
static void assembleVector(result_type result, vec_type& vec){\
    int rowSize = result.getRowSize();printf("rowSize:%d\n",rowSize);\
    for(int row=0; row<rowSize; row++)\
    {\
        ptr_type clz(new clazz);\
        clz->id.toField(result.getFieldValOfRow("id", row));\
        BOOST_PP_SEQ_FOR_EACH( TABLE_ROW_ASSIGN, row, member_seq)\
        vec.push_back(clz);\
    }\
}\
void insert();\
void update();\
void selectById(int64_t);\
static void deleteById(int64_t);\
public:\
    Field<int64_t> id;\
    BOOST_PP_SEQ_FOR_EACH( TABLE_MEMBER, , member_seq)\
    const static int param_num = 1 BOOST_PP_SEQ_FOR_EACH( TABLE_MEMBER_COUNT, , member_seq); \
    const static int param_less = param_num-1; \
};\
template <> inline void clazz<PSql>::update()\
{\
    const char* sql = "update " #table " set id=id " BOOST_PP_SEQ_FOR_EACH_I( PSQL_UPDATE, , member_seq) " where id=$" BOOST_PP_STRINGIZE( 1 );\
    const char *val[param_num];\
    LOG_INFO<<BOOST_PP_TUPLE_SIZE(member_seq);\
    int         len[param_num];\
    int         format[param_num];\
    \
    val[param_less] = id.getNetValue();\
    len[param_less] = id.getSizeof();\
    format[param_less] = 1;\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::query(sql, param_num, val, len, format, 1);\
}\
template <> inline void clazz<PSql>::insert(){\
    const char* sql = "insert into " #table " values (nextval('" #table "_id_seq')" BOOST_PP_SEQ_FOR_EACH_I( PSQL_INSERT, , member_seq) ");";\
    const char *val[param_num];\
    int         len[param_num];\
    int         format[param_num];\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::Result result = PSql::query(sql, param_less, val, len, format, 1);\
}\
template <> inline void clazz<PSql>::selectById(int64_t argId)\
{\
    const char* sql = "select * from " #table " where id=$1";\
    PSql::Result result = PSql::selectById(sql, argId);\
    id.toField(result.getFieldValOfRow("id", 0));\
    BOOST_PP_SEQ_FOR_EACH( TABLE_ASSIGN, 0, member_seq)\
    result.freeAll();\
}\
template <> inline void clazz<PSql>::deleteById(int64_t argId)\
{\
    const char* sql = "delete from " #table " where id=$1";\
    PSql::Result result = PSql::queryById(sql, argId);\
}\

#endif // ORM_PSQL_H_INCLUDED
