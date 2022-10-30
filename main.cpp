#include<iostream>
#include<thread>
#include<mutex>
#include<list>

#define PR_DEBUG

#ifdef PR_DEBUG 
#define LOG(x) std::cout << x 
#define NEWLINE std::cout << std::endl
#define QUANTITY 10
#else
#define LOG(x) 
#define NEWLINE
#define QUANTITY 10000
#endif



struct queue_item 
{
    int value;
    int priority;
};

int* queue_init()
{ 
    return (int*)(new std::list<queue_item>);  
}


void queue_push(int* pqueue, int v, int p)
{
    std::list<queue_item> * queue = ((std::list<queue_item>*)pqueue);
    queue->push_back({v, p});
}

bool queue_isEmpty(int* pqueue)
{
    std::list<queue_item> * queue = ((std::list<queue_item>*)pqueue);
    if(queue->size() == 0)
    {
        return true;
    }
    return false;
}


queue_item queue_take(int* pqueue)
{
    if(queue_isEmpty(pqueue))
    {
        throw std::invalid_argument("queue is empty");
    }
   
    std::list<queue_item> * queue = ((std::list<queue_item>*)pqueue);
    int higest_priority = queue->back().priority;

    //take max priotity 
    for(auto el : *(queue))
    {
        if(el.priority > higest_priority)
        {
            higest_priority = el.priority;
        }
    }

    queue_item value_to_return = queue_item();
    for(std::list<queue_item>::iterator it = queue->begin(); it!= queue->end(); it++)
    {
        if(it->priority == higest_priority){
            value_to_return = *it;
            queue->erase(it);            
            break;
        }
    }

    return value_to_return;
}


std::mutex writerMutex;
std::mutex readerMutex;
std::mutex logger;

void writer(int* pq, int number){
    logger.lock();
    std::cout << "Thread " << number << " start writing\n";
    logger.unlock();

    for(int i = 0; i < QUANTITY; i++)
    {
        writerMutex.lock();
        queue_push(pq, std::rand() % 1000, std::rand() % 1000);
        writerMutex.unlock();
    }

    logger.lock();
    std::cout << "Thead " << number << " over writing.\n";
    logger.unlock();

}

void reader(int* pq, int number){
    logger.lock();
    std::cout << "Thread " << number << " start reading\n";
    logger.unlock();
    queue_item buffer;
    for(int i = 0; i < QUANTITY; i++)
    {  
        readerMutex.lock();
        buffer = queue_take(pq);
        LOG(buffer.value);
        LOG(" | ");
        LOG(buffer.priority);
        NEWLINE;
        readerMutex.unlock();
    }

    logger.lock();
    std::cout << "Thread " << number << " over reading.\n";
    logger.unlock();

}


int main(){
    int* p_queue = queue_init();
    std::thread writers[10];
    
    //create 10 thread that will write in queue
    for(int i = 0; i < 10; i++){
        writers[i] = std::thread(writer, p_queue, i);
    }

    //start 10 thread in join mode 
    for(int i = 0; i < 10; i++){
        writers[i].join();
    }
    std::cout << "10 threads over writing." << std::endl;

    //create 10 reader thread 
    std::thread readers[10];
    for(int i = 0; i < 10; i++)
    {
        readers[i] = std::thread(reader, p_queue, i);
    }

    //start 10 thread to read
    for(int i = 0; i < 10; i++)
    {
        readers[i].join();
    }

    std::cout << "10 threads over reading." << std::endl;


    return 0;
}
