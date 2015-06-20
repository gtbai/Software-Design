#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

class Node{ 
    public:
        Node(int pData){
        //构造方法1，传入参数：  节点数据
            data = pData;
            downNode = NULL;
            nextNode = NULL;
        }
        Node(int pData, Node* pDownNode, Node* pNextNode){
        //构造方法2，传入参数：节点数据，指向该节点之下节点的指针，指向该节点之后节点的指针
            data = pData;
            downNode = pDownNode;
            nextNode = pNextNode;
        }
        void setData(int pData){
        //设置节点存储的数据
            data = pData;
        }
        int getData(void){
        //获得结点存储数据
            return data;
        }
        void setPos(int pPos){
        //设置节点在跳表中的相对位置
            pos = pPos;
        }
        int getPos(void){
        //获得节点在跳表中的相对位置
            return pos;
        }
        void setDownNode(Node* pDownNode){
        //设置当前节点的下邻节点
            downNode = pDownNode;
        }
        Node* getDownNode(void){
        //获得当前节点的下邻节点
            return downNode;
        }
        void setNextNode(Node* pNextNode){
        //设置当前节点的后邻节点
            nextNode = pNextNode;
        }
        Node* getNextNode(void){
        ////获得当前节点的后邻节点
            return nextNode;
        }
    private:
        int data;                    //存储节点的数据
        int pos;                    //存储节点的相对位置
        Node* downNode;            //指向该节点之下的节点的指针
        Node* nextNode;               //指向该节点之后节点的指针
};

class SkipList{
    //跳表类
    public:
        SkipList(){ //跳表的构造方法
            top = new Node(INT_MIN);    //顶节点存储的数据为最小的整数
            tail = new Node(INT_MAX);   //尾结点存储的数据为最大的整数
            top->setNextNode(tail);     //将顶节点和尾结点连接起来
            height = 1;
            width = 2;
        }
        int randomValue(){              //按照“抛硬币”方法产生一个随机数
            int result = 1;
            srand(time(NULL));
            while ((rand() * 1.0 / RAND_MAX) > 0.5) result++;
            return result;
        }
        void updatePos(){               //更新底层节点的位置编号
            Node* tmp = top;
            while (tmp->getDownNode()) tmp = tmp->getDownNode();
            for (int i=0;i<width;i++) {tmp->setPos(i); tmp = tmp->getNextNode(); }
        }
        void insertNode(int data){
            //向跳表中插入一个数据为data的结点
            int floor = randomValue();
            if (floor > height){    //如果要插入节点所占高度大于跳表高度时，先将顶、尾节点补至相应高度
                for (int i = 0; i < floor - height; i++){
                    Node* newTop = new Node(INT_MIN);
                    Node* newTail = new Node(INT_MAX);
                    newTop->setDownNode(top);
                    newTail->setDownNode(tail);
                    newTop->setNextNode(newTail);
                    top = newTop;
                    tail = newTail;
                    height++;
                }
            }
            Node* preserveTop = top;
            for (int i=0;i < height - floor;i++) top = top->getDownNode();  //将top下沉到指定高度
            Node* above = NULL;     //用来存储节点的上邻节点，以便连接
            while(top){
                Node* current = top->getNextNode();
                Node* previous = top;
                while (!((data >= previous->getData()) && (data <= current->getData())))
                    {previous = current; current = current->getNextNode();}
                Node* my = new Node(data);
                previous->setNextNode(my);
                my->setNextNode(current);
                if (above)  above->setDownNode(my);
                above = my;
                top = top->getDownNode();
            }
            top = preserveTop;
            width++;
            this->updatePos();  //调用函数，更新底层节点的位置编号
    }
    int searchNode(int data){
        //在跳表中查找数据为data的节点
        Node* tmp = top;
        while (1){
            while ( tmp->getNextNode()->getData() < data)  tmp = tmp->getNextNode();
            if (!(tmp->getDownNode())){     //判断是否到达底层
                if ( tmp->getNextNode()->getData() == data)  return (tmp->getNextNode()->getPos()+1);
                else return 0;
            }
            tmp = tmp->getDownNode();
        }
    }
    bool deleteNode(int data){
        //从跳表中删除数据为data的节点，返回的布尔值代表是否删除成功
        if (this->searchNode(data)){    //首先确认跳表中存在此节点
            Node* lPre = top;
            while(lPre){
                Node* pre = lPre, * tmp = lPre->getNextNode();
                while ((tmp->getNextNode()) && (tmp->getNextNode()->getData() <= data)) {
                    pre = tmp;
                    tmp = tmp->getNextNode();
                }
                if (tmp->getData() == data){
                    pre->setNextNode(tmp->getNextNode());
                    delete tmp;
            }
            lPre = lPre->getDownNode();
        }
        width--;
        return true;
        }
        return false;
    }
    void showList(){
        //打印跳表
        int *datas = new int[width], *counts = new int[width];
        int **graph;    //申请一个width*height的数组，用于存储跳表的信息
        graph = new int*[width];
        for (int i=0;i<width;i++) graph[i] = new int[height];
        for (int i=0;i<width;i++) for (int j=0;j<height;j++) graph[i][j] = 0;
        Node* tmp = top;
        while (tmp->getDownNode()) tmp = tmp->getDownNode();
        for (int i=0;i<width;i++) {datas[i] = tmp->getData();tmp = tmp->getNextNode(); }
        for(int i=0;i<width;i++) counts[i]=0;
        updatePos();
        tmp = top;
        while(tmp){
            Node* tmp2 = tmp;
            while (tmp2){
                Node* tmp3 = tmp2;
                while (tmp3->getDownNode()) tmp3 = tmp3->getDownNode();
                counts[tmp3->getPos()]++;
                tmp2 = tmp2->getNextNode();
            }
            tmp = tmp->getDownNode();
        }
        for (int i=0;i<width;i++){
            for (int j=0;j<counts[i];j++){
                graph[i][j] = datas[i];
            }
        }
        for (int i = height-1;i>=0;i--){
            for (int j=0;j<width;j++){
                if (graph[j][i]) printf("%d\t", graph[j][i]);
                else printf(" \t");
            }
            printf("\n");
        }
    }
    private:
        Node *tail;     //指向尾结点的指针
        Node *top;      //指向顶节点的指针
        int height;     //跳表的高度
        int width;      //跳表的宽度
};

int main()
{
    SkipList sl;
    int ch = 0;
    int data;
    while (ch!='0'){
        printf("==========跳表==========\n");
        printf("输入序号以执行对应操作：\n1.插入一个节点\n2.查找指定数据\n3.删除指定节点\n4.显示跳表\n0.退出\n");
        ch = getch();
        printf("=====================\n");
        switch(ch){
            case '1':
                printf("请输入您想插入的数据：");
                scanf("%d", &data);
                sl.insertNode(data);
                printf("插入成功！\n");
                break;
            case '2':
                printf("请输入您想查找的数据：");
                scanf("%d", &data);
                if (sl.searchNode(data)) printf("你所查找的数据在跳表中的位置是：%d\n", sl.searchNode(data));
                else printf("在跳表未找到您想要查找的数据！\n");
                break;
            case '3':
                printf("请输入您想删除的数据：");
                scanf("%d", &data);
                if (sl.deleteNode(data)) printf("删除成功！\n");
                else printf("删除失败，未找到该节点！\n");
                break;
        case '4':
                printf("跳表如下所示：\n");
                sl.showList();
                break;
        }
        printf("=====================\n");
    }
    return 0;
}
