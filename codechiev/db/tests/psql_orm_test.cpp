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
         
        //DECLEAR_TABLE( Student, student, ((int,id,id))((std::string,name,name))((std::string,sex,name)))
        DECLEAR_PSQL_TABLE( User, users, ((std::string,uname,username))((int,gender,gender))\
                           ((int64_t,createdate,createdate)))

        class UserEx:public User<PSql>
        {
        public:
            void selectByName(const std::string& name);
            static void selectListByGender(int32_t , User<PSql>::vec_type&);
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

            PSql::Result result = PSql::select(sql, 1, paramValues, paramLengths, paramFormats, 1);
            this->assemble(result);
        }

        void
        UserEx::selectListByGender(int32_t gender, User<PSql>::vec_type& userVec)
        {
            const char *sql = "SELECT * FROM user2 WHERE gender = $1";
            const char *paramValues[1];
            int paramLengths[1];
            int paramFormats[1];
            uint32_t val = net::hostToNetworkInt32(gender);
            paramValues[0] = (char *) &val;
            paramLengths[0] = sizeof(val);
            paramFormats[0] = 1;

            PSql::Result result = PSql::select(sql, 1, paramValues, paramLengths, paramFormats, 1);
            User<PSql>::assembleVector(result, userVec);
        }*/
        
        DECLEAR_PSQL_TABLE( Test, test1, ((std::string,t,t))((std::string,b,b)) )
    }
}
using namespace codechiev::base;
using namespace codechiev::db;
int main(int argc, const char * argv[])
{
    PSqlManager1* manager = Singleton<PSqlManager1>::get();
    manager->init("host=127.0.0.1 \
                  port=5432 \
                  dbname=codechiev \
                  user=postgres \
                  password= \
                  connect_timeout=3");
    //when using localhost it will get this error by valgrind:
    //Invalid free() / delete / delete[] / realloc()

    /*UserEx user;
    user.selectById(10001);
    LOG_INFO<<"user:"<<user.uname.getValue()<<", gender:"<<user.gender.getValue();

    user.uname.setValue("foobar") ;
    user.gender.setValue(123456);
    user.createdate.setValue(9876543210);
    user.insert();
    //user.gender.setValue(user.gender.getValue()+1);
    //user.update();
    //UserEx::deleteById(10002);*/
    
    Test<PSql> test;
    test.find();
    
    manager->close();
    return 0;
}

