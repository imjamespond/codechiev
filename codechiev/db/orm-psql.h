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
void selectById();\
void insert();\
void update();\
void assemble(result_type result);\
static void DeleteById(int64_t);\
static void assembleVector(result_type result, vec_type& vec);\
\
public:\
    Field<int64_t> id;\
    BOOST_PP_SEQ_FOR_EACH( TABLE_MEMBER, , member_seq)\
    const static int param_size = BOOST_PP_SEQ_SIZE(member_seq); \
    const static int param_num = 1 + param_size; \
};\
\
\
template <> void clazz<PSql>::selectById()\
{\
    const char* sql = "SELECT * FROM " #table " WHERE id = $1::bigint";\
    PSql::Result result;\
    PSql::selectById(result, sql, id.getValue());\
    BOOST_PP_SEQ_FOR_EACH( TABLE_ASSIGN, 0, member_seq) \
    result.freeAll();\
}\
template <> void clazz<PSql>::insert(){\
    const char* sql = "insert into " #table " values (nextval('" #table "_id_seq')" BOOST_PP_SEQ_FOR_EACH_I( PSQL_INSERT, , member_seq) ") returning id";\
    const char *val[param_size];\
    int         len[param_size];\
    int         format[param_size];\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::Result result;\
    PSql::select(result, sql, param_size, val, len, format, 1);\
    id.toField(result.getFieldValOfRow("id", 0));\
    result.freeAll();\
}\
template <> void clazz<PSql>::update()\
{\
    const char* sql = "update " #table " set id=id " BOOST_PP_SEQ_FOR_EACH_I( PSQL_UPDATE, , member_seq) " where id=$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,BOOST_PP_SEQ_SIZE(member_seq))) "::bigint";\
    const char *val[param_num];\
    int         len[param_num];\
    int         format[param_num];\
    \
    val[param_size] = id.getNetValue();\
    len[param_size] = id.getSizeof();\
    format[param_size] = 1;\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::Result result;\
    PSql::query(result, sql, param_num, val, len, format, 1);\
    result.freeAll();\
}\
template <> void clazz<PSql>::DeleteById(int64_t argId)\
{\
    const char* sql = "delete from " #table " where id=$1::bigint";\
    PSql::Result result;\
    PSql::queryById(result, sql, argId);\
    result.freeAll();\
}\
template <> void clazz<PSql>::assemble(result_type result)\
{\
    id.toField(result.getFieldValOfRow("id",0));\
    BOOST_PP_SEQ_FOR_EACH( TABLE_ASSIGN, 0, member_seq)\
    result.freeAll();\
}\
template <> void clazz<PSql>::assembleVector(result_type result, vec_type& vec)\
{\
    int rowSize = result.getRowSize();\
    for(int row=0; row<rowSize; row++)\
    {\
        ptr_type clz(new clazz);\
        clz->id.toField(result.getFieldValOfRow("id", row));\
        BOOST_PP_SEQ_FOR_EACH( TABLE_ROW_ASSIGN, row, member_seq)\
        vec.push_back(clz);\
    }\
    result.freeAll();\
}\

#endif // ORM_PSQL_H_INCLUDED
