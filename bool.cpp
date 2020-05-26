/*

�������: �� ������������ ������ ����������� ��������� ��������� ������-�������,
������� ��� �� ������ � ����� �������� ���������.

���������: ���������� ���������, ���������� ������ �������� ��������(! - not),
����������(* - and) � ����������(+ - or), ����� ����������� � ���� ������.
���������� ���� � ��������, ������ � 0 ��� 1.

��������: ��� ���������  !(!(M+L)*K)+(!K*!M)+N ������ ����� �����:

           +
        /    \
        +     N
      /   \
     !     *
     |    / \
     *    !  !
    / \   |  |
    !  K  K  M
    |
    +
   / \
  M   L

���� ������ �������� 0/1: !(!(1+0)*1)+(!1*!1)+1

��� ������: !(a+b+c)+!b+!(a+!b+c)*!(!a+b+c)+!a*!b

��������� ����� ������-����� ���������� ��������� ������������ ���:
<�������>  ::= <����> | <����> + <�������>.
<����>     ::= <����> | <����> * <����>.
<����>     ::= 1 | {true}
               0 | {false}
               !<����>|
               (<�������>)

������ �� ����� �������� �������� ���������� ������-������� ��������
�� ���� ����������� ��������, ������������ ��������� �� ������:

t = Form{
    t1 = Term <���������� ������ ��� ������� �����>
    while '+'{
         p - <���������� ���� ��� ����������>
         <t1 ���������� � ����� ���������>
          t1 = Term <���������� ������ ��� �����>
         <t1 ���������� � ������ ���������>
          t1 = p <��� ��������� ������������������ ����������>
    }
    return t1;
}

t = Term{
    t1 = Atom <���������� ������ ��� ������� �����>
    while '*'{
       p - <���������� ���� ��� ����������>
       <t1 ���������� � ����� ���������>
       t1 = Atom <���������� ������ ��� �����>
       <t1 ���������� � ������ ���������>
       t1 = p <��� ��������� ������������������ ����������>
    }
    return t1;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

*/



#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

struct Tree{
    char id;
    struct Tree *l;
    struct Tree *r;
};

Tree *Term(char *str,int &i);
Tree *Atom(char *str,int &i);

/////////////////////////////////////////////////////////////////////
void print(Tree*& head, int n){
    if (head != NULL)
    {
        print(head->l, n+1);
        cout << setw(n*3) << head->id << endl;
        print(head->r, n+1);
    }
}
/////////////////////////////////////////////////////////////////////
Tree *Form(char *str,int &i){
    Tree *t = new Tree;
    t = Term(str,i);
    int n = strlen(str);
    while (i<n && str[i]=='+'){
        Tree *p = new Tree;
        p->id = '+';
        p->l = t;
        p->r = Term(str,++i);
        t = p;
    }
    return t;
}
/////////////////////////////////////////////////////////////////////
Tree *Term(char *str,int &i){
    Tree *t = new Tree;
    t = Atom(str,i);
    int n = strlen(str);
    while(i<n && str[i]=='*'){
        Tree *p = new Tree;
        p->id = '*';
        p->l = t;
        p->r = Atom(str,++i);
        t = p;
    }
    return t;
}
/////////////////////////////////////////////////////////////////////
Tree *Atom(char *str,int &i){
    Tree *t = new Tree;
    t->l = NULL;
    t->r = NULL;
    if(str[i]=='0' || str[i]=='1'){
        t->id = str[i++];
    }
    else if(str[i]=='!'){
        t->id = str[i];
        t->l = Atom(str,++i);
        t->r = NULL;
    }
    else if(str[i]=='('){
        t = Form(str,++i);
        i++;
    }
    while(str[i]==' ') i++;
    return t;
}
/////////////////////////////////////////////////////////////////////
bool Result(Tree *t){
    bool x, y, z;
    if (t != NULL) {
        x = Result(t->l);
        y = Result(t->r);
        if (t->id=='+')
            z = x||y;
        else if(t->id=='*')
            z = x&&y;
        else if( t->id=='!')
            z =! x;
        else if(t->id=='0')
            z = false;
        else if(t->id=='1')
            z = true;
    }
    return z;
}
/////////////////////////////////////////////////////////////////////
int main()
{
    const int size=256;
    char  str[size];
    // �������� ���� �� ������, �� ���������
    ifstream fin("input.txt");
    fin.getline(str,size);
    fin.close();

    int i = 0;
    Tree *tr = Form(str,i);
    print(tr,2);
    int res = Result(tr);
    cout << "\nFormula: "<<str<< "\nResult : "<< res << "\n";
    return 0;
}
