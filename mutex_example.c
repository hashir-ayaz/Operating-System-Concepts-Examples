#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Shared resource
int resource = 0;

// Mutex
pthread_mutex_t mutex;

// Function executed by threads
void* access_resource(void* thread_id) {
    int id = *(int*)thread_id;

    printf("Thread %d is waiting to access the resource...\n", id);

    // Lock the mutex
    pthread_mutex_lock(&mutex);
    printf("Thread %d has locked the resource.\n", id);

    // Critical section
    resource++;
    printf("Thread %d updated the resource. Resource value: %d\n", id, resource);
    sleep(1); // Simulate some work

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    printf("Thread %d has unlocked the resource.\n", id);

    return NULL;
}

int main() {
    const int THREAD_COUNT = 3;
    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, access_resource, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
