#ifndef __GTTHREAD_MUTEX_H
#define __GTTHREAD_MUTEX_H

/****************************************************************
*  Define Mutex
*  Value to store whether mutex in use
****************************************************************/
typedef struct{
    //int volatile this;
    int this;
} gtthread_mutex_t;


/****************************************************************
*  Initializes mutex
*  Return 0 on initialize is successful
*  Return 1 if mutex doesn't exists
****************************************************************/
int  gtthread_mutex_init(gtthread_mutex_t *mutex);

/****************************************************************
*  Locks mutex
*  Return 0 if lock is successful
****************************************************************/
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);

/****************************************************************
*  Unlocks Mutex
*  Return 0 if unlock is successful
****************************************************************/
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);

/****************************************************************
*  destory Mutex
****************************************************************/
int gtthread_mutex_destroy(gtthread_mutex_t *mutex){

#endif 
