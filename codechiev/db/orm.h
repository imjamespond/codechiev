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



#endif // ORM_H_INCLUDED
