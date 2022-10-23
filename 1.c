#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>



struct job
{
    char job_name[20];
    int arrival_time;  // 抵达时间
    int burst_time;  // 运行时间
    int completion_time;  // 完成时间
    int turn_around_time;  // 周转时间 = Completion Time – Arrival Time
    int waiting_time;  // 等待时间 = Turn Around Time – Burst Time
    float response_ratio;  // 相应比 = (Waiting Time + Burst Time) / Burst Time

}jobs;

void Quick_Sort(struct job *L);
void print_jobs(struct job *L);
void print_jobs_on(struct job *L);
void print_jobs_hrrn(struct job *L);
void sjf(struct job *L, int o);
void hrrn(struct job *L, int o);
int swap(struct job *L, int i, int j);
int sjf_start();
int hrrn_start();

int n;  // 作业数 令为全局变量，同时方便输入快排参数
int main(){

    // sjf_start();
    hrrn_start();

    return 0;
}

int sjf_start(){
    struct job jobs[10];

    // printf("请输入作业数量(上限10)：");
    // scanf("%d", &n);
    // printf("分别输入作业的名称、抵达时间、运行时间\n");
    // for (int i = 0 ; i < n ; i++){
    //     scanf("%s", jobs[i].job_name);  // 作业名，数组即指针
    //     scanf("%d", &jobs[i].arrival_time);  // 到达时间
    //     scanf("%d", &jobs[i].burst_time);  // 运行时间
    // }
    n = 10;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++){
        int ret_arrival_time = (rand() % 20) + 1 ;
        int ret_burst_time = ((rand() % 40) + 1) + 10;
        char string[20];
        sprintf(string, "%d", (i+1)); //把整数i+1以10进制转为字符串保存在string中
        strcpy(jobs[i].job_name, string);
        jobs[i].arrival_time = ret_arrival_time;
        jobs[i].burst_time = ret_burst_time;
    }
    
    printf("随机生成进程、到达时间、运行时间\n");
    print_jobs(jobs);
    sjf(jobs, n);

    double avg_TA_time = 0.0;
    double avg_wait_time = 0.0;
    for (int i = 0 ; i < n ; i++){
        avg_TA_time += jobs[i].turn_around_time;
        // 顺便计算等待时间
        jobs[i].waiting_time = jobs[i].turn_around_time - jobs[i].burst_time;
        avg_wait_time += jobs[i].waiting_time;

    }
    avg_TA_time = avg_TA_time / n;
    avg_wait_time = avg_wait_time / n;
    printf("通过SJF处理后\n");
    print_jobs_on(jobs);
    printf("平均周转时间：%.3f\n",avg_TA_time);
    printf("平均等待时间：%.3f\n",avg_wait_time);
}

int hrrn_start(){
    struct job jobs[10];
    n = 10;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++){
        int ret_arrival_time = (rand() % 20) + 1 ;
        int ret_burst_time = ((rand() % 40) + 1) + 10;
        char string[20];
        sprintf(string, "%d", (i+1)); //把整数i+1以10进制转为字符串保存在string中
        strcpy(jobs[i].job_name, string);
        jobs[i].arrival_time = ret_arrival_time;
        jobs[i].burst_time = ret_burst_time;
    }
    
    printf("随机生成进程、到达时间、运行时间\n");
    print_jobs(jobs);
    hrrn(jobs, n);

    double avg_TA_time = 0.0;
    double avg_wait_time = 0.0;
    for (int i = 0 ; i < n ; i++){
        avg_TA_time += jobs[i].turn_around_time;
        // 顺便计算等待时间
        avg_wait_time += jobs[i].waiting_time;
    }
    avg_TA_time = avg_TA_time / n;
    avg_wait_time = avg_wait_time / n;
    printf("通过hrrn处理后\n");
    print_jobs_hrrn(jobs);

    printf("平均周转时间：%.3f\n",avg_TA_time);
    printf("平均等待时间：%.3f\n",avg_wait_time);
}

