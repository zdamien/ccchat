// basic test code.  hardly exhaustive, just makes sure it works.
#include <string>
#include <iostream>
#include "syn_queue.h"

using namespace std;

syn_queue<string> sq;
queue<string> q;

// this will cause core dumps at times, though no more interesting
// errors.
void f2() {
    q.push("hello");
    q.push("world");
    std::string s;
    s = q.front(); q.pop();
    cout << s << " ";
    q.push("zorses for horses");
    s = q.front(); q.pop();
    cout << s << " ";
    s = q.front(); q.pop();
    cout << s << " ";
    cout << "\n";
}


// actually uses syn_queue
void f1() {
    sq.put("hello");
    sq.put("world");
    std::string s;
    sq.get(s);
    cout << s << " ";
    sq.put("zorses for horses");
    sq.get(s);
    cout << s << " ";
    sq.get(s);
    cout << s << " ";
    cout << "\n";
}

int main() {
    thread t1(f1);
    thread t2(f1);
    thread t3(f1);
    thread t4(f1);
    /*
    thread t1(f2);
    thread t2(f2);
    thread t3(f2);
    thread t4(f2);
    */
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
