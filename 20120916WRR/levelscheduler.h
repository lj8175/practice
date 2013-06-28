#include <cstddef>
class LevelScheduler
{
public:
    LevelScheduler();
    virtual ~LevelScheduler();
    int saveServer(int *arr, size_t size);
    int allocServer();

private:
    int *m_alloc_times;
    int *m_can_alloc;
    int m_size;
    int m_min_index;
    double m_all;
};