int swap(struct job *L, int i, int j){
    int time_temp = L[i].burst_time;
    L[i].burst_time = L[j].burst_time;
    L[j].burst_time= time_temp;

    int arrival_temp = L[i].arrival_time;
    L[i].arrival_time = L[j].arrival_time;
    L[j].arrival_time= arrival_temp;

    char char_temp[20];
    strcpy(char_temp, L[i].job_name);
    strcpy(L[i].job_name, L[j].job_name);
    strcpy(L[j].job_name, char_temp);
}

int swap2(struct job *L, int i, int j){
    int time_temp = L[i].burst_time;
    L[i].burst_time = L[j].burst_time;
    L[j].burst_time= time_temp;

    int arrival_temp = L[i].arrival_time;
    L[i].arrival_time = L[j].arrival_time;
    L[j].arrival_time= arrival_temp;

    int waitt_temp = L[i].waiting_time;
    L[i].waiting_time = L[j].waiting_time;
    L[j].waiting_time= waitt_temp;

    char char_temp[20];
    strcpy(char_temp, L[i].job_name);
    strcpy(L[i].job_name, L[j].job_name);
    strcpy(L[j].job_name, char_temp);
}

int Partition(struct job *L, int l, int r){
    int pivot = L[r].arrival_time; //划分low - pivot - high
    int i = l - 1;  // 移动用指针先指向-1

    for(int j = l ; j < r ; j++){
        if (L[j].arrival_time < pivot){
            i++;
            swap(L, i, j);
        }
    }
    swap(L, i + 1, r);
    return i + 1;
}

void QSort(struct job *L, int l, int r){
    if (l < r) {
        //找到支点的位置
        int position = Partition(L, l, r);
        //对支点左侧的子表进行排序
        QSort(L, l, position - 1);
        //对支点右侧的子表进行排序
        QSort(L, position + 1, r);
    }
}


void sjf(struct job *L, int o){
    int b = 0;
    Quick_Sort(L);  // 抵达时间快排
    // 先处理优先到达的进程
    L[0].completion_time = L[0].burst_time + L[0].arrival_time;  // 完成时间 =  运行时间 + 抵达时间
    L[0].turn_around_time = L[0].burst_time;  // 周转时间 = 运行时间
    
    for(int i = 1; i < o; i++){
        // 最先抵达的0号job先执行，相当于有一个时间区间
        // 下一个job的抵达时间仍然比上一个job的完成时间小的话，即落在0号时间区间里面
        // b++ 以表示等待的job的数量
        // 因为是按抵达时间排序的，所以时间靠后的不做处理也仍然有序
        if(L[i].arrival_time > L[0].completion_time);
        else 
        {
            b++;
        }
    }
    for(int i = 1; i < b ; i++){
        if(L[i].burst_time > L[i+1].burst_time){
            //对落在最开始job运行时间区间里的进行按最短时间排序
            int time_temp = L[i].burst_time;
            L[i].burst_time = L[i+1].burst_time;
            L[i+1].burst_time= time_temp;

            int arrival_temp = L[i].arrival_time;
            L[i].arrival_time = L[i+1].arrival_time;
            L[i+1].arrival_time= arrival_temp;

            char char_temp[20];
            strcpy(char_temp, L[i].job_name);
            strcpy(L[i].job_name, L[i+1].job_name);
            strcpy(L[i+1].job_name, char_temp);
        }
    }

    for(int i = 1; i < n ;i++)
    {
        if(L[i].arrival_time > L[i-1].completion_time)
        {
            // 抵达时间大于上一个完成时间，说明情况与0号作业类似，以当前作业为标准
            L[i].completion_time = L[i].arrival_time + L[i].burst_time;
            L[i].turn_around_time = L[i].burst_time;
        }
        else
        {
            // 当前的完成时间是上一次的完成时间+本次运行时间
            L[i].completion_time = L[i-1].completion_time + L[i].burst_time;
            // 周转时间 = 完成时间 - 抵达时间
            L[i].turn_around_time = L[i].completion_time - L[i].arrival_time;
        }
    }
} // sjf end

