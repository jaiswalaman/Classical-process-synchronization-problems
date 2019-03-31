# Classical-process-synchronization-problems
CSN-232 Assignment-7
Commands for executing the programs:-
1.gcc filename.c -pthread
2. ./a.out
All the programs can be executed in two ways by using the in built semaphore library present in c language or use an custom semaphore header file created in.
Both files can be included by using following code:-
  1. #include<semaphore.h>
  2. #include"SemaphoreCustom.h"  
comment the one that need not to be used , by default Custom one is being used by default.
  the custom semaphore library ensures no busy waiting by suspending any process waiting in while loop and add it to a queue with first in first out mechanism.Dequeue any element as one of signal is triggered.mutual exclusion is ensured through semaphore atomic variable.For suspending any threads we have used pthreads conditional variable.mutex locks are also needed for the while of wait signal.
