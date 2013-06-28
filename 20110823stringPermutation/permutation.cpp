#include <algorithm>
#include <string>
#include <iostream>

int main() {
    int i=0;
    std::string s("dabc"); 
    std::sort(s.begin(), s.end()); 
    do{
       i++;
       std::cout << s << std::endl; 
    }while(std::next_permutation(s.begin(), s.end())); 

    std::cout << i << std::endl;
} 
