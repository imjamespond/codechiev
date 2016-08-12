#ifndef SQLMANAGER_H_INCLUDED
#define SQLMANAGER_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <base/Mutex.hpp>
#include <base/Logger.hpp>
#include <vector>

template <class DB_T, int Number>
class DBManager
{
public:
    typedef typename boost::shared_ptr<DB_T > db_ptr;
    typedef std::vector<db_ptr > db_vec;

    DBManager():index_(0)
    {}

    void
    init(const std::string& conninfo)
    {
        connInfo_ = conninfo;

        for(int i=0; i<Number; i++)
        {
            codechiev::base::MutexGuard lock(&mutex_);
            db_ptr db(new DB_T);
            db->connect(conninfo.c_str());
            dbs_.push_back(db);
        }
    }

    void
    close()
    {
        codechiev::base::MutexGuard lock(&mutex_);
        for(typename db_vec::const_iterator it = dbs_.begin();
            it != dbs_.end();
            it++)
        {
            *it->close();
        }
    }

    db_ptr
    getDB()
    {
        codechiev::base::MutexGuard lock(&mutex_);
        for(typename db_vec::const_iterator it = dbs_.begin();
            it != dbs_.end();
            it++)
        {
            const db_ptr& db = *it;
            if(db&&!db->isInUse())
            {
                db->setUsed(true);
                return db;
            }
        }
        return db_ptr();
    }

    void
    returnDB(db_ptr& pg)
    {
        codechiev::base::MutexGuard lock(&mutex_);
        if(pg)
        {
            pg->setUsed(false);
        }
    }

protected:
private:
    std::string *connInfo_;
    unsigned int index_;
    codechiev::base::Mutex mutex_;
    db_vec dbs_;
};

#endif // SQLMANAGER_H_INCLUDED
