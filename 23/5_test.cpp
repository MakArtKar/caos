#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "func.c"

fair_rwlock_t locker = FAIR_RWLOCK_INITIALIZER;

void * reader(void * ptr) {
  printf("%u new reader\n", pthread_self());
  fflush(stdout);
  fair_rwlock_rdlock(&locker);
  printf("%u reader entered\n", pthread_self);
  fflush(stdout);
  sleep(5);
  fair_rwlock_unlock(&locker);
  fair_rwlock_destroy(&locker);
  printf("%u reader unlocked\n", pthread_self());
  fflush(stdout);
  return NULL;
}

void * writer(void * ptr) {
  printf("%u new writer\n", pthread_self());
  fflush(stdout);
  fair_rwlock_wrlock(&locker);
  printf("%u writer entered\n", pthread_self());
  fflush(stdout);
  sleep(5);
  fair_rwlock_unlock(&locker);
  fair_rwlock_destroy(&locker);
  printf("%u writer unlocked\n", pthread_self());
  fflush(stdout);
  return NULL;
}

int main() {
  pthread_t ids[5];
  pthread_create(&ids[0], NULL, reader, NULL);
  pthread_create(&ids[1], NULL, reader, NULL);
  pthread_create(&ids[2], NULL, writer, NULL);
  pthread_create(&ids[3], NULL, writer, NULL);
  pthread_create(&ids[4], NULL, reader, NULL);

  pthread_join(ids[0], NULL);
  pthread_join(ids[1], NULL);
  pthread_join(ids[2], NULL);
  pthread_join(ids[3], NULL);
  pthread_join(ids[4], NULL);
}