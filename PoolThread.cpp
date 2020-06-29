#include "PoolThread.h"

PoolThread::PoolThread(int psize)
{
    this->PoolThreadCount = psize;
}
//////////////////////////////////////////////
PoolThread::PoolThread()
{
    this->PoolThreadCount = 1;
}
//////////////////////////////////////////////
PoolThread::~PoolThread()
{
    this->PoolThreadCount = 0;
}
//////////////////////////////////////////////
void PoolThread::Start(){
    if (this->PoolThreadCount > 0) {
        for (int i = 1; i <= this->PoolThreadCount; i++){
            std::thread thread(&PoolThread::ThreadStart, this, i);
            thread.detach();
        }
    }
}
//////////////////////////////////////////////
void PoolThread::ThreadStart(int id_th){
    while (this->PoolThreadCount > 0) {
        auto t_tmp = this->JQueue.GetJobM(id_th);
        if (std::get<1>(t_tmp)){
            EQueue jtmp = std::get<0>(t_tmp);
            jtmp.job->ExecJob();
            this->JQueue.ReturnJob(jtmp);
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (id_th > this->PoolThreadCount) break;
    }
}

void PoolThread::ResizePool(int psize){
    if (this->PoolThreadCount > psize) {
            this->PoolThreadCount = psize;
        } else {
            for (int i = this->PoolThreadCount + 1; i <= psize; i++){
                std::thread thread(&PoolThread::ThreadStart, this, i);
                thread.detach();
            }
            this->PoolThreadCount = psize;
        }
}


void PoolThread::SetPoolSize(int psize){
    if ((psize >= 0) and (this->PoolThreadCount != psize))
        ResizePool(psize);
}

string PoolThread::GetJobStatus(int id) {
    switch(this->JQueue.GetJobStatus(id)){
        case 1:
            return "pending";
            break;
        case 2:
            return "running";
            break;
        case 3:
            return "complited";
            break;
    }
    return "No jobs " + id;
}
int PoolThread::GetJobStatusId(int id) {
    return this->JQueue.GetJobStatus(id);
}

void PoolThread::SetMaxQueueSize(int qsize) {
    this->JQueue.SetMaxQueueSize(qsize);
};
AJob* PoolThread::SyncSubmitJob(AJob *j){
    int tid = this->JQueue.AddJobM(j);
    while (this->JQueue.GetJobStatus(tid) != 3)
        Sleep(1000);
    return this->JQueue.GetJobResById(tid);
};
int PoolThread::ASyncSubmitJob(AJob *j){
    return this->JQueue.AddJobM(j);
};
AJob* PoolThread::GetResJob(int id){
    if (this->JQueue.GetJobStatus(id) == 3)
        return this->JQueue.GetJobResById(id);
    else return nullptr;
};


