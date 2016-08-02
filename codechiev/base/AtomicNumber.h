#ifndef ATOMICNUMBER_H_INCLUDED
#define ATOMICNUMBER_H_INCLUDED

namespace codechiev
{
    namespace base
    {
        /*
int8_t / uint8_t
int16_t / uint16_t
int32_t / uint32_t
int64_t / uint64_t
        */
        template <typename T>
        class AtomicNumber
        {
        public:
            explicit AtomicNumber(T num):num_(num){}

            T fetchAdd(T num)
            {
                return __sync_fetch_and_add(&num_, num);
            }

            T addFetch(T num)
            {
                return __sync_add_and_fetch(&num_, num);
            }


        private:
            T num_;
        };
    }
}

#endif // ATOMICNUMBER_H_INCLUDED
