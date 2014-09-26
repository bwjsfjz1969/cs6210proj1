#ifndef GTTHREAD_H
#define GTTHREAD_H

#include "steque.h"
#include <ucontext.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


/* Define gtthread_t and gtthread_mutex_t types here */
typedef unsigned long gtthread_t;

struct gtthread {
//	int running;
	int complete;
	int exited;
	int cancel;
	int joinedThreadNum;
	gtthread_t treadID;
	ucontext_t treadContext;
	gtthread_t parentID; /* Parent Thread ID */
	void *retval;
} *current, *next, *prev;

struct gtthread_mutex_t {
    long lock;
    gtthread_t owner;
};


/*
  'period' is the scheduling quantum (interval)
  in microseconds (i.e., 1/1000000 sec.).
*/
void gtthread_init(long period);
int  gtthread_create(gtthread_t *thread, void *(*start_routine)(void *), void *arg);
int  gtthread_join(gtthread_t thread, void **status);
void gtthread_exit(void *retval);
void gtthread_yield(void);
int  gtthread_equal(gtthread_t t1, gtthread_t t2);
int  gtthread_cancel(gtthread_t thread);
gtthread_t gtthread_self(void);


int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);
int  gtthread_mutex_destroy(gtthread_mutex_t *mutex);
#endif
