#include "sortedfile_merge.h"


int main()
{
    SortedFileMerge* sfm = new SortedFileMerge();
    vector<string> inFileNames;
    inFileNames.push_back("a.txt");
    inFileNames.push_back("b.txt");
    inFileNames.push_back("c.txt");
    sfm->Merge(inFileNames, "out.txt");
    //sfm->Merge(inFileNames, "out.txt", SortedFileMerge::OP_AND);

    delete sfm;
    return 0;
}
