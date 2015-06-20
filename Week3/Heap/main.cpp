#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <conio.h>
using namespace std;

class PriorityQueue{             //优先队列类
private:
    int *elements;      //用来存放优先队列中元素的数组
    int n;              //用来存放优先队列中元素的个数
    int maxSize;        //优先队列的最大容量
public:
    PriorityQueue(int pMaxSize){
        /*构造函数：初始化一个优先队列*/
        maxSize = pMaxSize+1;
        elements = new int[maxSize];
        n = 0;
    }
    bool isEmpty(){
        /*函数：判断优先队列是否为空*/
        return !(n); 
    }
    bool isFull(){
        /*函数：判断优先队列是否满*/
        return (n==(maxSize-1));
    }
    int searchElem(void){
        /*函数：查找操作*/
        return elements[1];
    }
    void insertElem(int elem){
        /*函数：向优先队列中插入一个元素*/
        int i = n+1;
        if (this->isFull()) return; //如果优先队列已满，则返回
        if (this->isEmpty()) elements[1] = elem;    //如果优先队列为空，那么将元素插入到第一个位置
        else{
            while((i!=1) && (elem<elements[i/2])){  //寻找插入的合适位置
                elements[i] = elements[i/2];
                i/=2;
            }
        }
        elements[i] = elem;
        n++;    //优先队列中元素个数增一
    }
    int deleteElem(int index){
        /*函数：从优先队列中删除第index个元素*/
        if (this->isEmpty()|| index>n) return INT_MIN;
        int result = elements[index], parent = index, child = 2*parent, tmp = elements[n--];
        while (child <= n){
            if ((child<n)&&(elements[child] > elements[child+1])) child++;
            if (tmp < elements[child]) break;
            elements[parent] = elements[child];
            parent = child; child*=2;
        }
        elements[parent] = tmp;
        return result;
    }
    void displayQueue(int index, int space){
        /*函数：打印优先队列*/
        if (index <= n){
            displayQueue(index*2+1, space+1);    //递归打印左子树
            for (int i=0; i<2*space;i++) printf(" ");
            printf("%d\n", elements[index]);
            displayQueue(index*2, space+1);      //递归打印右子树
        }
    }
};
int main()
{
    int max, ch = 0, elem, result;
    printf("请输入优先队列元素的最大个数：");
    scanf("%d", &max);
    PriorityQueue pq(max);
    while (ch!='0'){
        printf("==========优先队列==========\n");
        printf("输入序号以执行对应操作：\n1.插入一个元素\n2.查找优先级最大的元素\n3.删除指定处于指定位置的元素\n4.显示优先队列\n0.退出\n");
        ch = getch();
        printf("=====================\n");
        switch(ch){
        case '1':
            printf("请输入您想插入的元素：");
            scanf("%d", &elem);
            pq.insertElem(elem);
            printf("插入成功！\n");
            break;
        case '2':
            printf("优先级最高的元素是：%d\n", pq.searchElem());
            break;
        case '3':
            printf("请输入您想删除的元素编号：");
            scanf("%d", &elem);
            result = pq.deleteElem(elem);
            if (result==INT_MIN) printf("删除失败，未找到该元素！\n");
            else printf("删除成功！您删除的元素为：%d\n", result);
            break;
        case '4':
            printf("优先队列如下所示：\n");
            pq.displayQueue(1, 0);
            break;
    }
    printf("=====================\n");
    }
    return 0;
}
