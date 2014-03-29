#include "sortedfile_merge.h"


int main()
{
    SortedFileMerge* sfm = new SortedFileMerge();
    vector<string> inFileNames;
    inFileNames.push_back("a.txt");
    inFileNames.push_back("b.txt");
    sfm->Merge(inFileNames, "c.txt");

    delete sfm;
    return 0;
}
