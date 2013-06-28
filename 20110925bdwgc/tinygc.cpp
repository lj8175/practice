#include <map>  
#include <list>  
#include <iostream>  
#include <stdlib.h>
#include <string.h>

using namespace std;  
  
class GC  
{  
public:  
    GC();  
    ~GC();  
  
    void Collect();  
    void *Allocate(size_t);  
    void RegisterRoot(void *, size_t);  
  
private:  
    void ScanRegion(size_t start, size_t size);  
    void ScanRoots();  
    void Tidy();  
  
    static size_t GetStackPointer();  
  
private:  
    list< pair<size_t, size_t> > roots;  
    size_t stack_start;  
    map<size_t, size_t> objects;  
    list< pair<size_t, size_t> > scanned_list; //objects in use; 
    list< pair<size_t, size_t> > unscanned_list;  
    list< pair<size_t, size_t> > free_list;  
};  
  
GC::GC() : stack_start(GetStackPointer()){
#ifndef NDEBUG
    cout << hex << "stack_start : 0x" << stack_start << endl;
#endif
}  
  
GC::~GC(){  
    Collect();  
}  
  
void GC::Collect(){  
    ScanRoots();  
    while(!unscanned_list.empty()){  
        pair<size_t, size_t> obj = unscanned_list.front();  
        unscanned_list.pop_front();  
        ScanRegion(obj.first, obj.second);  
    }  
    Tidy();  
}  
  
void GC::ScanRegion(size_t start, size_t size){  
    size_t *p = reinterpret_cast<size_t *>(start);  
    size_t *end = p + size;  
    bool    flag = false;
    while(p < end){  
        map<size_t, size_t>::iterator it = objects.find(*p);  
        if(it != objects.end()){  
            unscanned_list.push_back(*it);  
            scanned_list.push_back(*it);
            objects.erase(it);  
            flag = true;
        }  
        ++p;  
    } 
    if(flag){
        objects.erase(start);
        scanned_list.push_back(make_pair(start, size));
    }
}  
  
void GC::ScanRoots(){  
    list< pair<size_t, size_t> >::iterator p = roots.begin();  
    while(p != roots.end()){  
        ScanRegion(p->first, p->second);  
        ++p;  
    }  
    size_t sp = GetStackPointer();  
    if(sp > stack_start)  
        ScanRegion(stack_start, sp - stack_start);  
    else  
        ScanRegion(sp, stack_start - sp); //传递参数的大小应为(stack_start-sp)/sizeof(size_t)
}  
  
void GC::Tidy(){  
    map<size_t, size_t>::iterator p = objects.begin();  
    while(p != objects.end())  
        free_list.push_back(*p++);  
    objects.clear();  
  
    list<pair<size_t, size_t> >::iterator t = scanned_list.begin();  
    while(t != scanned_list.end())  
        objects.insert(*t++);  
    scanned_list.clear();  
  
#ifndef NDEBUG  
    cout << "live objects: " << (long)objects.size() << endl;  
    cout << "dead objects: " << (long)free_list.size() << endl;  
#endif  
}  
  
void *GC::Allocate(size_t size){  
    if(size == 0)  
        size = 1;  
    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);  
    void *p = malloc(size);  
    if(p != NULL){
        bzero(p, size);
        objects.insert(make_pair(reinterpret_cast<size_t>(p), size / sizeof(size_t)));  
        unscanned_list.push_back(make_pair(reinterpret_cast<size_t>(p), size / sizeof(size_t)));
    }
    return p;  
}  
  
void GC::RegisterRoot(void *p, size_t size){  
    size_t start = reinterpret_cast<size_t>(p) & ~(sizeof(size_t) - 1);  
    size_t end = (reinterpret_cast<size_t>(p) + size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);  
    size = (end - start) / sizeof(size_t);  
    roots.push_back(make_pair(start, size));  
}  
  
size_t GC::GetStackPointer(){  
    volatile size_t value = reinterpret_cast<size_t>(&value);  
    return value;  
}  
  
static GC gc;  
  
inline void *alloc(size_t size){  
    return gc.Allocate(size);  
}  
  
struct Pair{  
    void *first;  
    Pair *second;  
};  
  
void test(){  
    void *p = alloc(sizeof(GC));  
    alloc(sizeof(char));  
    //gc.Collect();  
}  
  
int main()  
{  
    Pair *p = static_cast<Pair *>(alloc(sizeof(Pair)));  
    p->first = alloc(sizeof(int));  
    p->second = (Pair *)alloc(sizeof(Pair));  
    alloc(32);  
    alloc(100);  
    test();  
    //gc.Collect();  
    return 0;  
}  

