/**
共实现了3种选择主元的方法：
1. 选择下标最大的元素作为主元
2. 随机选择一个元素作为主元
3. 选择最左元素，中间元素以及最后一个元素的中位数作为主元
共做出了3个优化：
1. 数组长度小于10时使用插入排序
2. 每次排序时将相同的元素聚集在主元周围
3. 减少递归次数
**/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

void InsertSort(long long int *A, int low, int high);  //对A[low, low+1, ... high]进行插入排序
void QuickSort(long long int *A, int low, int high); //快速排序函数， 对A[low, low+1, ... high]进行排序
void QuickSort2(long long int *A, int low, int high); //优化版快排函数，聚集相同元素
int Partition(long long int *A, int low, int high); //划分函数，对A[low, low+1, ... high]进行划分，以A[high]作为主元
long long int FindPivot_High(long long int *A, int low, int high);  //将数组中下标最大的元素作为主元
long long int FindPivot_Random(long long int *A, int low, int high);   //从数组中随机选取一个元素作为主元
long long int FindPivot_ThreeMid(long long int *A, int low, int high);  //选取数组中最左、中间以及最右的三个元素的中位数作为主元
int main()
{
    int len; //用于存储数组长度
    printf("请输入数组长度：");
    scanf("%d",&len);
    long long int* A = new long long  int[len];
    for (int i=0; i<len; i++){
        printf("请输入数组中第%d个元素的值：", i+1);
        scanf("%lld", A+i);
    }
    clock_t start, finish;
    start = clock();
    QuickSort2(A, 0, len-1); //对A[0, 1, ..., len-1]进行快速排序
    finish = clock();
    printf("执行时间是 %d ms", (int)((double)(finish - start) / CLOCKS_PER_SEC * 1000));
    printf("排序后的数组如下所示：\n");
    for (int i=0; i<len; i++){
        printf("%lld ", A[i]);
    }
    printf("\n");
    return 0;
}
void InsertSort(long long int *A, int low, int high){
    /*对A[low, low+1, ... high]进行插入排序*/
    int i, j;
    long long int tmp;
    for (i=low+1; i<=high; i++){
        if (A[i-1] > A[i]){
            tmp = A[i];
            for (j=i; (j>0) && (tmp < A[j-1]); j--)
                A[j] = A[j-1];
        A[j] = tmp;
        }
    }
}
void QuickSort2(long long int *A, int low, int high){
    /*优化版快排函数，聚集相同元素*/
    if (low < high){
        if (high-low+1 < 10)
            InsertSort(A, low, high);   //当元素个数小于10时，使用插入排序
        else{
            int first = low, left = low,
                last = high, right = high,
                leftlen = 0, rightlen = 0;
            long long int pivot = FindPivot_ThreeMid(A, low, high);   //三数取中法找主元
            while (low < high){
                while (low < high && A[low] <= pivot){
                    if (A[low] == pivot){   //处理相同元素
                        swap(A[left], A[low]);
                        left++; leftlen++;
                    }
                    low++;
                }
                A[high] = A[low];
                while (low < high && A[high] >= pivot){
                    if (A[high] == pivot){  //处理相同元素
                        swap(A[right], A[high]);
                        right--; rightlen++;
                    }
                    high--;
                }
                A[low] = A[high];
            }
            A[high] = pivot;
            //把与主元相同的元素移到主元周围
            int i = low-1, j = first;
            while (j<left && A[i]!=pivot){
                swap(A[j], A[i]);
                j++; i--;
            }
            i = low+1, j = last;
            while (j>right && A[i]!=pivot){
                swap(A[i], A[j]);
                j--; i++;
            }
            QuickSort2(A, first, low-1-leftlen);
            QuickSort2(A, low+1+rightlen, last);
        }
    }

}
void QuickSort(long long int *A, int low, int high){
    /*快速排序函数， 对A[low, low+1, ... high]进行排序*/
    if (low < high){
//        if (high-low+1 < 10)
//            InsertSort(A, low, high);   //当元素个数小于10时，使用插入排序
//        else{ //如果这段数组中有一个以上元素，那么需要划分
//            while (low < high){
//                int mid = Partition(A, low, high);
//                QuickSort(A, low, mid-1);   //递归地对左部分执行快速排序
//                low = mid + 1;
//            }
//        }
//        if (high-low+1 < 10)
//            InsertSort(A, low, high);   //当元素个数小于10时，使用插入排序
//        else{
            int mid = Partition(A, low, high);
            QuickSort(A, low, mid-1);
            QuickSort(A, mid+1, high);
        }
//    }
}
int Partition(long long int *A, int low, int high){
    /*划分函数，对A[low, low+1, ... high]进行划分，以A[high]作为主元*/
    long long int pivot =
    //FindPivot_High(A, low, high);    //找到主元
    //FindPivot_Random(A, low, high);
    FindPivot_ThreeMid(A, low, high);
    int i = low-1;
    for (int j=low; j<high; j++){
        if (A[j] <= pivot){
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i+1], A[high]);
    return i+1;
}
long long int FindPivot_High(long long int *A, int low, int high){
    /*将数组中下标最大的元素作为主元*/
    return A[high];
}
long long int FindPivot_Random(long long int *A, int low, int high){
    /*从数组中随机选取一个元素作为主元*/
    srand((unsigned)time(NULL));    //产生随机数种子
    int pos = rand()%(high-low+1) + low;
    swap(A[pos], A[high]);
    return A[high];
}
long long int FindPivot_ThreeMid(long long int *A, int low, int high){
    /*选取数组中最左、中间以及最右的三个元素的中位数作为主元*/
    int mid = (low + high) / 2;
    if (A[low] > A[mid]) swap(A[low], A[mid]); //A[low] <= A[mid]
    if (A[low] > A[high]) swap(A[low], A[high]); //A[low] <= A[high]
    if (A[high] > A[mid]) swap(A[high], A[mid]); //A[low] <= A[high] <= A[mid]
    return A[high];
}
