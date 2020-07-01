#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <mutex>
#include <memory>

namespace DesignPattern
{
    template<typename DataType, int instanceId = 0>
    class Singleton 
    {
    public: 
        static DataType* instance()
        {
            if (NULL == dataHolderM.get())
            {
                std::lock_guard<std::mutex> lock(dbLockMutexM);
                if (NULL == dataHolderM.get())
                {
                    dataHolderM.reset(new DataType);
                }
            }
            return dataHolderM.get();

        }
    private:
        Singleton(){};
        ~Singleton(){};

        static std::auto_ptr<DataType> dataHolderM;
        static std::mutex dbLockMutexM;
    };

    template<typename DataType, int instanceId>
    std::auto_ptr<DataType> Singleton<DataType, instanceId>::dataHolderM;

    template<typename DataType, int instanceId>
    std::mutex Singleton<DataType, instanceId>::dbLockMutexM;
	
	//DataType with init
    template<typename DataType, int instanceId = 0>
    class InitDataSingleton 
    {
    public: 
        static DataType* instance()
        {
            if (NULL == dataHolderM.get())
            {
                std::lock_guard<std::mutex> lock(dbLockMutexM);
                if (NULL == dataHolderM.get())
                {
					DataType* data = new DataType;
					data->init();
                    dataHolderM.reset(data);
                }
            }
            return dataHolderM.get();

        }
    private:
        InitDataSingleton(){};
        ~InitDataSingleton(){};

        static std::auto_ptr<DataType> dataHolderM;
        static std::mutex dbLockMutexM;
    };

    template<typename DataType, int instanceId>
    std::auto_ptr<DataType> InitDataSingleton<DataType, instanceId>::dataHolderM;

    template<typename DataType, int instanceId>
    std::mutex InitDataSingleton<DataType, instanceId>::dbLockMutexM;
}

#endif /* SINGLETON_HPP */

