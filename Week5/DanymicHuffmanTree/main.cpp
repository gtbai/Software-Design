#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 1000

using namespace std;

struct Node{    //Huffman树节点结构
    unsigned char letter;   //字符的值
    int weight, order;  //字符结点的重量及其编号
    Node* parent, *lchild,*rchild;  //父结点、左孩子以及右孩子结点指针
    Node* Front, *Back;  //指向同一重量链的前导结点和后继节点
}*root;

struct LeafNode{    //叶子节点的结构
    Node* charnode; //叶子节点对应的字符节点
    LeafNode* next; //叶子链表中的下一个节点
}*leaf, *weight;

ifstream in, tmpin;
ofstream out, tmpout;
long int all;

void Compression(void); //压缩文件函数
void Decompression(void);   //解压文件函数
void Update(struct Node*); //修改重量信息
void AddChar(unsigned char);    //向动态Huffman树中添加一个字符
unsigned char ReadChar(void);   //从文件中读取一个字符的ASCII码
void ProduceCode(Node*);    //产生编码信息

int main()
{
    char letter;
    printf("==========Dynamic Huffman Coding==========\n");
    printf("1. Dynamic Huffman Compression\n");
    printf("1. Dynamic Huffman Decompression\n");
    printf("Please input 1 or 2 to execute corresponding operation:");
    letter = getchar();
    if (letter == '1') Compression();
    else if (letter == '2') Decompression();
    return 0;
}
void initial(void){
/*初始化子程序*/
    root = (Node *) malloc (sizeof(Node));
    root->parent = root->lchild = root->rchild = NULL;
    root->Front = root->Back = NULL;
    root->weight = 0;
    root->letter = 0;
    root->order = 1000; //根节点的序号最大为1000
    leaf = (LeafNode*) malloc(sizeof(LeafNode));
    weight = (LeafNode*) malloc(sizeof(LeafNode));
    leaf->charnode = root;
    weight->charnode = NULL;
    leaf->next = weight->next = NULL;
}
void Compression(void){
/*压缩子程序*/
    LeafNode * p;
    char tmpletter;
    bool found;
    char tmpstr[MAX];
    char oldfile[MAX], newfile[MAX];    //老新文件路径
    printf("==========Huffman Compression Procedure==========\n");
    printf("Please input the old file name:");
    scanf("%s", oldfile);
    printf("Please input the new file name:");
    scanf("%s", newfile);
    in.open(oldfile);
    tmpout.open("temp.txt");
    out.open(newfile);
    initial();
    all = 0;
    tmpletter = in.get();
    while (!in.eof()){
        found = false;
        all++;
        p = leaf->next;
        while (p){
            if (p->charnode->letter == tmpletter){
                found = true; break;
            }
            else p = p->next;
        }
        if (!found){ //叶子链中无此字符
            ProduceCode(leaf->charnode);    //产生此字符的编码
            for (int i=0; i<8; i++){
                tmpout<<((tmpletter>>(7-i)) & 0x01); //将ASCII码送至临时输出文件
            }
            //tmpout<<tmpletter;
            AddChar(tmpletter); //将tmpletter加入到Huffman树中
            Update(leaf->charnode->parent);     //修改相应结点的重量信息
        }else{
            ProduceCode(p->charnode);   //产生编码
            Update(p->charnode);    //修改重量信息
        }
        tmpletter = in.get();
    }
    tmpout.close();
    tmpin.open("temp.txt");
    out<<all<<endl;
    tmpin.getline(tmpstr, MAX);
    while(strcmp(tmpstr, "")){
        out<<tmpstr<<endl;
        tmpin.getline(tmpstr, MAX);
    }
    in.close();
    tmpin.close();
    out.close();
    remove("temp.txt");
}
void Decompression(void){   //解压子程序
    char oldfile[MAX], newfile[MAX], str[MAX], bit;    //旧文件名和新文件名
    unsigned char letter;
    Node* tmp;
    printf("==========Dynamic Huffman Decompression Program==========\n");
    printf("Please input the old file name:");
    scanf("%s", oldfile);
    printf("Please input the new file name:");
    scanf("%s", newfile);
    in.open(oldfile);   //打开旧文件作为输入流
    out.open(newfile);  //打开新文件作为输出流
    initial();
    in.getline(str, MAX);
    all = atoi(str);
    letter = ReadChar();
    AddChar(letter);
    out<<letter;
    all--;
    while (all>0){  //解压没完成
        tmp = root;
        while ((tmp->lchild) && (tmp->rchild)){
            in>>bit;
            if (bit == '0') tmp = tmp->lchild;
            else tmp = tmp->rchild;
        }
        if (tmp == leaf->charnode){ //为空节点的情况
            letter = ReadChar();
            out<<letter;
            all--;
            AddChar(letter);
        }
        else{   //找到该结点
            letter = tmp->letter;
            out<<letter;
            all--;
        }
        Update(tmp);
    }
    in.close();
    out.close();
}
void InserWeight(Node* Tmp){
    /*根据重量大小将节点插入到重量链中*/
    LeafNode *p, *q;
    bool found = false;
    Node* Tmpa;
    p = weight;
    Tmp->Front = Tmp->Back = NULL;
    while (p->next){    //查找重量相同的链
        if (p->next->charnode->weight == Tmp->weight){
            found = true;
            break;
        }
        else p = p->next;
    }
    if (!found){    //如果没有找到对应的链
        q = (LeafNode*) malloc(sizeof(LeafNode));
        p->next = q;
        q->next = NULL;
        q->charnode = Tmp;
    }
    else{   //如果找到了对应的链，则按照序号由大到小插入
        Tmpa = p->next->charnode;
        //if (Tmpa != Tmp){
            if (Tmp->order >= Tmpa->order){
                Tmp->Back = Tmpa;
                Tmpa->Front = Tmp;
                p->next->charnode = Tmp;
                }
            else{
                while (Tmpa->Back != NULL){
                    if (Tmpa->Back->order < Tmp->order) break;
                    else Tmpa = Tmpa->Back;
                }
                Tmp->Back = Tmpa->Back;
                Tmp->Front = Tmpa;
                Tmpa->Back = Tmp;
                if (Tmp->Back != NULL) Tmp->Back->Front = Tmp;
        }
        //}
    }
}
void Update(Node* Tmp){
    /*修改相应的重量*/
    Node *Tmpa, *Pointer;
    LeafNode *p, *q, *b;
    unsigned char letter;
    while (Tmp != root){    //当还没有遇到根节点时
        p = weight;
        if (Tmp->weight){
            p = weight;
            while (p->next->charnode->weight != Tmp->weight)
                p = p->next;
        if (Tmp->Front != NULL){
            Tmpa = Tmp;
            while(Tmp->Front) {Tmp = Tmp->Front;}
            Pointer = Tmp->lchild;  //交换两个结点
            if (Pointer) Pointer->parent = Tmpa;
            Tmp->lchild = Tmpa->lchild;
            if (Tmp->lchild) Tmp->lchild->parent = Tmp;
            Tmpa->lchild = Pointer;
            Pointer = Tmp->rchild;
            if (Pointer) Pointer->parent = Tmpa;
            Tmp->rchild = Tmpa->rchild;
            if (Tmp->rchild) Tmp->rchild->parent = Tmp;
            Tmpa->rchild = Pointer;
            letter = Tmp->letter;
            Tmp->letter = Tmpa->letter;
            Tmpa->letter = letter;
            if ((!Tmpa->lchild)&&(!Tmpa->rchild)){
                b = leaf;
                while (b){
                    if (b->charnode == Tmp){
                        b->charnode = Tmpa;
                        break;
                    }
                    else b = b->next;
                }
            }
            if ((!Tmp->lchild)&&(!Tmp->rchild)){
                b = leaf;
                while (b){
                    if (b->charnode == Tmpa){
                        b->charnode = Tmp;
                        break;
                    }
                    else b = b->next;
                }
            }
        }
        p->next->charnode = Tmp->Back;  //从重量链中取下该节点
        if (!Tmp->Back){
            q = p->next;
            p->next = q->next;
            free(q);
        }
        else Tmp->Back->Front = NULL;
        }
        Tmp->weight++;  //重量+1
        Tmp->Back = Tmp->Front = NULL;
        InserWeight(Tmp);
        Tmp = Tmp->parent;
    }
}
void AddChar(unsigned char letter){
        /*向Huffman树中添加一结点*/
        Node *Tmpa, *Tmpb, *Tmpc;
        LeafNode* q;
        Tmpa = leaf->charnode;
        Tmpb = (Node *) malloc(sizeof(Node));
        Tmpc = (Node *) malloc(sizeof(Node));
        Tmpb->parent = Tmpc->parent = Tmpa;
        Tmpb->lchild = Tmpc->lchild = Tmpb->rchild = Tmpc->rchild = NULL;
        Tmpb->Front = Tmpc->Front = NULL;
        Tmpb->Back = Tmpc->Back = NULL;
        Tmpb->letter = 0; Tmpc->letter = letter;
        Tmpb->weight = 0; Tmpc->weight = 1;
        Tmpc->order = Tmpa->order-1;
        Tmpb->order = Tmpa->order-2;
        Tmpa->lchild = Tmpb; Tmpa->rchild = Tmpc;
        q = (LeafNode*) malloc(sizeof(LeafNode));
        q->charnode = Tmpc;
        leaf->charnode = Tmpb;
        q->next = leaf->next;
        leaf->next = q;
        InserWeight(Tmpc);
}
void ProduceCode(Node* Pointer){
/*产生编码*/
    char code[50];
    int count = 0, i;
    if (Pointer!=root){ //从叶节点到根节点编码
        while (Pointer != root){
            if (Pointer == Pointer->parent->lchild) code[count] = '0';
            else code[count] = '1';
            count++;
            Pointer = Pointer->parent;
        }
        for (i=count-1; i>=0; i--) tmpout<<code[i];
    }
}
unsigned char ReadChar(void){
/*从文件中读取一个字符的二进制ASCII码并将其转换为字符*/
    unsigned char letter = 0x00, bit;
    for (int i=0; i<8; i++){
        in>>bit;
        bit -= 48;
        letter = (letter | (bit<<(7-i)));
    }
    return letter;
}
