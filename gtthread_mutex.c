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

/*
  The gtthread_mutex_init() function is analogous to
  pthread_mutex_init with the default parameters enforced.
  There is no need to create a static initializer analogous to
  PTHREAD_MUTEX_INITIALIZER.
*/
int gtthread_mutex_init(gtthread_mutex_t* mutex){
  /* 
    if the mutex isn't sent error out
  */
  if (mutex == NULL)
    return 1;
  /* 
    since this is an init release lock if set and set to 0 to initialize
  */
  __sync_lock_release(&mutex->this);

  return 0;
}

/*
  The gtthread_mutex_lock() is analogous to pthread_mutex_lock.
  Returns zero on success.
*/
int gtthread_mutex_lock(gtthread_mutex_t* mutex){
  /* 
    Attempt to change value of mutex to 1 since it was 0 perviously
  */
  while (__sync_lock_test_and_set(&mutex->this, 1)==1){
    // Change context
    raise(SIGVTALRM);
  }

  return 0;
}

/*
  The gtthread_mutex_unlock() is analogous to pthread_mutex_unlock.
  Returns zero on success.
*/
int gtthread_mutex_unlock(gtthread_mutex_t *mutex){
  /* 
    release the lock and set to 0
  */
  __sync_lock_release(&mutex->this);

  return 0;
}

/*
  The gtthread_mutex_destroy() function is analogous to
  pthread_mutex_destroy and frees any resourcs associated with the mutex.
*/
int gtthread_mutex_destroy(gtthread_mutex_t *mutex){

}
