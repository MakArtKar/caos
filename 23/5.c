#include <pthread.h>

struct FairRwLock
{
  unsigned int readers;
  unsigned int writers;
  unsigned int locked;
  unsigned int unlocked;

  pthread_mutex_t mutex;
  pthread_cond_t readCondVar;
  pthread_cond_t writeCondVar;
};

#define FAIR_RWLOCK_INITIALIZER \
    { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, \
    PTHREAD_COND_INITIALIZER, 0, 0, 0, 0 }

typedef struct FairRwLock fair_rwlock_t;

void fair_rwlock_init(fair_rwlock_t * ptr) {
  ptr->readers = 0;
  ptr->writers = 0;
  ptr->locked = 0;
  ptr->unlocked = 0;

  pthread_mutex_init(&ptr->mutex, NULL);

  pthread_cond_init(&ptr->readCondVar, NULL);
  pthread_cond_init(&ptr->writeCondVar, NULL);
}

void fair_rwlock_destroy(fair_rwlock_t * ptr) {
  pthread_mutex_destroy(&ptr->mutex);

  pthread_cond_destroy(&ptr->readCondVar);
  pthread_cond_destroy(&ptr->writeCondVar);
}

void fair_rwlock_wrlock(fair_rwlock_t *prw) {
  pthread_mutex_lock(&prw->mutex);

  ++prw->locked;

  while ((prw->readers || prw->writers) && ) {
    pthread_cond_wait(&prw->writeCondVar, &prw->mutex);
  }

  ++prw->writers;

  pthread_mutex_unlock(&prw->mutex);
}

void fair_rwlock_rdlock(fair_rwlock_t *prw) {
  pthread_mutex_lock(&prw->mutex);

  ++prw->locked;

  while (prw->w_wait || prw->) {
    pthread_cond_wait(&prw->readCondVar, &prw->mutex);
  }

  ++prw->readers;

  pthread_mutex_unlock(&prw->mutex);
}

void fair_rwlock_unlock(fair_rwlock_t *prw) {
  pthread_mutex_lock(&prw->mutex);
  ++prw->unlocked;
  if (prw->readers) {
    --prw->readers;
    if (!prw->readers) {
      pthread_cond_signal(&prw->writeCondVar);
    }

  } else if (prw->writers) {
    --prw->writers;
    pthread_cond_signal(&prw->writeCondVar);
    pthread_cond_broadcast(&prw->readCondVar);
  }
  pthread_mutex_unlock(&prw->mutex);
}