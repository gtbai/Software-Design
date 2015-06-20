#include<iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#define ELEM_NUM 10
#define LENGTH 16
#define B 1009          //取跟ELEM_NUM最接近的质数作为散列表的大小
using namespace std;
//开放寻址法哈希表的存储结构定义
struct open_records{    //开放寻址法一条记录的存储结构的定义
    char key[LENGTH+1];
};
typedef open_records OPEN_HASH[B];
//拉链法哈希表的存储结构定义
struct celltype{        //拉链法一条记录的存储结构的定义
    char key[LENGTH+1]; //数据域
    celltype *next;     //指针域
};
typedef celltype * cellptr;
typedef cellptr LINKED_HASH[B];
char keys[ELEM_NUM][LENGTH+1];  //字符串关键字数组
char emp[LENGTH+1] = "";        //空字符串，用作判断字符串是否为空
char *rand_str(char *str,const int len);    //生成随机字符串的函数声明
unsigned int Hash(char *str);   //字符串哈希函数的声明
void Open_Insert(char *str, OPEN_HASH F);   //开放寻址法插入函数的声明
int Open_Search(char *str, OPEN_HASH F);    //开放寻址法查找函数的声明
int Linked_Search(char *str, LINKED_HASH F);//拉链法插入函数的声明
void Linked_Insert(char *str, LINKED_HASH F);//拉链法查找函数的声明
int main()
{
    srand(time(NULL));           //初始化随机数发生器
    OPEN_HASH OpenHashTable;     //定义一个长度为B的open_records类型数组作为闭散列表
    LINKED_HASH LinkedHashTable;
    for (int i=0;i<B;i++)        //将散列表中所有位置的关键字初始化为空字符串
    {
        strcpy(OpenHashTable[i].key, emp);
    }
    for (int i=0;i<ELEM_NUM;i++)
    {
        rand_str(keys[i], LENGTH);  //生成随机字符串并将其存储在keys[i]中
        Open_Insert(keys[i], OpenHashTable);    //把刚刚读入的一个字符串添加到开放寻址法实现的Hash表中
        Linked_Insert(keys[i], LinkedHashTable);    //把字符串添加到拉链法实现的Hash表中
    }
    cout<<"利用开放寻址法实现Hash算法，打印出每个字符串及其在哈希表中的位置"<<endl;
    cout<<"===================="<<endl;
    for (int i=0;i<ELEM_NUM;i++)
    {
        //在开放寻址法实现的哈希表中查找每一个字符串并打印出字符串及其在哈希表中的位置
        cout<<keys[i]<<'\t'<<Open_Search(keys[i], OpenHashTable)<<endl;
    }
    cout<<"===================="<<endl;
    cout<<"利用拉链法实现Hash算法，打印出每个字符串及其在哈希表中的位置"<<endl;
    cout<<"===================="<<endl;
    for (int i=0;i<ELEM_NUM;i++)
    {
        //在拉链法实现的哈希表中查找刚刚插入的字符串并打印出字符串及其在哈希表中的位置
        cout<<keys[i]<<'\t'<<Hash(keys[i])<<'\t'<<Linked_Search(keys[i], LinkedHashTable)<<endl;
    }
    cout<<"===================="<<endl;
}
char *rand_str(char *str,const int len)
/*随机生成长为len的字符串，返回字符串的首地址*/
{
    int i;
    for(i=0;i<len;i++)
        str[i]='A'+rand()%26;
    str[i]='\0';
    return str;
}
// 字符串哈希函数
unsigned int Hash(char *str)
{
    unsigned int seed = 1; // 例如31 131 1313 13131 131313等等
    unsigned int h = 0;
    for (;*str;str++)
    {
        h = seed * h + *str;
    }
    return (h % B);
}
void Open_Insert(char *str, OPEN_HASH F)
{
    int locate = Hash(str), first = Hash(str), rehash = 0;
    //first为str的散列地址，locate记录当前正在查询的地址，rehash为线性探测的次数
    while ((rehash<B) && strcmp(F[locate].key, str))           //当还没有找遍哈希表中的所有地址时循环继续
    {
        locate = (first+rehash)%B;
        if(!strcmp(F[locate].key, emp))     //如果找到了一个空的桶
            strcpy(F[locate].key, str);     //将关键字字符串放入空桶中
        else
            rehash++;                      //否则继续寻找空桶
        if (rehash>=B)
            cout<<"Hash Table is full!"<<endl;  //如果找遍所有的地址还没发现空桶，则哈希表已满
    }
}
int Open_Search(char *str, OPEN_HASH F)
/*在哈希表中查找str关键字，若找到则返回str所在的地址，否则返回-1*/
{
    int locate = Hash(str), first = Hash(str), rehash = 0;
    //first为str的散列地址，locate记录当前正在查询的地址，rehash为线性探测的次数
    while((rehash<B) && strcmp(F[locate].key, emp))
    //如果还没有查询完哈希表所有的位置并且还未遇到空桶
    {
        if (!strcmp(F[locate].key, str))    //如果找到str关键字则返回当前查询的地址
            return locate;
        else
            rehash++;                       //否则继续寻找关键字
        locate = (first+rehash) % B;
    }
    return -1;  //如果未找到关键字str则返回-1
}
int Linked_Search(char *str, LINKED_HASH F)
/*在以拉链法存储的哈希表中查找str关键字，如果找到则返回str在同义词子表
中的位置，未找到则返回-1*/
{
    cellptr tmpptr = F[Hash(str)];
    int locate = 1; //locate记录str在其对应同义词表中的相对位置
    while (tmpptr)
    {
        if (!strcmp(tmpptr->key, str))  //如果在同义词表中找到了str关键字
            return locate;
        else{
            tmpptr = tmpptr->next;      //否则令tmpptr指向同义词表中的下一个元素
            locate++;
        }
    }
    return -1;
}
void Linked_Insert(char *str, LINKED_HASH F)
/*向拉链法存储结构实现的哈希表中插入一条以str为关键字的记录*/
{
    int bucket;
    cellptr oldheader;
    if (Linked_Search(str, F)==-1)  //如果在哈希表中没有查找到以str为关键字的记录
    {
        bucket = Hash(str);         //把str的散列地址赋值给bucket
        oldheader = F[bucket];      //oldheader指向同义词表的第一个结点
        F[bucket] = new celltype;   //申请一个新的结点
        strcpy(F[bucket]->key, str); //使得新结点的数据域存放关键字
        F[bucket]->next = oldheader;//令新结点的指针域指向原来的头结点
    }
}
