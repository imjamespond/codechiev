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
void find();\
\
public:\
    Field<int64_t> id;\
    BOOST_PP_SEQ_FOR_EACH( TABLE_MEMBER, , member_seq)\
    const static int param_size = BOOST_PP_SEQ_SIZE(member_seq); \
    const static int param_num = 1 + param_size; \
};\
template <> void clazz<PSql>::find()\
{\
const char* sql = "SELECT * FROM " #table " WHERE id = $1::int4";\
PSql::selectById(sql, id.getValue());\
}

#endif // ORM_PSQL_H_INCLUDED
