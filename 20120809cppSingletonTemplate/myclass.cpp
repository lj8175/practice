#include<stdio.h>
#include"Singleton.h"

typedef struct node
{
  int value;
  struct node* next;
} Node;

class List
{
public:
  static List* Instance();
  int insert(Node node);
  Node* getHead();
  virtual ~List(){}
private:
  Node head;  
};


List* List::Instance()
{
  return CSingleton<List>::Instance();
}
int List::insert(Node node){}
Node* List::getHead()
{
  return &(this->head);
}

int main()
{
  List *a = List::Instance(); 
  List *b = List::Instance(); 
  printf("0x%x\n", (unsigned int)a->getHead());
  printf("0x%x\n", (unsigned int)b->getHead());
}

