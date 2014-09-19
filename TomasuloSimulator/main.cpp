#include <iostream>
#include <deque>

using namespace std;

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    deque<int> d;
    
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.push_back(4);
    d.push_back(5);
    d.push_back(6);
    
    cout << d[3] << endl;
    cout << d[2] << endl;
    return 0;
}
