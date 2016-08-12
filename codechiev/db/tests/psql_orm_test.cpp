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
            void selectByName(const std::string& name);
            static void SelectListByGender(int32_t , vec_type&);
        };

        void
        UserEx::selectByName(const std::string &name)
        {
            const char *sql = "SELECT * FROM user2 WHERE username = $1";
            const char *paramValues[1];
            int paramLengths[1];
            int paramFormats[1];
            paramValues[0] = name.c_str();
            paramLengths[0] = static_cast<int>(name.length());
            paramFormats[0] = 1;
            
            PSql::Result result;
            PSql::select(result, sql, 1, paramValues, paramLengths, paramFormats, 1);
            this->assemble(result);
        }

        void
        UserEx::SelectListByGender(int32_t gender, vec_type& userVec)
        {
            const char *sql = "SELECT * FROM user2 WHERE gender = $1";
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

    User<PSql> user;
    user.id.setValue(1);
    user.selectById();
    LOG_INFO<<"user:"<<user.uname.getValue()<<", gender:"<<user.gender.getValue();

    user.uname.setValue("foobar") ;
    user.gender.setValue(123456);
    user.createdate.setValue(9876543210);
    user.insert();
    LOG_INFO<<"id:" << user.id.getValue()<< ",user:"<<user.uname.getValue()<<", gender:"<<user.gender.getValue();
    
    user.gender.setValue(1);
    user.update();
    //User<PSql>::DeleteById(user.id.getValue()-1);
    
    LOG_INFO<<"UserEx::SelectListByGender";
    UserEx::vec_type users;
    UserEx::SelectListByGender(1, users);
    for(UserEx::vec_type::const_iterator it=users.begin();
        it!=users.end();
        it++)
    {
        const UserEx::ptr_type& u = (*it);
        LOG_INFO<<"id:" << u->id.getValue()<< ",user:"<<u->uname.getValue()<<", gender:"<<u->gender.getValue();
    }

    /*Test<PSql> test;
    test.id.setValue(1);
    test.selectById();
    LOG_INFO<<"id:"<<test.id.getValue();
    LOG_INFO<<"text:"<<test.t.getValue();
    LOG_INFO<<"bytes:"<<test.b.getValue();*/
    
    manager->close();
    return 0;
}

