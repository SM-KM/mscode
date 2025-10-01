#include <array>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <list>
#include <queue>
#include <stack>
#include <vector>

/*
STL Containers
Iterators:
    begin():
    end():
    cbegin():
    cend():
    rbegin():
    rend():
    crbegin():
    crend():

Containers:
Sequence continers:
    Vector, Array, Deque, List, Forward_list

Associative containers:
    Set, Map, Multiset, Multimap

Unordered associative containers:
    Unordered_set, Unordered_map, Unordered_multiset, Unordered_multimap

Container adapters:
    Stack, Queue, Priority_queue

*/

/*
Vector

Methods:
    push_back(typename): adds an element to the end of the vector
    pop_back(): removes the last element
    @int size(): size of the vector
    @bool empty(): is empty the vector
    @typename at(typename): return the value at that position
    @typename front(): returns first element
    @typename back(): returns last element
    insert(iterator, value): inserts the value in the pos passed
    erase(iterator): removes the value in the pos passed
    clear(): removes all the elements
    resize(n): change the size of the vector to n
    assign(amount, value): assigns the amount of times the values
    swap(vector): swaps the values of the 2 vectors, if the sizes
    are different they swap sizes as well.

    emplace_back(typename): like push_back but inplace.
    @int capacity(): returns how many elements the vector can allocate
    shrink_to_fit(): reduces capacity to the match the size
*/

std::vector<int> v = {1, 2, 3};
std::vector<int> vswap = {4, 5, 6};

void methodsVector() {
  v.push_back(2);
  v.pop_back();
  int n = v.size();
  bool empty = v.empty();
  int el = v.at(1);
  int f = v.front();
  int b = v.back();
  v.insert(v.begin() + 1, 2);
  v.erase(v.begin() + 2);
  v.clear();
  v.resize(10);
  v.assign(5, 5);
  v.swap(vswap);
  v.emplace_back(4);
  int c = v.capacity();
  v.shrink_to_fit();
}

/*
Array
Arr like C in cpp
    operator[]: acces elements inside arr

Methods:
    at(i): returns element in position i, with bounds checking
    @typename front(): returns first element
    @typename back(): returns last element
    @*typename data(): returns pointer to the c array under it, first el
    @int size(): returns the size of arr
    @bool empty(): returns whether the arr is empty
    fill(typename): fills arr with typename
    swap: swap contents of 2 arrs, of same typename and size n

*/

std::array<int, 2> arr = {1, 2};
std::array<int, 2> arrswap = {4, 2};

void methodsArray() {
  arr.at(2);
  int f = arr.front();
  int b = arr.back();
  int *p = arr.data();
  int s = arr.size();
  bool em = arr.empty();
  arr.swap(arrswap);
}

/*
Deque
Double ended queue

similar to vector is dynamic, but stores elements in chunks
not in one block as vector.
    - supports fast insertion and removal at the front

Methods:
    push_back(typename): inserts typename at the end of dq
    push_front(typename): inserts typename at the front of dq
    pop_back(): removes element in the back of the dq
    pop_front(): removes element in the front of the dq
    @typename at(i): returns element in i
    swap(dqswap): swaps dq with dqswap
    insert(iterator, value): insert value in the position passed
    erase(iterator): removes element in position passed
    clear(): removes all elements inside dq
    assign(amount, value): it adds count times value to dq
    resize(size): resize dq, with the size passed, if empty new positions
    added it fills them with zeros

*/

std::deque<int> dq = {2, 3, 4};
std::deque<int> dqswap = {2, 3};

void methosDeque() {
  dq.push_back(4);
  dq.push_front(5);
  int el = dq.at(2);
  int f = dq.front();
  int b = dq.back();
  dq.swap(dqswap);
  dq.insert(dq.begin() + 1, 2);
  dq.erase(dq.begin() + 1);
  dq.clear();
  dq.assign(4, 5);
  dq.resize(10);
}

/*
List
Doubled linked list

struct Node {
    T data;
    Node* next;
    Node* before;
}

Methods:
    push_back(typename): adds element to the end
    push_front(typename): adds element to the front
    pop_back(): removes element in the end of list
    pop_front(): removes element in the front of the list
    @typename back(): returns last element in the list
    @typename front(): returns first element in the list
    insert(iterator, value): inserts value at iterator pos
    erase(iterator): remove element in the iterator pos
    @int size(): returns amount of Nodes
    clear(): removes all nodes;
    @bool empty(): returns whether the list is empty
    assign(amount, value): assigns the value amount times to the list

*/

std::list<int> l = {1, 2, 3};
std::list<int> lswap = {5, 6, 7, 8, 9};

void methodsList() {
  l.push_back(5);
  l.push_front(8);
  l.pop_back();
  l.pop_front();
  int f = l.front();
  int b = l.back();
  l.insert(l.begin()++, 2);
  l.erase(l.begin()++);
  int n = l.size();
  l.clear();
  bool em = l.empty();
  l.swap(lswap);
  l.assign(4, 5);
}

/*
Forward_list
Implements a Linked List

struct Node {
    T data;
    Node* next;
}

Methods:
    push_front(typename): adds elemnts at the beginning
    pop_front(): removes element at the start of the list
    insert_after(iterator, typename): adds typename after the position of the
    iterator emplace_back(iterator, typename): construct element in place after
the pos of iterator

    erase_after(iterator): delete element after the pos of iterator
    remove(typename): removes all elements of type typename
    remove_if(predicate): removes elements with a condition
    clear(): clears the list
    @bool empty(): check whether the list is empty
    @iterator before_begin(): special iterator before the first element

    For iterators it only includes forward iterators
*/

std::forward_list<int> fl = {3, 4, 5};

void methodsForwardList() {
  fl.push_front(20);
  fl.pop_front();
  fl.insert_after(fl.begin(), 2);
  fl.erase_after(fl.begin());
  fl.remove(3);
  fl.remove_if([](int x) { return x % 2 == 0; });
  fl.clear();
  bool em = fl.empty();
  fl.insert_after(fl.before_begin(), 2);
}

/*
Stack
is a deque underneath but can be implemented with a vector or a list
is LIFO (Last-in, First-out) inforced

Methods:
    push(typename): adds typename to the top of the stack
    emplace(typename): adds to the top but without creating a copy first, in
    place

    pop(): removes the first element in the stack
    @typename top(): access the top element on the stack
    @bool empty(): check whether the stack is empty
    @int size(): returns amount of elements in the stack

*/

struct Point {
public:
  int x;
  int y;

  Point(int x, int y) : x(x), y(y) {};
};

std::stack<Point> s = {};

void methodsStack() {
  s.push(Point(2, 3));
  s.emplace(2, 3);
  s.pop();
  bool em = s.empty();
  int size = s.size();
}

/*
Queue
Is implemented as well with a deque, but can be implemented using
a list. and inforces FIFO (First-in, First-out)

Methods:
    push(typename): insert at the end of the queue typename
    emplace(typename | args): insert in place arguments | typename
    pop(): remove element from the front of the list
    @typename front(): accesses the first element of queue
    @typename back(): accesses the last element of queue
    @bool empty(): check whether the queue is empty
    @int size(): returns the amount of elements in queue

*/

std::queue<Point> q = {};

void methodsQueue() {
  q.push(Point(2, 3));
  q.emplace(5, 7);
  q.pop();
  Point f = q.front();
  Point b = q.back();
  bool em = q.empty();
  int s = q.size();
}
