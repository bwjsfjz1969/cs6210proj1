/**********************************************************************
gtthread_sched.c.  

This file contains the implementation of the scheduling subset of the
gtthreads library.  A simple round-robin queue should be used.
 **********************************************************************/
/*
  Include as needed
*/

#include "gtthread.h"

/* 
   Students should define global variables and helper functions as
   they see fit.
 */

#define MAXTHREADS 1000

/* helper routines*/
int getThreadID();
void initializeTimer(long period);
void queueThread (struct gtthread *newThread);
void switchContext(int sig);
void executeRoutine(void *(*start_routine)(void *),void *arg);
struct gtthread threadPool[MAXTHREADS];
gtthread_t usedThreadID[MAXTHREADS];

/* initialize the queue for scheduleling */
steque_t threadQueue;
steque_init(&threadQueue);

int gtthreadInit = 0; /* used to check if the gttthread has been initialized */

static ucontext_t nextContext; /* ucontext context for the next thread */

struct itimerval timer;
sigset_t signalSet;



int getThreadID()
{
  int i=0; 
  while(i<MAXTHREADS) {
    if(!usedThreadID[i]) { 
      usedThreadID[i]=1; 
      return i;
    }
    i++;
  }
  return 0;
}

void initializeTimer(long period){
	sigemptyset(&signalSet);
	sigaddset(&signalSet,SIGVTALRM);
	timer.it_value.tv_sec=0;
	timer.it_value.tv_usec=period;
	timer.it_interval.tv_sec=0;
	timer.it_interval.tv_usec=period;
	signal(SIGVTALRM,switchContext);
	setitimer(ITIMER_VIRTUAL,&timer,NULL); 
}

void queueThread (struct gtthread *newThread) {
  /* Block the calling thread */
  sigprocmask(SIG_BLOCK, &signalSet, NULL);
  if (steque_size(threadQueue) == 0) {
    next = newThread;
    /* add newThread threadID to back of queue */
    steque_enqueue(threadQueue, newThread->threadID);
  } else {
    next = steque_front(threadQueue);
    /* add newThread threadID to back of queue */
    steque_enqueue(threadQueue, newThread->threadID);
  }
  (threadPool + (tempThreadToJoin->parentID))->joinedThreadNum += 1;
  /* Unblock the calling thread */
  sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
}

void switchContext(int sig){
  /*change the signal mask of calling thread - block it */
  /* Bock the calling thread */
  sigprocmask(SIG_BLOCK, &signalSet, NULL);
  /* 
    check if the current process has not exited (exited==0)
    AND that there are threads which can be used for switching the
    current context to theirs.
  */
  if (current->exited==0 && steque_size(threadQueue) > 1 ){
    /* move a current threadID to back of queue */
    steque_cycle(threadQueue);
    next = steque_front(threadQueue);
  } else {
    current->exited=0;
    /* make next then current threadID */
    next = current
  }
  /* Reset timer - Give next thread a time-slice */
  initializeTimer(period);
  /* Unblock the calling thread */
  sigprocmask(SIG_UNBLOCK, &signalSet, NULL);	
  /* swap current context with next thread  */
  swapcontext(&current->threadContext,&next->threadContext);
  current = steque_front(threadQueue);
}

void executeRoutine(void *(*start_routine)(void *),void *arg){
  void *retval=start_routine(arg);
  gtthread_exit(retval);
}

/*
  The gtthread_init() function does not have a corresponding pthread equivalent.
  It must be called from the main thread before any other GTThreads
  functions are called. It allows the caller to specify the scheduling
  period (quantum in micro second), and may also perform any other
  necessary initialization.  If period is zero, then thread switching should
  occur only on calls to gtthread_yield().

  Recall that the initial thread of the program (i.e. the one running
  main() ) is a thread like any other. It should have a
  gtthread_t that clients can retrieve by calling gtthread_self()
  from the initial thread, and they should be able to specify it as an
  argument to other GTThreads functions. The only difference in the
  initial thread is how it behaves when it executes a return
  instruction. You can find details on this difference in the man page
  for pthread_create.
 */
