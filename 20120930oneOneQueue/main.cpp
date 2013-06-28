#include <string>
#include <stdio.h>
#include "oneonequeue.h"

using namespace std;

const int MQ_SIZE = 108;

typedef struct msg
{
    string msg;
    int type;
} MSG;

int main()
{
    OneOneQueue<MSG *> mq(101);
    MSG *m = new MSG[MQ_SIZE];
    for (int i=0; i<MQ_SIZE; ++i)
    {
        m[i].type = i;
    }

    for (int i=0; i<MQ_SIZE; ++i)
    {
        int ret = mq.push(&m[i]);
        if (QOP_FULL == ret)
        {
            printf("msg queue is full!\n");
        }
    }

    for (int i=0; i<MQ_SIZE; ++i)
    {
        MSG *temp = mq.pop();
        if (NULL == temp)
        {
            printf("msg queue is empty!\n");
            break;
        }
        printf("%d\t", temp->type);
    }

}
