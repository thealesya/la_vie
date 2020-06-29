#include "ThQueue.h"
using namespace std;

ThQueue::ThQueue()
{
    this->max_queue_size = 10;
}
/////////////////////////////////////////////
ThQueue::~ThQueue()
{
    
}
/////////////////////////////////////////////
bool ThQueue::CheckAddJob() {
    int qsize = this->PendingJobsList.size() + \
                this->RunningJobsList.size() + \
                this->CompletedJobsList.size();
    if (qsize< this->max_queue_size) return true;
    else return false;
}
/////////////////////////////////////////////
int ThQueue::GetNewId(){
    this->id_count++;
    return this->id_count;
}
/////////////////////////////////////////////
void ThQueue::SetMaxQueueSize(int qsize) {
    this->max_queue_size = qsize;
}
/////////////////////////////////////////////
int ThQueue::AddJobM(AJob *j) {
	mtx.lock();
    if (CheckAddJob()) {
        EQueue NewJob;
        NewJob.id = GetNewId();
        NewJob.job = j;
        this->PendingJobsList.push_back(NewJob);
		mtx.unlock();
        return NewJob.id;
	}
	else {
		mtx.unlock(); return -1;
	}
}
/////////////////////////////////////////////
std::tuple<std::list<EQueue>::iterator, bool> ThQueue::find_id(int id, std::list <EQueue>& TList) {
    for (std::list<EQueue>::iterator it=TList.begin(); it != TList.end(); ++it){
        EQueue tmp = *it;
        if (tmp.id == id)
            return std::make_tuple(it, true);
    }
    std::list<EQueue>::iterator it;
    return std::make_tuple(it, false);
}
/////////////////////////////////////////////
AJob* ThQueue::GetJobResById(int id) {
	mtx.lock();
    auto t_tmp = find_id(id, this->CompletedJobsList);
    if (std::get<1>(t_tmp)) {
        std::list<EQueue>::iterator it = std::get<0>(t_tmp);
        EQueue tmp = *it;
		mtx.unlock();
        return tmp.job;
    }
	mtx.unlock();
    return nullptr;
}
/////////////////////////////////////////////
bool ThQueue::DelJobM(int id) {
    
	mtx.lock();
    auto t_tmp = find_id(id, this->PendingJobsList);
    if (std::get<1>(t_tmp)) {
        std::list<EQueue>::iterator it = std::get<0>(t_tmp);
        this->PendingJobsList.erase(it);
		mtx.unlock();
        return true;
    }
    t_tmp = find_id(id, this->CompletedJobsList);
    if (std::get<1>(t_tmp)) {
        std::list<EQueue>::iterator it = std::get<0>(t_tmp);
        this->CompletedJobsList.erase(it);
		mtx.unlock();
        return true;
    }
	mtx.unlock();
    return false;
}
/////////////////////////////////////////////
int ThQueue::GetJobStatus(int id) {
    auto t_tmp = find_id(id, this->PendingJobsList);
    if (std::get<1>(t_tmp)) {
        return 1;
    }
    t_tmp = find_id(id, this->RunningJobsList);
    if (std::get<1>(t_tmp)) {
        return 2;
    }
    t_tmp = find_id(id, this->CompletedJobsList);
    if (std::get<1>(t_tmp)) {
        return 3;
    }
    return -1;
}
/////////////////////////////////////////////
std::tuple<EQueue, bool> ThQueue::GetJobM(int pid) {
    mtx.lock();
    if (!this->PendingJobsList.empty()) {
        EQueue tmp = this->PendingJobsList.front();
        tmp.idthread = pid;
        this->PendingJobsList.pop_front();
        this->RunningJobsList.push_back(tmp);
        mtx.unlock();
        return std::make_tuple(tmp, true);
    } else {
        mtx.unlock();
        EQueue tmp;
        return std::make_tuple(tmp, false);
    }
}
/////////////////////////////////////////////
void ThQueue::ReturnJob(EQueue j) {
    mtx.lock();
    auto t_tmp = find_id(j.id, this->RunningJobsList);
    if (std::get<1>(t_tmp)) {
        this->RunningJobsList.erase(std::get<0>(t_tmp));
        this->CompletedJobsList.push_back(j);
    }
    mtx.unlock();
}
/////////////////////////////////////////////
void ThQueue::PrintQueue(){
    mtx.lock();
    for (EQueue n : this->PendingJobsList)
        std::cout << n.id << " " << n.idthread <<" Pending" << std::endl;
    for (EQueue n : this->RunningJobsList)
        std::cout << n.id << " " << n.idthread << " Running" << std::endl;
    for (EQueue n : this->CompletedJobsList)
        std::cout << n.id << " " << n.idthread << " Complete" << std::endl;
    mtx.unlock();
}
