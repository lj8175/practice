#include <map>
#include <string>
#include <vector>

#ifdef DEBUG
#define PRINTF(x...)
#else
#define PRINTF(x...) printf(x)
#endif

using std::map;
using std::string;
using std::vector;

class SortedFileMerge
{
public:
    enum
    {
        OP_OR = 0,
        OP_AND = 1,
    };
    static int Merge(vector<string> inFileNames, const string outFileName, const int op = OP_OR);

};
