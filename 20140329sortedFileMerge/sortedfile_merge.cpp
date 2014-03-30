#include "sortedfile_merge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int SortedFileMerge::Merge(vector<string> inFileNames, const string outFileName, const int op)
{
    const int fileNum = inFileNames.size();
    FILE *fp[fileNum];
    char *line[fileNum];
    size_t len[fileNum];
    int read[fileNum];

    for (int i=0; i<fileNum; i++)
    {
        fp[i] = 0;
        line[i] = 0;
        len[i] = 0;
        read[i] = 0;
    }

    FILE *ofp = fopen(outFileName.c_str(), "w");
    if(ofp==NULL)
    {
        PRINTF("can not open out file[%s]\n", outFileName.c_str());
        return -1;
    }
    
    for (int i=0; i<fileNum; i++)
    {
        fp[i]=fopen(inFileNames[i].c_str(), "r");
        if (fp[i]==NULL)
        {
            PRINTF("can not open in file[%s]\n", inFileNames[i].c_str());
            for (int j=i-1; j>=0; --j)
            {
                fclose(fp[j]);
            }
            fclose(ofp);
            return -2;
        }
    }

   
    int closeFileNum = 0;
    vector<int> minIndex;
    char lastWrite[512] = {0};
    for (int i=0; i<fileNum; i++)
    {
        minIndex.push_back(i);
    }

    do
    {
        for (int i=0; i<(int)minIndex.size(); i++)
        {
            int index = minIndex[i];
            if (fp[index] == NULL) continue;
            if ( (read[index]=getline(&line[index], &len[index], fp[index]))!=-1 )
            {
                PRINTF("load line [%s] from file[%s]\n", line[index], inFileNames[index].c_str());
            }
            else
            {
                PRINTF("close in file[%s]\n", inFileNames[index].c_str());
                fclose(fp[index]);
                fp[index] = NULL;
                free(line[index]);
                line[index] = NULL;
                ++closeFileNum;
            }
        }
 
        char* min = NULL;
        minIndex.clear();
        for (int i=0; i<fileNum; i++)
        {
            if(line[i] != NULL) 
            {
                min = line[i];
                minIndex.push_back(i);
                break;
            }
        }
        for (int i=minIndex[0]+1; i<fileNum; i++)
        {
            if(line[i]==NULL) continue;
            int cmp = strcmp(min, line[i]);
            if(cmp > 0) 
            {
                min = line[i];
                minIndex.clear();
                minIndex.push_back(i);
            }
            else if(cmp == 0)
            {
                minIndex.push_back(i);
            }
        }
        if(op == OP_OR)
        {
            int slen = (min)?strlen(min):0;
            if(slen>0)
            {
                if(strcmp(min, lastWrite)>0)
                {
                    fwrite(min, slen, 1, ofp);
                    strncpy(lastWrite, min, 512);
                }
                else
                {
                    PRINTF("skip misordered content[%s]\n", min);
                }
            }
        }
        else if(op == OP_AND  &&  (int)minIndex.size() == fileNum)
        {
            int slen = (min)?strlen(min):0;
            if(slen>0)
            {
                if(strcmp(min, lastWrite)>0)
                {
                    PRINTF("write [%s] to outfile \n", min);
                    fwrite(min, slen, 1, ofp);
                    strncpy(lastWrite, min, 512);
                }
                else
                {
                    PRINTF("skip misordered content[%s]\n", min);
                }
            }
        }

    } while(closeFileNum<fileNum);

    fclose(ofp);
    PRINTF("close out file[%s], return now\n", outFileName.c_str());
    return 0;
}
