#ifndef SQLMANAGER_H_INCLUDED
#define SQLMANAGER_H_INCLUDED

#include <boost/shared_ptr.hpp>

template <class DB_T, int Number>
class DBManager
{
public:
    typedef typename boost::shared_ptr<DB_T > db_ptr;

    DBManager():index_(0)
    {}

    void
    init(const char* conninfo)
    {
        connInfo_ = conninfo;

        for(int i=0; i<Number; i++)
        {
            dbs_[i].reset(new DB_T());
            dbs_[i]->connect(connInfo_);
        }
    }
    
    void
    close()
    {
        for(int i=0; i<Number; i++)
        {
            dbs_[i]->close();
        }
    }

    db_ptr
    getDB()
    {
        db_ptr db;

        for(int i=index_; i<Number; i++)
        {
            //codechiev::base::MutexGuard lock(mutex_);
            index_=(++index_)%Number;
            db = dbs_[index_];
            if(db&&!db->isInUse())
            {
                db->setUsed(true);
                break;
            }
        }

        if(db)
        {
            db->connect(connInfo_);
        }
        return db;
    }

    void
    returnDB(db_ptr& pg)
    {
        //codechiev::base::MutexGuard lock(mutex_);
        if(pg)
        {
            pg->setUsed(false);
        }
    }

protected:
private:
    const char *connInfo_;
    unsigned int index_;
    //codechiev::base::Mutex mutex_;
    db_ptr dbs_[Number];
};

#endif // SQLMANAGER_H_INCLUDED
