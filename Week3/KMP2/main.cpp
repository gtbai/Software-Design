#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#define MAX_LEN 100
using namespace std;
int next[MAX_LEN+1];
class SString{
    public:
        SString(char pStr[]){
        //串的构造方法
            for (int i=0; i<strlen(pStr); i++) str[i+1] = pStr[i];
            length = strlen(pStr);
        }
        char* getString(void){
        //获得串
            return str;
        }
        int getLength(void){
        //获得串的长度
            return length;
        }
    private:
        int length;             //存储串长度
        char str[MAX_LEN+1];     //存储串
};
void getNext(SString str, int next[]){
    int i=1, j = 0;
    next[i] = 0;
    while (i<str.getLength()){  //当还未处理到串末尾时
        if (j==0 || str.getString()[i] == str.getString()[j]) {i++; j++; next[i]=j;}
        else j=next[j];
    }
}
void KMP(SString S, SString T){
    //利用KMP算法进行模式匹配， 主串为S， 模式串为T
    int i = 1, j = 1;
    vector<int> results;
    while (i<=S.getLength()){
        {
            while (j<= T.getLength()){
                if (j==0 || S.getString()[i] == T.getString()[j]) {i++; j++;}
                else j = next[j];
            }
            if (j>T.getLength()) {
               results.push_back(i-T.getLength());
               j=next[j-1];
               if (i>S.getLength()) break;
               else i--;
            }

        }
    }
    if (!results.size()) printf("没有在主串中匹配到模式串！");
    else{
        printf("在主串中匹配到模式串的位置有：");
        for (int i=0;i<results.size();i++) printf("%d ", results[i]);
    }
    printf("\n");
}
int main()
{
    char str[MAX_LEN+1];
    printf("请输入主串：");
    gets(str);
    SString S(str);
    printf("请输入模式串：");
    gets(str);
    SString T(str);
    getNext(T, next);

    KMP(S, T);
    system("pause");
    return 0;
}
