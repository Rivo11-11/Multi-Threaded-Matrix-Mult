#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10000 


typedef struct {
    int* arr;
    int left;
    int right;
} ThreadData;

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0; 
    j = 0; 
    k = left; 

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void* merge_sort(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int left = data->left;
    int right = data->right;
    int* arr = data->arr;

    if (left < right) {
        int mid = left + (right - left) / 2;

        ThreadData left_data = { arr, left, mid };
        ThreadData right_data = { arr, mid + 1, right };

        pthread_t left_thread, right_thread;

        // Create threads to sort the two halves
        pthread_create(&left_thread, NULL, merge_sort, &left_data);
        pthread_create(&right_thread, NULL, merge_sort, &right_data);

        // Wait for both threads to finish
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
}

int main() {
    FILE* file = fopen("input", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);
    int arr[MAX];
    for (int i = 0; i < n; i++) {
        fscanf(file, "%d", &arr[i]); 
    }
    fclose(file);

    ThreadData data = { arr, 0, n - 1 };
    pthread_t initial_thread;

    pthread_create(&initial_thread, NULL, merge_sort, &data);
    pthread_join(initial_thread, NULL);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
