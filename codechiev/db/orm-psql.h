#ifndef ORM_PSQL_H_INCLUDED
#define ORM_PSQL_H_INCLUDED

#include "orm.h"

#define DECLEAR_PSQL_TABLE( clazz, table, num, member_seq ) \
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
};\
template <> inline void clazz<PSql>::update()\
{\
    const char* sql = "update " #table " set id=id " BOOST_PP_SEQ_FOR_EACH_I( PSQL_UPDATE, , member_seq) " where id=$" BOOST_PP_STRINGIZE(BOOST_PP_ADD(1,num));\
    const char *val[BOOST_PP_ADD(1,num)];\
    int len[BOOST_PP_ADD(1,num)];\
    int format[BOOST_PP_ADD(1,num)];\
    \
    val[num] = id.getNetValue();\
    len[num] = id.getSizeof();\
    format[num] = 1;\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::query(sql, BOOST_PP_ADD(1,num), val, len, format, 1);\
}\
template <> inline void clazz<PSql>::insert(){\
    const char* sql = "insert into " #table " values (nextval('" #table "_id_seq')" BOOST_PP_SEQ_FOR_EACH_I( PSQL_PLACE_HOLDER, , member_seq) ");";\
    const char *val[num];\
    int len[num];\
    int format[num];\
    BOOST_PP_SEQ_FOR_EACH_I( PSQL_ASSIGN, , member_seq)\
    PSql::Result result = PSql::query(sql, num, val, len, format, 1);\
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
