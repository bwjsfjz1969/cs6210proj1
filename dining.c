#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

typedef enum  {left, right} direction_t;

pthread_mutex_t chopstick_mutex[5];

int phil_to_chopstick(int phil_id, direction_t d){
  return (phil_id + d) % 5;      
}

int chopstick_to_phil(int stick_id, direction_t d){
  return (stick_id + 5 - d) % 5;      
}

void pickup_one_chopstick(int stick_id, int phil_id){
  /*
    Use the line
    printf("Philosopher %d picks up chopstick %d \n", phil_id, stick_id);
    to have the philosopher pick-up a chopstick. Debugging will be easier
    if you remember to flush stdout.
    fflush(stdout)
  */
  /*Your code here*/
  printf("Philosopher %d picks up chopstick %d \n", phil_id, stick_id);
  fflush(stdout);
}

void putdown_one_chopstick(int stick_id, int phil_id){
  /*
    Use the line
    printf("Philosopher %d puts down chopstick %d \n", phil_id, stick_id);
    to have the philosopher pick-up a chopstick. Debugging will be easier
    if you remember to flush stdout.
    fflush(stdout)
  */
  /*Your code here*/
  printf("Philosopher %d puts down chopstick %d \n", phil_id, stick_id);
  fflush(stdout);
}

void pickup_chopsticks(int phil_id){
  /*Use     
    pickup_chopstick(phil_to_chopstick(phil_id, right), phil_id);
    and
    pickup_chopstick(phil_to_chopstick(phil_id, left), phil_id);
        to pickup the right and left chopsticks.  Beware of deadlock!
  */
  /*Your code here*/
  int leftChopstickUp=phil_to_chopstick(phil_id, left);
  int rightChopstickUp=phil_to_chopstick(phil_id, right);
  /************************************************************************
  * Odd philosophers picks up left chopstick first                        *
  * Even philosophers picks up right chopstick first                      *
  ************************************************************************/
  if (phil_id % 2 == 1) {
    // philosopher wants to eat lock both chopsticks print message
    pthread_mutex_lock(&chopstick_mutex[leftChopstickUp]);
    pthread_mutex_lock(&chopstick_mutex[rightChopstickUp]);
    pickup_one_chopstick(leftChopstickUp, phil_id);
    pickup_one_chopstick(rightChopstickUp, phil_id);
  } else {
    // philosopher wants to eat lock both chopsticks print message
    pthread_mutex_lock(&chopstick_mutex[rightChopstickUp]);
    pthread_mutex_lock(&chopstick_mutex[leftChopstickUp]);
    pickup_one_chopstick(rightChopstickUp, phil_id);
    pickup_one_chopstick(leftChopstickUp, phil_id);
  }
}

void putdown_chopsticks(int phil_id){
  /*Use putdown_chopstick to put down the chopsticks*/    
  /*Your code here*/
  /************************************************************************
  * Odd philosophers puts down right chopstick first                      *
  * even philosophers puts down left chopstick first                      *
  ************************************************************************/
  int leftChopstickDown=phil_to_chopstick(phil_id, left);
  int rightChopstickDown=phil_to_chopstick(phil_id, right);

  if (phil_id % 2 == 1) {
    // philosopher finished eating unlock both chopsticks print message
    putdown_one_chopstick(rightChopstickDown, phil_id);
    putdown_one_chopstick(leftChopstickDown, phil_id);
    pthread_mutex_unlock(&chopstick_mutex[rightChopstickDown]);
    pthread_mutex_unlock(&chopstick_mutex[leftChopstickDown]);
  } else {
    // philosopher finished eating unlock both chopsticks print message
    putdown_one_chopstick(leftChopstickDown, phil_id);
    putdown_one_chopstick(rightChopstickDown, phil_id);
    pthread_mutex_unlock(&chopstick_mutex[leftChopstickDown]);
    pthread_mutex_unlock(&chopstick_mutex[rightChopstickDown]);
  }
}

