#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

class Tuple3{   //三元组类
private:
    int row;    //行数
    int column; //列数
    int data;   //数据
public:
    Tuple3(int pRow, int pColumn, int pData){
    /*构造函数*/
        row = pRow; column = pColumn; data = pData;
    }
    int getRow(void){
        /*函数：返回行数*/
        return row;
    }
    int getColumn(void){
        /*函数：返回列数*/
        return column;
    }
    int getData(void){
        /*函数：返回数据*/
        return data;
    }
};
class Matrix{   //矩阵类
private:
    vector<Tuple3> elems;   //存放矩阵元素
    int rowNum; //矩阵的行数
    int columnNum;  //矩阵的列数
    int totalNum;    //矩阵包含元素的个数
public:
    Matrix(){
        rowNum = 0;
        columnNum = 0;
        totalNum = 0;
    }
    void insertElem(int row, int column, int data){
        Tuple3 t3(row, column, data);
        this->elems.push_back(t3);
        if (row > rowNum) rowNum = row;
        if (column > columnNum) columnNum = column;
        totalNum += 1;
    }
    void setColumnNumber(int pColumnNumber){
        columnNum = pColumnNumber;
    }
    int getColumnNumber(void){
        /*函数：返回矩阵的列数*/
        return columnNum;
    }
    void setRowNumber(int pRowNumber){
        /*函数：设置矩阵的行数*/
        rowNum = pRowNumber;
    }
    int getRowNumber(void){
        /*函数：返回矩阵的列数*/
        return rowNum;
    }
    void setTotalNumber(int pTotalNumber){
        /*函数：设置矩阵的元素个数*/
        totalNum = pTotalNumber;
    }
    int getTotalNumber(void){
        /*函数：返回矩阵的列数*/
        return totalNum;
    }
    void setElem(int index, int row, int col, int data){
        Tuple3 tmp(row, col, data);
        elems[index] = tmp;
    }
    vector<Tuple3> getElems(void){
        /*函数：返回元素列表*/
        return elems;
    }
    void showMatrix(void){
        /*函数：显示矩阵*/
        int **array;
        array = new int*[rowNum];
        for (int i=0; i<rowNum; i++) array[i] = new int[columnNum];
        for (int i=0; i<rowNum; i++) for (int j=0; j<columnNum; j++) \
            array[i][j] = 0;
        for (int i=0; i<totalNum; i++){     //首先将元素装到一个数组里
            Tuple3 elem = elems[i];
            array[elem.getRow()-1][elem.getColumn()-1] = elem.getData();
        }
        for (int i=0; i<rowNum; i++){
            for (int j=0; j<columnNum; j++){
                printf("%d\t", array[i][j]);
            }
            printf("\n");
        }
    }
};
void Transpose(Matrix* matrixA, Matrix* matrixB){
    /*函数：将matrixA指向的矩阵转置后存放在matrixB指向的矩阵中*/
    for (int i=1; i <= matrixA->getColumnNumber(); i++){
        for (int j=0; j < matrixA->getTotalNumber(); j++){
            if (matrixA->getElems()[j].getColumn() == i){
                matrixB->insertElem(matrixA->getElems()[j].getColumn(), \
                                    matrixA->getElems()[j].getRow(), \
                                    matrixA->getElems()[j].getData());
            }
        }
    }
}
void FastTranspose(Matrix* A, Matrix* B){
    /*快速转置函数*/
    int column, i, j;
    int *number = new int[A->getColumnNumber()+1], *position = new int[A->getColumnNumber()+1];
    B->setRowNumber(A->getColumnNumber());B->setColumnNumber(A->getRowNumber());
    for (i=0; i<A->getTotalNumber(); i++) B->insertElem(0, 0, 0);
    for (column=1; column<=A->getColumnNumber(); column++) number[column] = 0;
    for (i=0;i<A->getTotalNumber();i++) number[A->getElems()[i].getColumn()]++;
    position[1] = 0;
    for (column=2;column<=A->getColumnNumber();column++)
        position[column] = position[column-1] + number[column-1];
    for (i=0; i<A->getTotalNumber(); i++){
        j = position[A->getElems()[i].getColumn()];
        B->setElem(j, A->getElems()[i].getColumn(), A->getElems()[i].getRow(), \
                   A->getElems()[i].getData());
        position[A->getElems()[i].getColumn()]++;
    }
}
int main()
{
    Matrix A, B, C;
    string inputStr;        //存放用户输入的字符串
    int row, column, data;  //存放用户输入的行数，列数和数据
    bool contin = true;    //用于判断是否继续输入矩阵元素
    do{
        printf("请输入矩阵元素（输入‘0’结束）：");
        getline(cin, inputStr); //获得用户输入的整行数据
        char* inputStr2 = new char[inputStr.length()+1];
        strcpy(inputStr2,inputStr.c_str());
        if (!strcmp(inputStr2, "0")) {
            printf("输入结束！\n");
            contin  = false;
        }
        else{
            row = atoi(strtok(inputStr2, " "));     //按照空格划分用户输入的字符串
            column = atoi(strtok(NULL, " "));
            for (int i=0;i<A.getTotalNumber();i++){
                if ((A.getElems()[i].getRow()==row)&&(A.getElems()[i].getColumn()==column)){
                    printf("对不起，矩阵在该位置已有元素，请重新输入！\n");
                    continue;
                }

            }
            data = atoi(strtok(NULL, " "));
            A.insertElem(row, column, data);
        }
    }while(contin);
    printf("矩阵如下所示：\n");
    A.showMatrix();
    Transpose(&A, &B);
    printf("转置后的矩阵如下所示：\n");
    B.showMatrix();
    FastTranspose(&A, &C);
    printf("快速转置后的矩阵如下所示：\n");
    C.showMatrix();
    return 0;
}
