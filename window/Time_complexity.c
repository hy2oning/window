//////1-4 O(logN)변환(미완성)
//
//
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define TIME_SLICE 10
//#define MAX_PROCESSES 100
//
//int current_time = 0;
//
//typedef enum process_state {
//    READY,
//    RUNNING,
//    TERMINATED
//} STATE;
//
//typedef struct process {
//    char name[50];
//    int size;
//    int exec_time;
//    int time_slice;
//    double progress;
//    STATE state;
//    int class;
//} PROC;
//
//typedef struct heap {
//    PROC* data[MAX_PROCESSES];
//    int size;
//} HEAP;
//
//void swap(PROC** a, PROC** b) {
//    PROC* temp = *a;
//    *a = *b;
//    *b = temp;
//}
//
//void heapify_up(HEAP* heap, int index) {
//    int parent = (index - 1) / 2;
//    if (index && (heap->data[parent]->class > heap->data[index]->class ||
//        (heap->data[parent]->class == heap->data[index]->class && strcmp(heap->data[parent]->name, heap->data[index]->name) > 0))) {
//        swap(&heap->data[parent], &heap->data[index]);
//        heapify_up(heap, parent);
//    }
//}
//
//void heapify_down(HEAP* heap, int index) {
//    int left = 2 * index + 1;
//    int right = 2 * index + 2;
//    int smallest = index;
//
//    if (left < heap->size && (heap->data[left]->class < heap->data[smallest]->class ||
//        (heap->data[left]->class == heap->data[smallest]->class && strcmp(heap->data[left]->name, heap->data[smallest]->name) < 0))) {
//        smallest = left;
//    }
//
//    if (right < heap->size && (heap->data[right]->class < heap->data[smallest]->class ||
//        (heap->data[right]->class == heap->data[smallest]->class && strcmp(heap->data[right]->name, heap->data[smallest]->name) < 0))) {
//        smallest = right;
//    }
//
//    if (smallest != index) {
//        swap(&heap->data[index], &heap->data[smallest]);
//        heapify_down(heap, smallest);
//    }
//}
//
//void insert_heap(HEAP* heap, PROC* proc) {
//    if (heap->size < MAX_PROCESSES) {
//        heap->data[heap->size] = proc;
//        heapify_up(heap, heap->size++);
//    }
//}
//
//PROC* extract_min(HEAP* heap) {
//    if (heap->size) {
//        PROC* proc = heap->data[0];
//        heap->data[0] = heap->data[--heap->size];
//        heapify_down(heap, 0);
//        return proc;
//    }
//    return NULL;
//}
//
//void print_heap(const HEAP* heap) {
//    printf("[");
//    for (int i = 0; i < heap->size; i++) {
//        printf("%s (%d%%, Class%d)", heap->data[i]->name, (int)(heap->data[i]->progress * 100), heap->data[i]->class);
//        if (i < heap->size - 1) printf(", ");
//    }
//    printf("]");
//}
//
//int compare_proc(const void* a, const void* b) {
//    return ((PROC*)b)->size - ((PROC*)a)->size;
//}
//
//PROC* init(int* proc_count) {
//    FILE* file = fopen("workload.txt", "r");
//    if (!file) {
//        perror("Failed to open file");
//        return NULL;
//    }
//
//    fscanf(file, "%d\n", proc_count);
//    PROC* proc_arr = (PROC*)malloc(sizeof(PROC) * (*proc_count));
//
//    for (int i = 0; i < *proc_count; i++) {
//        fscanf(file, "%s %d\n", proc_arr[i].name, &proc_arr[i].size);
//        proc_arr[i].exec_time = 0;
//        proc_arr[i].time_slice = TIME_SLICE;
//        proc_arr[i].progress = 0.0;
//        proc_arr[i].state = READY;
//    }
//
//   
//    qsort(proc_arr, *proc_count, sizeof(PROC), compare_proc);
//    for (int i = 0; i < *proc_count; i++) {
//        proc_arr[i].class = (i < *proc_count * 0.3) ? 1 : 2;  
//    }
//
//    fclose(file);
//    return proc_arr;
//}
//
//void update_progress(PROC* proc) {
//    if (proc->state != TERMINATED) {
//        proc->progress = (double)proc->exec_time / proc->size;
//        if (proc->progress >= 1.0) {
//            proc->progress = 1.0;
//            proc->state = TERMINATED;
//        }
//    }
//}
//
//void sched(PROC** running, HEAP* proc_heap) {
//    static char terminated_name[50] = "";
//    if (*running) {
//        (*running)->exec_time += 1;
//        (*running)->time_slice -= 1;
//        update_progress(*running);
//
//        if ((*running)->state == TERMINATED) {
//            strcpy(terminated_name, (*running)->name);
//            *running = NULL;
//        }
//        else if ((*running)->time_slice <= 0) {
//            (*running)->time_slice = TIME_SLICE;
//            (*running)->state = READY;
//            insert_heap(proc_heap, *running);
//            *running = NULL;
//        }
//    }
//
//    if (*running == NULL) {
//        *running = extract_min(proc_heap);
//        if (*running) {
//            (*running)->state = RUNNING;
//            printf("%d ms: run %s, ", current_time, (*running)->name);
//            print_heap(proc_heap);
//            if (strlen(terminated_name) > 0) {
//                printf(", terminated: %s", terminated_name);
//                strcpy(terminated_name, "");
//            }
//            printf("\n");
//        }
//    }
//}
//
//void cleanup(PROC* proc_arr, int proc_count) {
//    for (int i = 0; i < proc_count; i++) {
//        free(proc_arr[i].name);
//    }
//    free(proc_arr);
//}
//
//int main() {
//    HEAP proc_heap;
//    proc_heap.size = 0;
//    PROC* running = NULL;
//    int proc_count = 0;
//
//    PROC* proc_arr = init(&proc_count);
//    for (int i = 0; i < proc_count; i++) {
//        insert_heap(&proc_heap, &proc_arr[i]);
//    }
//
//    printf("init: ");
//    print_heap(&proc_heap);
//    printf("\n");
//
//    while (proc_heap.size > 0 || running != NULL) {
//        sched(&running, &proc_heap);
//        current_time++;
//    }
//
//    cleanup(proc_arr, proc_count);
//    printf("End of scheduling.\n");
//    return 0;
//}
