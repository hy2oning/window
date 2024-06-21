////1-2 우선순위 스케줄러(전부구현)
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
//int current_time = 0; // 현재 시간 추적을 위한 전역 변수
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
//            printf("%s (%d%%)", p->name, (int)(p->progress * 100));
//        }
//        else {
//            printf("%s (Completed)", p->name);
//        }
//        rq_head = rq_head->next;
//        if (rq_head != NULL) printf(", ");
//    }
//    printf("]");
//}
//
//
//void insert_rq(NODE** rq_head, PROC* proc) {
//    NODE* new_node = (NODE*)malloc(sizeof(NODE));
//    new_node->proc = proc;
//    new_node->next = NULL;
//
//    if (*rq_head == NULL) {
//        *rq_head = new_node;
//    }
//    else {
//        NODE* temp = *rq_head;
//        while (temp->next != NULL) {
//            temp = temp->next;
//        }
//        temp->next = new_node;
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
//        char name[100];
//        int size;
//        fscanf(file, "%s %d\n", name, &size);
//
//        proc_arr[i].name = _strdup(name);
//        proc_arr[i].size = size;
//        proc_arr[i].exec_time = 0;
//        proc_arr[i].time_slice = TIME_SLICE;
//        proc_arr[i].state = READY;
//        proc_arr[i].progress = 0.0; // 초기 진행률을 0%로 설정
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
//void insert_sorted_rq(NODE** rq_head, PROC* proc) {
//    NODE* new_node = (NODE*)malloc(sizeof(NODE));
//    new_node->proc = proc;
//    new_node->next = NULL;
//
//    if (*rq_head == NULL) {
//        *rq_head = new_node;
//    }
//    else {
//        NODE* current = *rq_head;
//        NODE* previous = NULL;
//
//        // 낮은 progress (낮은 진행률 = 높은 우선순위)를 우선적으로 배치
//        while (current != NULL && proc->progress > current->proc->progress) {
//            previous = current;
//            current = current->next;
//        }
//
//        // 진행률이 같을 경우, 먼저 도착한 순서로 유지 (FIFO)
//        while (current != NULL && proc->progress == current->proc->progress) {
//            previous = current;
//            current = current->next;
//        }
//
//        if (previous == NULL) { // 리스트의 시작 부분에 삽입
//            new_node->next = *rq_head;
//            *rq_head = new_node;
//        }
//        else { // 중간 또는 끝 부분에 삽입
//            previous->next = new_node;
//            new_node->next = current;
//        }
//    }
//}
//
//// 스케줄링을 수행하는 함수
//int sched(PROC** running, NODE** rq_head, int total_procs, PROC* proc_arr) {
//    static char terminated_proc_name[100] = "";  // 종료된 프로세스 이름 저장
//    static PROC* last_running = NULL;  // 이전에 실행 중이던 프로세스 추적
//    int proc_terminated = 0;  // 프로세스가 이번 스케줄링 사이클에서 종료되었는지 여부
//
//    if (*running) {
//        (*running)->exec_time += 1;
//        (*running)->time_slice -= 1;
//        update_progress(*running);  // 진행률 업데이트
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
//            insert_sorted_rq(rq_head, *running);  // 수정된 삽입 함수 사용
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
//    // 모든 프로세스가 종료되었는지 검사
//    for (int i = 0; i < total_procs; i++) {
//        if (proc_arr[i].state != TERMINATED) {
//            current_time += 1; // 아직 실행해야 할 프로세스가 남아 있음
//            return 1; // 계속 실행
//        }
//    }
//
//    // 모든 프로세스가 종료된 경우, 마지막으로 종료된 프로세스가 있으면 출력
//    if (proc_terminated) {
//        printf("%d ms: end of execution, terminated: %s\n", current_time, terminated_proc_name);
//    }
//    else {
//        printf("%d ms: end of execution\n", current_time);
//    }
//
//    return 0; // 종료
//}
//
//
//void cleanup(PROC* proc_arr, int proc_count, NODE* rq_head) {
//    for (int i = 0; i < proc_count; i++) {
//        free(proc_arr[i].name); // 프로세스 이름에 할당된 메모리 해제
//    }
//    free(proc_arr); // 프로세스 배열에 할당된 메모리 해제
//
//    // 레디 큐에 남아 있는 노드 메모리 해제
//    while (rq_head != NULL) {
//        NODE* temp = rq_head;
//        rq_head = rq_head->next;
//        free(temp);
//    }
//}
//
//
//
//int main() {
//    NODE* rq_head = NULL;
//    PROC* running = NULL;
//    int proc_count = 0;
//
//    PROC* proc_arr = init(&proc_count);
//
//    for (int i = 0; i < proc_count; i++) {
//        insert_rq(&rq_head, &proc_arr[i]);
//    }
//
//    printf("init: ");
//    print_rq(rq_head);
//    printf("\n");
//
//
//
//    while (sched(&running, &rq_head, proc_count, proc_arr));
//
//    cleanup(proc_arr, proc_count, rq_head);
//
//    return 0;
//}