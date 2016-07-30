//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//
#include <base/Singleton.h>
#include <db/orm-psql.h>
#include <db/Psql.h>
#include <vector>
#include <boost/shared_ptr.hpp>
namespace codechiev{
    namespace db{
        //DECLEAR_TABLE( Student, student, ((int,id,id))((std::string,name,name))((std::string,sex,name)))
        DECLEAR_PSQL_TABLE( User, users, 3, ((std::string,uname,username))((int,gender,gender))\
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
        }
    }
}
using namespace codechiev::base;
using namespace codechiev::db;
int main(int argc, const char * argv[])
{
    Singleton<PSqlManager1>::get()->init("host=localhost \
                                               port=5432 \
                                               dbname=codechiev \
                                               user=postgres \
                                               password= \
                                               connect_timeout=3");
    UserEx user;
    user.selectById(10093);
    return 0;
}
