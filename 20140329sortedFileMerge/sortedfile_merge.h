#include <map>
#include <string>
#include <vector>

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
    int Merge(vector<string> inFileNames, string outFileName, int op = OP_OR);

private:
    map<int, string> m_fdFileMap;
};
