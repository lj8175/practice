#include <vector>

const int SERVER_COUNT = 400;

using std::vector;

typedef struct step_item{
    int step;
    int distance;
    int remain_call;
} STEP_ITEM;

class LClient //local client
{
public:
    LClient();
    virtual ~LClient();
    int saveServer();
    int allocServer();
    bool isOver(){ return !m_rflag; }

private:
    STEP_ITEM *m_step; 
    int *m_loop;
    int m_lsize; //loop size, loop length;
    int m_lindex; //loop index;
    bool m_sflag; //step flag;
    bool m_lflag; //loop flag;
    bool m_rflag; //remain flag;
};
