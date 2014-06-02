// Copyright (c) 2014 Andranik Abrahamyan

#include "stlfile.h"
#include "stlcompare.h"

StlCompare::StlCompare()
{
}
	
StlCompare::~StlCompare()
{
}





/*
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Node
{
	public:
	 int a;
	 char c;
	 Node *left, *right;
	 
	 Node(){left=right=NULL;}

	 Node(Node *L, Node *R) 
	 {  left =  L;
	    right = R;
	    a = L->a + R->a;  }
};


struct MyCompare
{
    bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};


vector<bool> code;                
map<char,vector<bool> > table;    

void BuildTable(Node *root)
{	
    if (root->left!=NULL) 
                      { code.push_back(0);
                      BuildTable(root->left);}
     
    if (root->right!=NULL)
                       { code.push_back(1);
                       BuildTable(root->right);}
   
    if (root->c) table[root->c]=code;     
    
    code.pop_back();
}


int main (int argc, char *argv[])
{
////// считаем частоты символов	
	
	ifstream f("1.txt");
	
	map<char,int> m;

	while (!f.eof())
	{ char c = f.get(); 
	   m[c]++;}
	
  
////// записываем начальные узлы в список list
	     	     
   list<Node*> t;
   for( map<char,int>::iterator itr=m.begin(); itr!=m.end(); ++itr)
   {  
      Node *p = new Node;
      p->c = itr->first;
      p->a = itr->second;  
      t.push_back(p);      }	
	

//////  создаем дерево		

  while (t.size()!=1)
  {  
     t.sort(MyCompare());
    
     Node *SonL = t.front();
     t.pop_front();
     Node *SonR = t.front(); 
     t.pop_front();
     
     Node *parent = new Node(SonL,SonR); 
     t.push_back(parent);

  }
	
	Node *root = t.front();   //root - указатель на вершину дерева

////// создаем пары 'символ-код':			

   	BuildTable(root);   
		
////// Выводим коды в файл output.txt

    f.clear(); f.seekg(0); // перемещаем указатель снова в начало файла
    
	ofstream g("output.txt", ios::out | ios::binary);
    	
    int count=0; char buf=0;
    while (!f.eof())
    { char c = f.get();
	  vector<bool> x = table[c];
	  for(int n=0; n<x.size(); n++)
	   {buf = buf | x[n]<<(7-count);   
	    count++;   
	    if (count==8) { count=0;   g<<buf; buf=0; } 
       }
    }

    f.close();
	g.close(); 
	
///// считывание из файла output.txt и преобразование обратно

	ifstream F("output.txt", ios::in | ios::binary);

	setlocale(LC_ALL,"Russian"); // чтоб русские символы отображались в командной строке
	
	Node *p = root;
	count=0; char byte; 
	byte = F.get();
	while(!F.eof())
	{   bool b = byte & (1 << (7-count) ) ; 
		if (b) p=p->right; else p=p->left;
		if (p->left==NULL && p->right==NULL) {cout<<p->c; p=root;}
		count++;
		if (count==8) {count=0; byte = F.get();}
	}
	
	F.close();	

	return 0;
}










//Programe For Huffman Code

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAX_TREE_HT 100

struct MH_Node
{
    char character;
    unsigned frequency;
    struct MH_Node *l, *r;
};
     
     
struct M_Heap
{
    unsigned size;
    unsigned space;
    struct MH_Node **array;
};
     
struct MH_Node* newNode(char character, unsigned frequency)
{
    struct MH_Node* temp = (struct MH_Node*) malloc(sizeof(struct MH_Node));
    temp->l = temp->r = NULL;
    temp->character = character;
    temp->frequency = frequency;
    return temp;
}
     
     
struct M_Heap* createM_Heap(unsigned space)
{
    struct M_Heap* M_Heap = (struct M_Heap*) malloc(sizeof(struct M_Heap));
    M_Heap->size = 0;
    M_Heap->space = space;
    M_Heap->array = (struct MH_Node**)malloc(M_Heap->space * sizeof(struct MH_Node*));
    return M_Heap;
}
     
     
void swapMH_Node(struct MH_Node** a, struct MH_Node** b)
{
    struct MH_Node* t = *a;
    *a = *b;
    *b = t;
}
     
     
void M_Heapify(struct M_Heap* M_Heap, int idx)
{
    int smallest = idx;
    int l = 2 * idx + 1;
    int r = 2 * idx + 2;
     
    if (l < M_Heap->size && M_Heap->array[l]->frequency < M_Heap->array[smallest]->frequency)
    smallest = l;
     
    if (r < M_Heap->size && M_Heap->array[r]->frequency < M_Heap->array[smallest]->frequency)
    smallest = r;
     
    if (smallest != idx)
    {
    swapMH_Node(&M_Heap->array[smallest], &M_Heap->array[idx]);
    M_Heapify(M_Heap, smallest);
    }
}
     
int isSizeOne(struct M_Heap* M_Heap)
{
    return (M_Heap->size == 1);
}
     
     
struct MH_Node* extractMin(struct M_Heap* M_Heap)
{
    struct MH_Node* temp = M_Heap->array[0];
    M_Heap->array[0] = M_Heap->array[M_Heap->size - 1];
    --M_Heap->size;
    M_Heapify(M_Heap, 0);
    return temp;
}
     
     
void insertM_Heap(struct M_Heap* M_Heap, struct MH_Node* MH_Node)
{
    int i = M_Heap->size - 1;
    ++M_Heap->size;
    while (i && MH_Node->frequency < M_Heap->array[(i - 1)/2]->frequency)
    {
		M_Heap->array[i] = M_Heap->array[(i - 1)/2];
		i = (i - 1)/2;
    }
    M_Heap->array[i] = MH_Node;
}
     
     
void buildM_Heap(struct M_Heap* M_Heap)
{
    int n = M_Heap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
    M_Heapify(M_Heap, i);
}
     
     
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
    printf("%d", arr[i]);
    printf("\n");
}
     
     
int isLeaf(struct MH_Node* root)
{
	return !(root->l) && !(root->r) ;
}
     
     
struct M_Heap* createAndBuildM_Heap(char character[], int frequency[], int size)
{
    int i;
    struct M_Heap* M_Heap = createM_Heap(size);
    for (i = 0; i < size; ++i)
	    M_Heap->array[i] = newNode(character[i], frequency[i]);
    
	M_Heap->size = size;
    buildM_Heap(M_Heap);
    
	return M_Heap;
}
     

	struct MH_Node* buildHuffmanTree(char character[], int frequency[], int size)
{
    struct MH_Node *l, *r, *top;
    struct M_Heap* M_Heap = createAndBuildM_Heap(character, frequency, size);
     
    while (!isSizeOne(M_Heap))
    {
		l = extractMin(M_Heap);
		r = extractMin(M_Heap);
		top = newNode('$', l->frequency + r->frequency);
		top->l = l;
		top->r = r;
		insertM_Heap(M_Heap, top);
    }
    return extractMin(M_Heap);
}

	
void printCodes(struct MH_Node* root, int arr[], int top)
{
    if (root->l)
    {
		arr[top] = 0;
		printCodes(root->l, arr, top + 1);
    }
    if (root->r)
    {
		arr[top] = 1;
		printCodes(root->r, arr, top + 1);
    }
    if (isLeaf(root))
    {
		printf("%c: ", root->character);
		printArr(arr, top);
    }
}


void HuffmanCodes(char character[], int frequency[], int size)
{
    struct MH_Node* root = buildHuffmanTree(character, frequency, size);
    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}
     
void main()
{
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequency[] = {5, 9, 12, 13, 16, 45};
    int size;
    clrscr();
    size = sizeof(arr)/sizeof(arr[0]);
    HuffmanCodes(arr, frequency, size);
    getch();
}
*/