void hrrn(struct job *L, int o){
    int b = 0;
    Quick_Sort(L);  // 抵达时间快排
    print_jobs(L);
    // 先处理优先到达的进程
    L[0].waiting_time = 0;  // 第一个作业没有等待时间
    L[0].response_ratio = 0;
    L[0].completion_time = L[0].burst_time + L[0].arrival_time;  // 完成时间 =  运行时间 + 抵达时间
    L[0].turn_around_time = L[0].burst_time;  // 周转时间 = 运行时间
    int i = 1;
    int p = 0;
    while (i < o) {
        // 依次每次循环需要计算相应比
        // 起始位置逐步提升
        i = 1 + p; //外部控制for的循环区间
        int temp2 = i;
        int max = i;
        for(int temp2 = i; temp2 < o; temp2++){
            // 等待时间，p是大循环用于控制已经执行的作业的下标
            // 目的是让完成时间始终保存有序的首位
            // 这里的等待时间与SJF的不一样
            L[temp2].waiting_time = L[p].completion_time - L[temp2].arrival_time;
            // printf("等待时间i=%d,%d\t\n",temp2,L[temp2].waiting_time); 
            // printf("完成时间i=%d,%d\t\n",temp2,L[p].completion_time); 
            // 相应比. 这里必须强制转换，否则传的始终是整数
            L[temp2].response_ratio = (((double)L[temp2].waiting_time + (double)L[temp2].burst_time) / (double)L[temp2].burst_time);
            // printf("内部i=%d, waiting_time=%d, burst_time=%d,  %f\t\n",temp2,L[temp2].waiting_time,L[temp2].burst_time,L[temp2].response_ratio); 

            if (L[max].response_ratio < L[temp2].response_ratio){
                max = temp2; // 保存响应比最大的下标
            }
            
        }
        // printf("max = %d\n",max); 
        // 上面是逐个计算相应比，然后比较得到最大的下标

        // 在交换后再计算当前确定好的完成时间，周转时间，等待时间
        if(temp2 < o || max < o){
            // 这里是把最大相应比的下标通过计算相应比，正确地赋予到当前比较区间的第一个job
            L[temp2].response_ratio =((float)L[max].waiting_time + (float)L[max].burst_time) / (float)L[max].burst_time;
            //printf("外部i=%d,%.3f\t\n",temp2,L[temp2].response_ratio); 
            //printf("内部交换%d\n",max);
            swap2(L,temp2,max);  // 版本2有交换等待时间
            L[temp2].completion_time = L[p].completion_time + L[temp2].burst_time;
            L[temp2].turn_around_time = L[temp2].completion_time - L[temp2].arrival_time;
        }
        p++;
    }
} // hrrn end



void Quick_Sort(struct job *L){  
    // 由于全局变量n是后输入的，函数需要摆在后面
    QSort(L, 0, n - 1);
}
void print_jobs(struct job *L){
    printf("Process\t\tArrival Time\tBurst Time\n");
    for (int i = 0 ; i < n ; i++)
    {
        printf("%s\t\t%dms\t\t%dms\t\t\n",L[i].job_name, L[i].arrival_time, L[i].burst_time);
    }
}
void print_jobs_on(struct job *L){
    printf("Process\t\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\n");
    for (int i = 0 ; i < n ; i++)
    {
        printf("%s\t\t%dms\t\t%dms\t\t%dms\t\t\t%dms\t\t\t%dms\n",L[i].job_name, L[i].arrival_time, L[i].burst_time, L[i].completion_time,L[i].turn_around_time,L[i].waiting_time);
    }
}
void print_jobs_hrrn(struct job *L){
    printf("Process\t\tArrival Time\tBurst Time\tCompletion Time\t\tTurn Around Time\tWaiting Time\tResponse Ratio\n");
    for (int i = 0 ; i < n ; i++)
    {
        printf("%s\t\t%dms\t\t%dms\t\t%dms\t\t\t%dms\t\t\t%dms\t\t%.3f\n",L[i].job_name, L[i].arrival_time, L[i].burst_time, L[i].completion_time,L[i].turn_around_time,L[i].waiting_time,L[i].response_ratio);
    }
}
