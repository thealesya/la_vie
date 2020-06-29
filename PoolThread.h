#pragma once
#include "ThQueue.h"
#include "AJob.h"
#include <cstring>
#include <chrono>
#include <thread>
#include <windows.h>

class PoolThread
{
    private:
        int PoolThreadCount;
        ThQueue JQueue;

        void ThreadStart(int id_th);
        void ResizePool(int psize);

    public:
        PoolThread(int psize);
        PoolThread();
        virtual ~PoolThread();
        void Start();
        void SetPoolSize(int psize);
        void SetMaxQueueSize(int qsize);

        string GetJobStatus(int id);
        int GetJobStatusId(int id);
        AJob* SyncSubmitJob(AJob *j);
        int ASyncSubmitJob(AJob *j);
        AJob* GetResJob(int id);
        void PrintQueue(){this->JQueue.PrintQueue();};
};
