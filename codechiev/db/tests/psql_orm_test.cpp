//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//
#include <base/Singleton.h>
#include <base/Logger.hpp>
#include <db/orm-psql.h>
#include <db/Psql.h>
#include <vector>
#include <boost/shared_ptr.hpp>
namespace codechiev{
    namespace db{
        /*
         create database codechiev;
         CREATE TABLE users
         (
         id bigint NOT NULL, -- primary key
         username character varying,
         gender integer,
         createdate bigint,
         CONSTRAINT pk_users PRIMARY KEY (id )
         );
         CREATE SEQUENCE users_id_seq
         INCREMENT 1
         MINVALUE 1
         MAXVALUE 9223372036854775807
         START 1
         CACHE 1;
         */
        DECLEAR_PSQL_TABLE( User, users, ((std::string,uname,username))((int,gender,gender))\
                           ((int64_t,createdate,createdate)))
        DECLEAR_PSQL_TABLE( Test, test1, ((std::string,t,t))((std::string,b,b)) )

        class UserEx : public User<PSql>
        {
        public:
            static void SelectByName(UserEx& user, const std::string &name );
            static void SelectListByGender(vec_type&, int32_t );
        };

        void
        UserEx::SelectByName(UserEx& user, const std::string &name)
        {
            const char *sql = "SELECT * FROM users WHERE username = $1";
            const char *paramValues[1];
            paramValues[0] = name.c_str();

            PSql::Result result;
            PSql::select(result, sql, 1, paramValues, NULL, NULL, 1);
            //user.assemble(result);
            result.freeAll();
        }

        void
        UserEx::SelectListByGender(vec_type& userVec, int32_t gender )
        {
            const char *sql = "SELECT * FROM users WHERE gender = $1";
            const char *paramValues[1];
            int paramLengths[1];
            int paramFormats[1];
            uint32_t val = net::hostToNetworkInt32(gender);
            paramValues[0] = (char *) &val;
            paramLengths[0] = sizeof(val);
            paramFormats[0] = 1;

            PSql::Result result;
            PSql::select(result, sql, 1, paramValues, paramLengths, paramFormats, 1);
            UserEx::assembleVector(result, userVec);
        }
    }
}
using namespace codechiev::base;
using namespace codechiev::db;
int main(int argc, const char * argv[])
{
    const char* conninfo = "host=127.0.0.1 \
    port=5432 \
    dbname=codechiev \
    user=postgres \
    password= \
    connect_timeout=3";
    PSqlManager* manager = Singleton<PSqlManager>::get();
    manager->init(conninfo);
    //when using localhost it will get this error by valgrind:
    //Invalid free() / delete / delete[] / realloc()

    UserEx user;
    user.id.setValue(1);
    user.selectById();
    LOG_INFO<<"user:"<<user.uname.getValue()<<", gender:"<<user.gender.getValue();

    user.uname.setValue("foobar") ;
    user.gender.setValue(123456);
    user.createdate.setValue(9876543210);
    user.insert();
    LOG_INFO<<"id:" << user.id.getValue()<< ",user:"<<user.uname.getValue()<<", gender:"<<user.gender.getValue();

    user.uname.setValue("codechiev");
    user.gender.setValue(1);
    user.update();
    UserEx::DeleteById(user.id.getValue()-1);

    LOG_INFO<<"\n\n\nUserEx::SelectByName";
    UserEx userFooBar;
    UserEx::SelectByName(userFooBar, "foobar");
    LOG_INFO<<"id:" << userFooBar.id.getValue()<< ",user:"<<userFooBar.uname.getValue()<<", gender:"<<userFooBar.gender.getValue();

    /*LOG_INFO<<"\n\n\nUserEx::SelectListByGender";
    UserEx::vec_type users;
    UserEx::SelectListByGender( users, 1);
    for(UserEx::vec_type::const_iterator it=users.begin();
        it!=users.end();
        it++)
    {
        const UserEx::ptr_type& u = (*it);
        LOG_INFO<<"id:" << u->id.getValue()<< ",user:"<<u->uname.getValue()<<", gender:"<<u->gender.getValue();
    }

    Test<PSql> test;
    test.id.setValue(1);
    test.selectById();
    LOG_INFO<<"id:"<<test.id.getValue();
    LOG_INFO<<"text:"<<test.t.getValue();
    LOG_INFO<<"bytes:"<<test.b.getValue();*/

    manager->close();
    return 0;
}

