////1-3 scheduling class(���α���)
//
//
//
//
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define TIME_SLICE 10
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
//    char* name;
//    int size;
//    int exec_time;
//    int time_slice;
//    double progress;
//    STATE state;
//    int class; 
//} PROC;
//
//typedef struct list_node {
//    PROC* proc;
//    struct list_node* next;
//} NODE;
//
//void print_rq(NODE* rq_head) {
//    printf("[");
//    while (rq_head != NULL) {
//        PROC* p = rq_head->proc;
//        if (p->state != TERMINATED) {
//            printf("%s (%d%%, Class%d)", p->name, (int)(p->progress * 100), p->class);
//        }
//        else {
//            printf("%s (Completed, Class%d)", p->name, p->class);
//        }
//        rq_head = rq_head->next;
//        if (rq_head != NULL) printf(", ");
//    }
//    printf("]");
//}
//
//void insert_sorted_rq(NODE** rq_head, PROC* proc) {
//    NODE* new_node = (NODE*)malloc(sizeof(NODE));
//    new_node->proc = proc;
//    new_node->next = NULL;
//
//    NODE* current = *rq_head;
//    NODE* previous = NULL;
//
//    
//    while (current != NULL && (proc->class > current->proc->class || (proc->class == current->proc->class && proc->progress > current->proc->progress))) {
//        previous = current;
//        current = current->next;
//    }
//
//    if (previous == NULL) {
//        new_node->next = *rq_head;
//        *rq_head = new_node;
//    }
//    else {
//        previous->next = new_node;
//        new_node->next = current;
//    }
//}
//
//PROC* next_proc(NODE** rq_head) {
//    if (*rq_head == NULL) return NULL;
//
//    NODE* temp = *rq_head;
//    *rq_head = (*rq_head)->next;
//
//    PROC* selected = temp->proc;
//    free(temp);
//
//    return selected;
//}
//
//int compare_size(const void* a, const void* b) {
//    PROC* procA = (PROC*)a;
//    PROC* procB = (PROC*)b;
//    return procB->size - procA->size; 
//}
//
//PROC* init(int* proc_count) {
//    FILE* file = fopen("workload.txt", "r");
//    if (file == NULL) {
//        perror("Failed to open file");
//        return NULL;
//    }
//
//    fscanf(file, "%d\n", proc_count);
//    PROC* proc_arr = (PROC*)malloc(sizeof(PROC) * (*proc_count));
//
//    for (int i = 0; i < *proc_count; i++) {
//        proc_arr[i].name = malloc(100 * sizeof(char));
//        fscanf(file, "%s %d\n", proc_arr[i].name, &proc_arr[i].size);
//        proc_arr[i].exec_time = 0;
//        proc_arr[i].time_slice = TIME_SLICE;
//        proc_arr[i].state = READY;
//        proc_arr[i].progress = 0.0;
//    }
//
//    fclose(file);
//
//  
//    qsort(proc_arr, *proc_count, sizeof(PROC), compare_size);
//    int threshold = (int)(*proc_count * 0.3); 
//
//    for (int i = 0; i < *proc_count; i++) {
//        proc_arr[i].class = (i < threshold) ? 1 : 2; 
//    }
//
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
//int sched(PROC** running, NODE** rq_head, int total_procs, PROC* proc_arr) {
//    static char terminated_proc_name[100] = "";  // ����� ���μ��� �̸� ����
//    static PROC* last_running = NULL;  // ������ ���� ���̴� ���μ��� ����
//    int proc_terminated = 0;  // ���μ����� �̹� �����ٸ� ����Ŭ���� ����Ǿ����� ����
//
//    if (*running) {
//        (*running)->exec_time += 1;
//        (*running)->time_slice -= 1;
//        update_progress(*running);  // ����� ������Ʈ
//
//        if ((*running)->exec_time >= (*running)->size) {
//            (*running)->state = TERMINATED;
//            (*running)->progress = 1.0;
//            proc_terminated = 1;
//            snprintf(terminated_proc_name, sizeof(terminated_proc_name), "%s", (*running)->name);
//            *running = NULL;
//        }
//        else if ((*running)->time_slice <= 0) {
//            (*running)->time_slice = TIME_SLICE;
//            (*running)->state = READY;
//            insert_sorted_rq(rq_head, *running);
//            *running = NULL;
//        }
//    }
//
//    if (*running == NULL) {
//        *running = next_proc(rq_head);
//        if (*running) {
//            (*running)->state = RUNNING;
//            (*running)->time_slice = TIME_SLICE;
//            if (*running != last_running || proc_terminated) {
//                printf("%d ms: run %s, ", current_time, (*running)->name);
//                print_rq(*rq_head);
//                if (proc_terminated) {
//                    printf(", terminated: %s", terminated_proc_name);
//                }
//                printf("\n");
//                last_running = *running;
//            }
//        }
//    }
//
//    // ��� ���μ����� ����Ǿ����� �˻�
//    for (int i = 0; i < total_procs; i++) {
//        if (proc_arr[i].state != TERMINATED) {
//            current_time += 1; // ���� �����ؾ� �� ���μ����� ���� ����
//            return 1; // ��� ����
//        }
//    }
//
//    // ��� ���μ����� ����� ���, ���������� ����� ���μ����� ������ ���
//    if (proc_terminated) {
//        printf("%d ms: end of execution, terminated: %s\n", current_time, terminated_proc_name);
//    }
//    else {
//        printf("%d ms: end of execution\n", current_time);
//    }
//
//    return 0; // ����
//}
//
//void cleanup(PROC* proc_arr, int proc_count, NODE* rq_head) {
//    for (int i = 0; i < proc_count; i++) {
//        free(proc_arr[i].name); // ���μ��� �̸��� �Ҵ�� �޸� ����
//    }
//    free(proc_arr); // ���μ��� �迭�� �Ҵ�� �޸� ����
//
//    // ���� ť�� ���� �ִ� ��� �޸� ����
//    while (rq_head != NULL) {
//        NODE* temp = rq_head;
//        rq_head = rq_head->next;
//        free(temp);
//    }
//}
//
//int main() {
//    NODE* rq_head = NULL;
//    PROC* running = NULL;
//    int proc_count = 0;
//
//    PROC* proc_arr = init(&proc_count);
//
//    for (int i = 0; i < proc_count; i++) {
//        insert_sorted_rq(&rq_head, &proc_arr[i]);
//    }
//
//    printf("init: ");
//    print_rq(rq_head);
//    printf("\n");
//
//    while (sched(&running, &rq_head, proc_count, proc_arr));
//
//    cleanup(proc_arr, proc_count, rq_head);
//
//    return 0;
//}