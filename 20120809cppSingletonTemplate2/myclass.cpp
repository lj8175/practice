#include<stdio.h>
#include"Singleton.h"

typedef struct node
{
  int value;
  struct node* next;
} Node;

class List : public Singleton<List>
{
public:
  int insert(Node node);
  Node* getHead();
  virtual ~List(){}
private:
  Node head;  
};


int List::insert(Node node){}
Node* List::getHead()
{
  return &(this->head);
}

int main()
{
  new List(); //should new first and donot thread safe
  List *a = List::GetInstancePtr(); 
  List *b = List::GetInstancePtr(); 
  printf("0x%x\n", (unsigned int)a->getHead());
  printf("0x%x\n", (unsigned int)b->getHead());
  new List();
  a = List::GetInstancePtr(); 
  b = List::GetInstancePtr(); 
  printf("0x%x\n", (unsigned int)a->getHead());
  printf("0x%x\n", (unsigned int)b->getHead());
}

