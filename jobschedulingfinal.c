#include<stdio.h>
#include<stdlib.h>

#define MAX 100


typedef struct process {
    int p_no;
    int start_AT, AT; 
    int BT_left, BT;   
    int CT, TAT, WT, RT; 
    int priority;
} process;


void swap(process* a, process* b) {
    process temp = *a;
    *a = *b;
    *b = temp;
}


void sortByArrival(process arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].AT > arr[j].AT) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}


void displayGanttChart(process gantt[], int ganttSize) {
    printf("\nGantt Chart (IS indicates ideal state):\n");

    for (int i = 0; i < ganttSize; i++) {
        if (gantt[i].p_no == -1)
            printf(" | IS ");
        else
            printf(" | P%d ", gantt[i].p_no);
    }
    printf("|\n");

    int time = 0;
    for (int i = 0; i < ganttSize; i++) {
        printf("%d\t", time);
        time += gantt[i].BT;
    }
    printf("%d\n", time);
}


void displayProcesses(process proc[], int n) {
    printf("\n+-------------+--------------+------------+-----------------+-----------------+--------------+---------------+----------+\n");
    printf("| Process No. | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Response Time | Priority |\n");
    printf("+-------------+--------------+------------+-----------------+-----------------+--------------+---------------+----------+\n");

    for (int i = 0; i < n; i++) {
        printf("|     %2d      |      %2d      |     %2d     |        %2d       |        %2d       |      %2d      |      %2d      |    %2d    |\n",
               proc[i].p_no, proc[i].AT, proc[i].BT, proc[i].CT, proc[i].TAT, proc[i].WT, proc[i].RT, proc[i].priority);
    }
    printf("+-------------+--------------+------------+-----------------+-----------------+--------------+---------------+----------+\n");
}

 
void calculateMetrics(process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].TAT = proc[i].CT - proc[i].AT;        
        proc[i].WT = proc[i].TAT - proc[i].BT;        
        proc[i].RT = proc[i].CT - proc[i].BT;         
    }
}


int findHighestPriorityProcess(process proc[], int n, int time) {
    int maxPriority = -1;
    int index = -1;
    for (int i = 0; i < n; i++) {
        if (proc[i].AT <= time && proc[i].BT_left > 0) {
            if (proc[i].priority > maxPriority) {
                maxPriority = proc[i].priority;
                index = i;
            }
        }
    }
    return index;
}


void priorityPreemptiveScheduling(process proc[], int n) {
    int time = 0;
    int completed = 0;
    int ganttSize = 0;
    process gantt[MAX];  

    while (completed != n) {
        int index = findHighestPriorityProcess(proc, n, time);

        if (index != -1) {
            if (proc[index].BT_left == proc[index].BT) {
                proc[index].RT = time - proc[index].AT;  
            }

            proc[index].BT_left -= 1;
            gantt[ganttSize++] = proc[index];

            if (proc[index].BT_left == 0) {
                proc[index].CT = time + 1;  
                completed++;
            }
        } else {
           
            gantt[ganttSize].p_no = -1;  
            gantt[ganttSize++].BT = 1;
        }
        time++;
    }

    calculateMetrics(proc, n);
    displayProcesses(proc, n);
    displayGanttChart(gantt, ganttSize);
}


void calculateAverageMetrics(process proc[], int n) {
    int totalTAT = 0, totalWT = 0, totalRT = 0;
    for (int i = 0; i < n; i++) {
        totalTAT += proc[i].TAT;
        totalWT += proc[i].WT;
        totalRT += proc[i].RT;
    }

    printf("\nAverage Turnaround Time: %.2f\n", (float)totalTAT / n);
    printf("Average Waiting Time: %.2f\n", (float)totalWT / n);
    printf("Average Response Time: %.2f\n", (float)totalRT / n);
}


int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    process proc[MAX];

    
    for (int i = 0; i < n; i++) {
        proc[i].p_no = i + 1;
        printf("Enter Arrival Time, Burst Time, and Priority for Process %d: ", i + 1);
        scanf("%d %d %d", &proc[i].AT, &proc[i].BT, &proc[i].priority);
        proc[i].BT_left = proc[i].BT;
    }

    sortByArrival(proc, n);
    priorityPreemptiveScheduling(proc, n);
    calculateAverageMetrics(proc, n);

    return 0;
}