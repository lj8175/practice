#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>
#include "timer.h"
#include "tvectorjce/tvector.h"
#include "tmapjce/tmap.h"

#define NUM_MEMBERS 100
#define NUM_FINDERS 10000000
#define BUF_LEN 10240

using namespace std;
using namespace TEST_JEC_VECTOR;
using namespace TEST_JEC_MAP;

int main()
{
    vector<int> ivec;
    set<int> iset;
    map<int, int> imap;
    unordered_set<int> ihset;
    unordered_map<int, int> ihmap;
    tvec itvec;
    tmap itmap;
    for(int i=0; i<NUM_MEMBERS; i++)
    {
        ivec.push_back(i);
        iset.insert(i);
        ihset.insert(i);
        imap[i] = i;
        ihmap[i] = i;
        itvec.ivec.push_back(i);
        itmap.imap[i]=i;
    }

    Timer t;
    for(int i=0; i<NUM_FINDERS; i++)
    {
        int num = random()%NUM_MEMBERS;
        for(vector<int>::iterator it=ivec.begin(); it!=ivec.end(); ++it)
        {
            if(*it == num) break;
        }
    }
    cout<<t.timeDiff()<<endl; //968758us

    for(int i=0; i<NUM_FINDERS; i++)
    {
        iset.find(random()%NUM_MEMBERS);
    }
    cout<<t.timeDiff()<<endl; //790022us

    for(int i=0; i<NUM_FINDERS; i++)
    {
        ihset.find(random()%NUM_MEMBERS);
    }
    cout<<t.timeDiff()<<endl; //268959us

    for(int i=0; i<NUM_FINDERS; i++)
    {
        imap.find(random()%NUM_MEMBERS);
    }
    cout<<t.timeDiff()<<endl; //769960us

    for(int i=0; i<NUM_FINDERS; i++)
    {
        ihmap.find(random()%NUM_MEMBERS);
    }
    cout<<t.timeDiff()<<endl; //274789us

    char buf[BUF_LEN];
    int bufLen = BUF_LEN;
    pdu_protocol_header header;
    int ret = struct_tvec_pack(&itvec, (uint8_t *)buf, &bufLen, &header);
    cout << "pdu_protocol_header len:"<< sizeof(pdu_protocol_header) <<"  header bufLen:" << bufLen << endl; //23  227
    ret = struct_tvec_pack(&itvec, (uint8_t *)buf, &bufLen, NULL);
    cout << "bufLen:" << bufLen << endl; //202
    for(int i=0; i<NUM_FINDERS; i++)
    {
        tvec itvec2;
        ret = struct_tvec_unpack((uint8_t *)buf, &bufLen, &itvec2, NULL);
        int num = random()%NUM_MEMBERS;
        for(vector<int>::iterator it=itvec2.ivec.begin(); it!=itvec.ivec.end(); ++it)
        {
            if(*it == num) break;
        }
    }
    cout<<t.timeDiff()<<endl; //19335062 us

    bufLen = BUF_LEN;
    ret = struct_tmap_pack(&itmap, (uint8_t *)buf, &bufLen, NULL);
    //cout << bufLen << endl; //401
    for(int i=0; i<NUM_FINDERS; i++)
    {
        tmap itmap2;
        ret = struct_tmap_unpack((uint8_t *)buf, &bufLen, &itmap2, NULL);
        itmap2.imap.find(random()%NUM_MEMBERS);
    }
    cout<<t.timeDiff()<<endl; //213722922us

    
}
