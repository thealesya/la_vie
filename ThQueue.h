#pragma once
#include "AJob.h"
#include <list>
#include <mutex>
#include <iostream>
#include <tuple>
//Статус задачи
//1 - задача в очереди
//2 - задача выполняется
//3 - задача выполнена


using namespace std;

struct EQueue
{
    int id = 0;
    int idthread = 0;
    AJob *job = nullptr;
};

class ThQueue
{
    private:
        
        int max_queue_size; // Ограничение на размер очереди
        std::list <EQueue> PendingJobsList; // Список заданий
		std::list <EQueue> RunningJobsList;
		std::list <EQueue> CompletedJobsList;

        int id_count = 0; // Счетчик заданий

    public:
        std::mutex mtx;
        ThQueue();
        virtual ~ThQueue();

        //Служебные функции класса
		bool CheckAddJob(); // Проверяет можно ли добавить еще одно задание
        int GetNewId(); //Получить новый идентификатор
        std::tuple<std::list<EQueue>::iterator, bool>  find_id(int id, std::list <EQueue>& TList);

        //Пользовательские функции класса
		void SetMaxQueueSize(int qsize); // Устанавливает ограничение на размер очереди
		int AddJobM(AJob *j); // Добавляет задачу в очередь и вернуть ее id (если задача в очередь не поставлена, то вернуть -1)
		AJob* GetJobResById(int id); // Получить выполненную задачу. Если пользователь запросил не выполненную задачу, то вернуть nullprt
		bool DelJobM(int id); //Удалить задание из очереди
		int GetJobStatus(int id);
		void PrintQueue();

        //Worker
		std::tuple<EQueue, bool> GetJobM(int pid);// Получить задачу, если вернулось nullptr, то заданий нет
		void ReturnJob(EQueue j) ;//Возвращаем выполненное задание в очередь
};
