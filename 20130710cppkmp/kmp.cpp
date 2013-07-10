#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<int> getNext(string str)
{
    vector<int> v;
    int i=0, j=-1;
    v.push_back(j);
    while (i < str.length())
    {
        if ((j == -1) || (str[i] == str[j]))
        {
            ++i;
            ++j;
            v.push_back(j);
        } 
        else
        {
            j = v[j];
        }
        
    }
    return v;

}

int main()
{
    vector<int> v = getNext(string("abaabcac"));
    for (int i=0; i<v.size(); ++i)
    {
        cout << v[i] << endl;
    }
}
