#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

typedef struct buckect{
    vector<int> nodes;
    int count;
}buckect;
void CountingSort(int *A, int *B, int k, int n);   //对数组A进行计数排序，结果存于B数组中
int GetDigit(int data, int k);  //获取data的第k位数
void RadixSort(int *A, int *C, int d, int n);  //对数组A进行基数排序， 结果存于数组C中
void BucketSort(int *A, int n); //对数组A进行桶排序
int main(){
    clock_t start, finish;
    int len, max_num = 0, d =1; //用于存储数组长度
    printf("请输入数组长度：");
    scanf("%d",&len);
    int* A = new int[len], *B = new int[len+1], *C = new int[len];
    for (int i=0; i<len; i++){
        printf("请输入数组中第%d个元素的值：", i+1);
        scanf("%d", A+i);
        if (A[i] > max_num) max_num = A[i];
        int leftnum = A[i], another_d = 0;
        while (leftnum != 0){
            another_d++;
            leftnum/=10;
        }
        if (another_d > d) d = another_d;
    }
    start = clock();
    CountingSort(A, B, max_num, len); //对A[0, 1, ..., len-1]进行快速排序
    finish = clock();
    printf("计数执行时间是 %d ms\n", (int)((double)(finish - start) / CLOCKS_PER_SEC * 1000));
    printf("计数排序后的数组如下所示：\n");
    for (int i=1; i<len+1; i++){
        printf("%d ", B[i]);
    }
    printf("\n");
    printf("基数排序后的数组如下所示：\n");
    start = clock();
    RadixSort(A, C, d, len);
    finish = clock();
    printf("基数执行时间是 %d ms\n", (int)((double)(finish - start) / CLOCKS_PER_SEC * 1000));
    printf("基数排序后的数组如下所示：\n");
    for (int i=0; i<len; i++){
        printf("%d ", C[i]);
    }
    printf("\n");
    start = clock();
    BucketSort(A, len);
    finish = clock();
    printf("桶执行时间是 %d ms\n", (int)((double)(finish - start) / CLOCKS_PER_SEC * 1000));
    printf("桶排序后的数组如下所示：\n");
    for (int i=0; i<len; i++){
        printf("%d ", A[i]);
    }
    printf("\n");
    return 0;
}
void CountingSort(int *A, int *B, int k, int n){
    /*对数组A进行计数排序，结果存于B数组中*/
    int *C = new int[k+1], i;
    for (i=0; i<=k; i++) C[i] = 0;
    for (i=0; i<n; i++) C[A[i]]++;
    for (i=1; i<=k; i++) C[i]+=C[i-1];
    for (i=n-1; i>=0; i--){
        B[C[A[i]]] = A[i];
        C[A[i]]--;
    }
}
int GetDigit(int data, int k){
    /*获取data的第k位数*/
    int a[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
    return (data / a[k-1]) % 10;
}

void RadixSort(int *A, int *C, int d, int n){
    /*对数组A进行基数排序， 结果存于数组C中*/
    const int radix = 10;
    int *count = new int[radix], i, j, k;
    for (i=0; i<n; i++) C[i] = A[i];
    int *bucket = new int[n];
    for (k=1; k<=d; k++){//逐位处理
        for (i=0; i<radix; i++) count[i] = 0;   //每个桶里都没有数据
        for (i=0; i<n; i++) count[GetDigit(C[i], k)]++; //统计第k位上0-9各自出现的次数
        for (i=1; i<radix; i++) count[i] += count[i-1]; //count[i]存储第k位上数字为i的元素在桶中的右边界
        for (i=n-1; i>=0; i--){
            j = GetDigit(C[i], k);
            bucket[--count[j]] = C[i];
        }
        for (i=0; i<n; i++) C[i] = bucket[i];
    }
    free(count);
    free(bucket);
}
void InsertSort(vector<int> &A, int low, int high){
    /*对A[low, low+1, ... high]进行插入排序*/
    int i, j, tmp;
    for (i=low+1; i<=high; i++){
        if (A[i-1] > A[i]){
            tmp = A[i];
            for (j=i; (j>0) && (tmp < A[j-1]); j--)
                A[j] = A[j-1];
        A[j] = tmp;
        }
    }
}
void BucketSort(int *A, int n){
    /*对数组A进行桶排序*/
    int min_, max_, num, pos, i, j, k;
    buckect *ArrBuckets;
    min_ = max_ = A[0]; //找数组中的最小元素和最大元素
    for (i=1; i<n; i++){
        if (A[i] < min_) min_ = A[i];
        if (A[i] > max_) max_ = A[i];
    }
    num = (max_ - min_) / 10 + 1;
    ArrBuckets = new buckect[num];
    for (i=0; i<num; i++){
        ArrBuckets[i].nodes.clear();
        ArrBuckets[i].count = 0;
    }
    for (i=0; i<n; i++){    //将元素放入对应桶中
        k = (A[i] - min_) / 10; //获取对应的桶的编号
        ArrBuckets[k].nodes.push_back(A[i]);
        ArrBuckets[k].count++;
    }
    pos = 0;
    for (i=0; i<num; i++){
        int *nodes = new int[ArrBuckets[i].count];
        for (j=0; j<ArrBuckets[i].count;j++) nodes[j] = ArrBuckets[i].nodes[j];
        sort(nodes, nodes+ArrBuckets[i].count);
        for (j=0; j<ArrBuckets[i].count; j++){
            A[pos++] = nodes[j];
        }
    }
}
