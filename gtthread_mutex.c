/**********************************************************************
gtthread_mutex.c.  

This file contains the implementation of the mutex subset of the
gtthreads library.  The locks can be implemented with a simple queue.
**********************************************************************/

/*
  Include as needed
*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "gtthread.h"

init mutexID = 0;


/*
  The gtthread_mutex_init() function is analogous to
  pthread_mutex_init with the default parameters enforced.
  There is no need to create a static initializer analogous to
  PTHREAD_MUTEX_INITIALIZER.
*/
int gtthread_mutex_init(gtthread_mutex_t *mutex){
  if (mutex->lock == 1) {
    return -1;
  }
  mutex->lock = 0;
  mutex->owner = -1;
  return 0;
}

/*
  The gtthread_mutex_lock() is analogous to pthread_mutex_lock.
  Returns zero on success.
*/
int gtthread_mutex_lock(gtthread_mutex_t * mutex){
//  while(mutexTracker[*mutex] != 1) {
//    raise(SIGVTALRM);
//  }
//  mutexTracker[*mutex] = current->data;
//  return 0;
  if ((mutex->owner) == current->threadID) {
    return -1;
  }

  while (mutex->lock != 0 && mutex->owner != current->threadID) gtthread_yield();

  mutex->lock = 1;
  mutex->owner = current->threadID;
  return 0;
}

/*
  The gtthread_mutex_unlock() is analogous to pthread_mutex_unlock.
  Returns zero on success.
*/
int gtthread_mutex_unlock(gtthread_mutex_t * mutex){
//  if(mutexTracker[*mutex] == current->data) {
//    mutexTracker[*mutex] = 1;
//    return 0;
//  } else {
//    return 1;	// Mutex unlock error
//  }
  if (mutex->lock == 1 && mutex->owner == current->threadID) { 
    mutex->lock = 0;
    mutex->owner = -1;
    return 0;
    }
  return -1;
}


/*
  The gtthread_mutex_destroy() function is analogous to
  pthread_mutex_destroy and frees any resourcs associated with the mutex.
*/
int gtthread_mutex_destroy(gtthread_mutex_t *mutex){

}
