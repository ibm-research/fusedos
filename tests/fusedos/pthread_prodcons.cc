/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/* IBM Confidential                                                 */
/*                                                                  */
/* Licensed Machine Code Source Materials                           */
/*                                                                  */
/* Product(s):                                                      */
/*     Blue Gene/Q Licensed Machine Code                            */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011, 2012                        */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <pthread.h>
#include <iostream>
#include <cstring>
#include <cstdio>

#define NUM_ENTRIES 20
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2

#define PRODUCTION_PER_THREAD 12345

static const int debug = 0;

class ProducerConsumerBuffer
{
public:
    ProducerConsumerBuffer(int entries) : _entries(entries+1), head(1), tail(0), _active(true) {
        buffer = new int[_entries];

        int rc = pthread_mutex_init( &lock, NULL );
        if(rc)
            std::cerr << "Error creating mutex lock for ProducerConsumerBuffer: " << strerror(rc) << std::endl;

        rc = pthread_cond_init( &cv_empty, NULL );
        if(rc)
            std::cerr << "Error creating mutex lock for ProducerConsumerBuffer: " << strerror(rc) << std::endl;
        rc = pthread_cond_init( &cv_full, NULL );
        if(rc)
            std::cerr << "Error creating mutex lock for ProducerConsumerBuffer: " << strerror(rc) << std::endl;
    }

    int consume();
    void produce(int v);

    void stop();

    bool isActive() const {
        return _active;
    }
    bool isEmpty() const {
        return (tail+1) % _entries == head;
    }

private:
    const int _entries;
    int * buffer;
    int head, tail;

    pthread_mutex_t lock;
    pthread_cond_t  cv_empty;
    pthread_cond_t  cv_full;
    bool _active;
};

int ProducerConsumerBuffer::consume()
{
    int product;
    pthread_mutex_lock( &lock );

    // empty ?
    while( isEmpty() && isActive() )
        pthread_cond_wait( &cv_empty, &lock );

    if(!isActive() && isEmpty()) {
        pthread_mutex_unlock( &lock );
        return 0;
    }

    tail = (tail + 1) % _entries;

    product = buffer[tail];

    pthread_mutex_unlock( &lock );

    pthread_cond_signal( &cv_full );
    return product;
}

void ProducerConsumerBuffer::produce(int v)
{

    pthread_mutex_lock( &lock );

    // full ?
    while( head == tail && isActive() )
        pthread_cond_wait( & cv_full, &lock );

    if(!isActive()) {
        pthread_mutex_unlock( &lock );
        return;
    }

    buffer[head] = v;

    head = (head + 1) % _entries;

    pthread_mutex_unlock( &lock );

    pthread_cond_signal( & cv_empty );
}

void ProducerConsumerBuffer::stop()
{
    _active = false;
    pthread_cond_broadcast( &cv_empty );
    pthread_cond_broadcast( &cv_full );
}

extern "C" void * worker(void * data);

class WorkerThread
{
public:
    WorkerThread(ProducerConsumerBuffer * buffer) : _buffer(buffer) {};
    int run();
    int join();

    virtual void doWork(ProducerConsumerBuffer * buffer) = 0;
    void callWork();

private:
    ProducerConsumerBuffer * _buffer;
    pthread_t thread;

};

class ProducerThread : public WorkerThread
{
public:
    ProducerThread(ProducerConsumerBuffer * buffer, int threadNo)
        : WorkerThread(buffer), _produced(0), _threadNo(threadNo) {};
    virtual void doWork(ProducerConsumerBuffer * buffer);
    long getProductionSum() const {
        return _produced;
    }
private:
    long _produced;
    int _threadNo;
};

class ConsumerThread : public WorkerThread
{
public:
    ConsumerThread(ProducerConsumerBuffer * buffer, int threadNo)
        : WorkerThread(buffer), _consumed(0), _threadNo(threadNo) {};
    virtual void doWork(ProducerConsumerBuffer * buffer);
    long getConsumptionSum() const {
        return _consumed;
    }
private:
    long _consumed;
    int _threadNo;
};


int WorkerThread::run()
{
    return pthread_create( &thread, NULL, worker, this);
}

int WorkerThread::join()
{
    return pthread_join( thread, NULL );
}

void WorkerThread::callWork()
{
    doWork( _buffer );
}

extern "C" void * worker(void * data)
{
    //ProducerConsumerBuffer *buf = (ProducerConsumerBuffer *) data;
    WorkerThread * t = (WorkerThread *)data;
    t->callWork();
    return NULL;
}

void ProducerThread::doWork(ProducerConsumerBuffer * buffer)
{

    int foo = 13*42*(_threadNo + 1);

	if(debug) printf("producer %2d will produce %d items\n",
			_threadNo, PRODUCTION_PER_THREAD);

    for(int i=0; i < PRODUCTION_PER_THREAD; i++) {
        int product = foo;
        if(debug) printf("prod %2d producing %d\n", _threadNo, product);
        buffer->produce(product);
        _produced += product;

        foo = (foo*7+3)%137;
    }

    if(debug) printf("producer %2d finished\n", _threadNo);
}

void ConsumerThread::doWork(ProducerConsumerBuffer * buffer)
{

    while( buffer->isActive() || !buffer->isEmpty() ) {
        int product = buffer->consume();
        _consumed += product;
        if(debug) printf("consumer %2d consumed %d\n", _threadNo, product);
    }
    if(debug) printf("consumer %2d finished.\n", _threadNo);
}


int main(int argc, char ** argv)
{

    ProducerConsumerBuffer buffer(NUM_ENTRIES);

    ProducerThread *pt[NUM_PRODUCERS];
    ConsumerThread *ct[NUM_CONSUMERS];

    // create producer and consumer objects
    for(int i=0; i < NUM_PRODUCERS; i++)
        pt[i] = new ProducerThread( &buffer, i );

    for(int i=0; i < NUM_CONSUMERS; i++)
        ct[i] = new ConsumerThread( &buffer, i );

    // start producer and consumer threads
    for(int i=0; i < NUM_PRODUCERS; i++)
        pt[i]->run();

    for(int i=0; i < NUM_CONSUMERS; i++)
        ct[i]->run();

    // join producer and consumer threads
    for(int i=0; i < NUM_PRODUCERS; i++)
        pt[i]->join();

    // signal stop to consumers via buffer
    buffer.stop();

    for(int i=0; i < NUM_CONSUMERS; i++)
        ct[i]->join();

    long produced = 0, consumed = 0;

    for(int i=0; i < NUM_PRODUCERS; i++) {
        printf("producer thread %2d produced %ld\n", i, pt[i]->getProductionSum() );
        produced += pt[i]->getProductionSum();
    }

    for(int i=0; i < NUM_CONSUMERS; i++) {
        printf("consumer thread %2d consumed %ld\n", i, ct[i]->getConsumptionSum() );
        consumed += ct[i]->getConsumptionSum();
    }

    if( consumed == produced )
        printf("sum of %ld consumed matches sum of %ld produced, fine.\n", consumed, produced);
    else
        printf("sum of %ld consumed does not match sum of %ld produced, very bad!\n", consumed, produced);

}