void gtthread_init(long period){
  if (period != 0){

    memset(threadPool,0,sizeof(struct threadDS)*MAXTHREADS);
    memset(usedThreadID,0,sizeof(gtthread_t)*MAXTHREADS);
    memset(midInUse,0,sizeof(gtthread_mutex_t)*MAXTHREADS);

    int tempThreadID=getThreadID();

    struct gtthread *newThread = (threadPool + tempThreadID);
    newThread->threadID = tempThreadID
    newThread->complete = 0;
    newThread->cancel = 0;
    newThread->exited = 0;
    newthread->joinedThreadNum = 0;
    newthread->parentID = newThread->threadID;

    current = newThread
  
    queueThread(newthread);

    initializeTimer(period);
    return;
  } else {
    printf("--> Error - Invalid period.\n");
    exit(1);
  }
}


/*
  The gtthread_create() function mirrors the pthread_create() function,
  only default attributes are always assumed.
 */
int gtthread_create(gtthread_t *thread, void *(*start_routine)(void *), void *arg){
  if (gtthreadInit != 1) {
    printf("Error - GTThread not initialized\n");
    exit(-1);
  }

  int tempThreadID=getThreadID();

  struct gtthread *newThread = (threadPool + tempThreadID);
  newThread->threadID = tempThreadID
  newThread->complete = 0;
  newThread->cancel = 0;
  newThread->exited = 0;
  newthread->parentID = current->threadID;

  if (thread) {
    *thread=newThread->threadID
  };

  getcontext(&newThread->threadContext);
  newThread->threadContext.uc_link = 0
  newThread->tContext.uc_stack.ss_sp = malloc(SIGSTKSZ);
  newThread->threadContext.uc_stack.ss_size = SIGSTKSZ;
  newThread->threadContext.uc_stack.ss_flags = 0;

  makecontext(&newThread->threadContext, (void(*)(void)) executeRoutine, 2, start_routine, arg);

  queueThread(newthread);
}

/*
  The gtthread_join() function is analogous to pthread_join.
  All gtthreads are joinable.
 */
int gtthread_join(gtthread_t thread, void **status){
  struct gtthread *tempThreadToJoin = (threadPool + thread);

  (threadPool + (tempThreadToJoin->parentID))->joinedThreadNum += 1;

  threadPool[tempThreadtoJoin->pid].joinedThreadNum += 1;

  while(tempThreadtoJoin->complete!=1);
  
  if (*status) (
    *status = tempThreadtoJoin->retval;
  }
  
  return 0;
}

/*
  The gtthread_exit() function is analogous to pthread_exit.
 */
void gtthread_exit(void* retval){
  current->retval = retval;
  current->complete = 1;
  /* while there are sub-threads executing, wait */
  while (current->joinedThreadNum > 0);
  current->exited=1;

  if (steque_size(threadQueue) == 1) {
    /*if current == main*/
    exit(0);
  } 
  /* Block the calling thread */
  sigprocmask(SIG_BLOCK, &signalSet, NULL);
  steque_pop(threadQueue);
  next = steque_front(threadQueue);
  /* add newThread threadID to back of queue */
  steque_enqueue(threadQueue, newThread->threadID);
  /* Unblock the calling thread */
  sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
  /* swap current context with next thread  */
  swapcontext(&current->threadContext,&next->threadContext);
}

/*
  The gtthread_yield() function is analogous to pthread_yield, causing
  the calling thread to relinquish the cpu and place itself at the
  back of the schedule queue.
 */
void gtthread_yield(void){
  raise(SIGVTALRM);
}

/*
  The gtthread_equal() function is analogous to pthread_equal,
  returning zero if the threads are the same and non-zero otherwise.
 */
int  gtthread_equal(gtthread_t t1, gtthread_t t2){
  if (t1 == t2){
    return 1;
  } else {
    return 0;
  }
}

/*
  The gtthread_cancel() function is analogous to pthread_cancel,
  allowing one thread to terminate another asynchronously.
 */
int  gtthread_cancel(gtthread_t thread){
  struct gtthread *tempThreadToCancel = thread;
  tempThreadToCancel->ret=(void*)"CANCELED";
  tempThreadToCancel->complete=1;
	
  /* while there are sub-threads executing, wait */
  while(tempThreadToCancel->joinedThreadNum>0);

  /* Block the calling thread */
  sigprocmask(SIG_BLOCK, &signalSet, NULL);
  /* decrease join thread number and free mem */
  (threadPool+(tempThreadToCancel->pid))->joinedThreadNum -= 1;
  free(tempThreadToCancel->tContext.uc_stack.ss_sp);

  /* Unblock the calling thread */
  sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
  if (current ==tempThreadToCancel) {
    raise(SIGVTALRM);
  } else {
    return 0;
  }
}

/*
  Returns calling thread.
 */
gtthread_t gtthread_self(void){
  return current->threadID;
}
