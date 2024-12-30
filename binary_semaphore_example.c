#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Define a binary semaphore
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} binary_semaphore_t;

// Initialize the semaphore
void binary_semaphore_init(binary_semaphore_t* sem, int initial_value) {
    sem->value = initial_value; // Must be 0 or 1 for binary semaphore
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

// Wait (P operation)
void binary_semaphore_wait(binary_semaphore_t* sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) { // Wait until the semaphore value is 1
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value = 0; // Take the resource
    pthread_mutex_unlock(&sem->mutex);
}

// Signal (V operation)
void binary_semaphore_signal(binary_semaphore_t* sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value = 1; // Release the resource
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

// Destroy the semaphore
void binary_semaphore_destroy(binary_semaphore_t* sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
}

// Shared resource
int resource = 0;
binary_semaphore_t binary_semaphore;

// Function executed by threads
void* access_resource(void* thread_id) {
    int id = *(int*)thread_id;

    printf("Thread %d is waiting for the resource...\n", id);
    binary_semaphore_wait(&binary_semaphore); // Wait for the semaphore

    // Critical section
    printf("Thread %d is accessing the resource.\n", id);
    resource++;
    printf("Resource value updated by Thread %d: %d\n", id, resource);
    sleep(1); // Simulate some work with the resource

    printf("Thread %d is releasing the resource.\n", id);
    binary_semaphore_signal(&binary_semaphore); // Signal the semaphore

    return NULL;
}

int main() {
    const int THREAD_COUNT = 3;
    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];

    // Initialize the binary semaphore with an initial value of 1
    binary_semaphore_init(&binary_semaphore, 1);

    // Create threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, access_resource, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    binary_semaphore_destroy(&binary_semaphore);

    return 0;
}
