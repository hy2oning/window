//1-1 Round Robin �����ٷ�(���α���)




#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_SLICE 10

int current_time = 0; // ���� �ð� ������ ���� ���� ����

typedef enum process_state {
    READY,
    RUNNING,
    TERMINATED
} STATE;

typedef struct process {
    char* name;
    int size;
    int exec_time;
    int time_slice;
    STATE state;
} PROC;

typedef struct list_node {
    PROC* proc;
    struct list_node* next;
} NODE;

void print_rq(NODE* rq_head) { // ���� ť ���¸� ����ϴ� �Լ�
    printf("[");
    while (rq_head != NULL) {
        printf("%s", rq_head->proc->name);
        rq_head = rq_head->next;
        if (rq_head != NULL) printf(", ");
    }
    printf("]");
}

void insert_rq(NODE** rq_head, PROC* proc) {
    NODE* new_node = (NODE*)malloc(sizeof(NODE));
    new_node->proc = proc;
    new_node->next = NULL;

    if (*rq_head == NULL) {
        *rq_head = new_node;
    }
    else {
        NODE* temp = *rq_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

PROC* next_proc(NODE** rq_head) {
    if (*rq_head == NULL) return NULL;

    NODE* temp = *rq_head;
    *rq_head = (*rq_head)->next;

    PROC* selected = temp->proc;
    free(temp);

    return selected;
}

PROC* init(int* proc_count) {
    FILE* file = fopen("workload.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    fscanf(file, "%d\n", proc_count);
    PROC* proc_arr = (PROC*)malloc(sizeof(PROC) * (*proc_count));

    for (int i = 0; i < *proc_count; i++) {
        char name[100];
        int size;
        fscanf(file, "%s %d\n", name, &size);

        proc_arr[i].name = _strdup(name);
        proc_arr[i].size = size;
        proc_arr[i].exec_time = 0;
        proc_arr[i].time_slice = TIME_SLICE;
        proc_arr[i].state = READY;
    }

    fclose(file);
    return proc_arr;
}

// �����ٸ��� �����ϴ� �Լ�
int sched(PROC** running, NODE** rq_head, int total_procs, PROC* proc_arr) {
    static char terminated_proc_name[100] = ""; // ����� ���μ��� �̸� ����
    static PROC* last_running = NULL; // ������ ���� ���̴� ���μ��� ����
    int proc_terminated = 0; // ���μ����� �̹� �����ٸ� ����Ŭ���� ����Ǿ����� ����

    if (*running) {
        (*running)->exec_time += 1;
        (*running)->time_slice -= 1;

        if ((*running)->exec_time >= (*running)->size) { // ���μ��� ���� �Ϸ�
            strcpy(terminated_proc_name, (*running)->name);
            (*running)->state = TERMINATED;
            proc_terminated = 1;
            *running = NULL;
        }
        else if ((*running)->time_slice <= 0) { // �ð� ���� ����
            (*running)->time_slice = TIME_SLICE; // �ð� ���� �缳��
            (*running)->state = READY;
            insert_rq(rq_head, *running); // ���� ť�� �ٽ� ����
            *running = NULL;
        }
    }

    if (*running == NULL) {
        *running = next_proc(rq_head);
        if (*running) {
            (*running)->state = RUNNING;
            (*running)->time_slice = TIME_SLICE; // �ð� ���� �缳��
            if (*running != last_running || proc_terminated) { // ���μ��� �����
                printf("%d ms: run %s, ", current_time, (*running)->name);
                print_rq(*rq_head);
                if (proc_terminated) {
                    printf(", terminated: %s\n", terminated_proc_name);
                }
                else {
                    printf("\n");
                }
                last_running = *running;
            }
        }
    }

    // ��� ���μ����� ����Ǿ����� �˻�
    for (int i = 0; i < total_procs; i++) {
        if (proc_arr[i].state != TERMINATED) {
            current_time += 1; // ���� �����ؾ� �� ���μ����� ���� ����
            return 1; // ��� ����
        }
    }

    // ��� ���μ����� ����� ���, ���������� ����� ���μ����� ������ ���
    if (proc_terminated) {
        printf("%d ms: end of execution, terminated: %s\n", current_time, terminated_proc_name);
    }
    else {
        printf("%d ms: end of execution\n", current_time);
    }

    return 0; // ����
}


void cleanup(PROC* proc_arr, int proc_count, NODE* rq_head) {
    for (int i = 0; i < proc_count; i++) {
        free(proc_arr[i].name); // ���μ��� �̸��� �Ҵ�� �޸� ����
    }
    free(proc_arr); // ���μ��� �迭�� �Ҵ�� �޸� ����

    // ���� ť�� ���� �ִ� ��� �޸� ����
    while (rq_head != NULL) {
        NODE* temp = rq_head;
        rq_head = rq_head->next;
        free(temp);
    }
}

int main() {
    NODE* rq_head = NULL;
    PROC* running = NULL;
    int proc_count = 0;

    PROC* proc_arr = init(&proc_count);

    for (int i = 0; i < proc_count; i++) {
        insert_rq(&rq_head, &proc_arr[i]);
    }

    printf("init: ");
    print_rq(rq_head);
    printf("\n");

   

    while (sched(&running, &rq_head, proc_count, proc_arr));

    cleanup(proc_arr, proc_count, rq_head);

    return 0;
}