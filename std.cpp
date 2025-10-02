#include <algorithm>
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

// TODO: Add examples to everything as well as complexity,
// explain difference on inserting vs implacing

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

 TODO: explain differences of the containers

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

/*
Priority_queue
is implemented by default with a vector, but using heap instead
of container, tree structure. by default max-heap meaning max element as root
using:
    std::make_heap, std::push_heap, std::pop_heap
    std::less<t>

Methods:
    push(typename): inserts new element it gets placed in the correct heap
    position auto
    emplace(typename | args): construct element in place
    pop(): remove largest element by default
    @typename top(): access the top element
    @bool empty(): checks whether the
    @int size(): returns number of elements in pq

*/

std::priority_queue<Point> pq;

void methodsPriorityQueue() {
  pq.push(Point(4, 5));
  pq.emplace(4, 5);
  Point t = pq.top();
  bool em = pq.empty();
  int s = pq.size();
}

/*
 TODO: Add links to binary search tree to explain

Set
Stores unique elements in sorted order by default (ascending)
usually as a balanced binary search tree
For operations provides O(log n), insertion, search and deletion
And does not allow duplicates, in the case that a duplicated is inserted nothing
changes
    Supports forward traversal

Is ascending by default using std::less, but can be changes for
example descending using std::greater.

Methods:
    insert(typename): adds new element if not present
    emplace(typename | args): adds in place the element in the set
    @iterator | end() <- if not found find(): searches an element
    @int count(typename): counts instances of an element
    erase(typename | iterator): delete element found
    clear(): deletes all elements
    @bool empty(): check whether the set is empty
    @int size(): returns number of elements
    lower_bound(typename): finds first element >= typename
    upper_bound(typename): finds first element > typename

*/

std::set<Point> set;
void methodsSet() {
  set.insert(Point(4, 5));
  set.emplace(6, 8);
  auto f = set.find(Point(4, 5));
  int c = set.count(Point(4, 5));
  set.erase(Point(4, 5));
  set.clear();
  bool em = set.empty();
  int s = set.size();
  auto lb = set.lower_bound(Point(4, 5));
  auto ub = set.upper_bound(Point(3, 8));
}

/*

 TODO: Add references to balanced BST [red-black tree]

Miltiset
It works like a set but allows duplicated is also order them
ascending and sorted. is implemented as a balanced BST(red-black tree)
And as set operations are also O(log n)

Methods:
    insert(typename): inserts element and in the correct place, but does not
    ignore duplicates like set
    implace(typename | args): inserts element in the correct place but in place
    @iterator | end() find(): returns the first instance of a value or end()
    @int count(): returns the number of times element is present
    @{ fiterator, eiterator } equal_range(typename): returns the iterator to the
    begin and end portion when typename is present

    erase(typename | range [iterators] | iterator): removes element or elements
    in range
    @int size(): amount of elements
    @bool empty(): checks whether the multiset is empty
    @typename lower_bound(typename): returns element >= typename
    @typename upper_bound(typename): return element > typename

*/

std::multiset<int> ms;
void methodsMultiset() {
  ms.insert(4);
  auto v = ms.find(4);
  int c = ms.count(2);
  auto r = ms.equal_range(2);
  ms.erase(r.first, r.second);
  int s = ms.size();
  bool em = ms.empty();
}

/*

 TODO: Link hash tables explanation, and hash containers

Unordered_set
Store unique elements like a normal set but are unordered, and implemented
with a hash tables, meaning elements go into buckets depending on their hash
values, normally buckes look like a std::list or std::forward_list
    elements are stored using a hash function.

Operations:
    Average: O(1)
    Worst: O(n)

Methods:
    insert(typename): inserts element to set ignoring duplicates
    implace(typename | args): inserts element to set but in place
    @iterator | end() find(typename): returns the iterator to the el or end()
    @int count(): returns 1 if element exists, or 0-otherwise
    @int erase(): removes element if found
    @int size(): returns amount of elements inside set
    clear(): removes all elements
    @bool empty(): checks whether the set is empty

    Specific to hash containers:
        @int bucket_count(): returns number of buckets
        @int bucket(typename): returns the bucket index where typename is
        @int load_factor(): average number of elements per bucket
        rehash(n): reserves the n buckets and regenarate the hash table
        reserve(n): reserves space for n elements and regenarate the hash table

Iterators traversal for Unordered_sets are random, meaning is
unpredictable.

*/

std::unordered_set<int> us;
void methodsUnorderedSet() {
  us.insert(4);
  us.emplace(5);
  auto it = us.find(4);
  int c = us.count(4);
  us.erase(4);

  int i = us.bucket(4);
  int cbs = us.bucket_count();
  int ld = us.load_factor();
}

/*

 TODO: add link to hash tables, and unordered_set

Unordered_multiset
Is implemented in the same way as an unordered_set using a hash table
but it allows duplicates

Methods:
    insert(typename): inserts element to set accepting duplicates
    @int count(typename): amount of times typename is present
    @{ fiterator, eiterator } equal_range(typename): returns the iterator to the
    begin and end portion when typename is present
    @iterator | end() find(): returns the first instance of a value or end()

*/

std::unordered_multiset<int> ums;
void methodsUnorderedMultiset() {
  ums.insert(2);
  ums.count(2);
  ums.equal_range(10);
  ums.find(2);
}

/*
 TODO: add reference to red-black trees

Map
container that stores values as key-value pairs ordered by default
with std::less (ascending) and is internally implemented as a
self-balanced red-black tree, with no duplicates

Complexity:
    Operations: O(log n)
    Iteration: O(1)

Methods:
    operator[]: returns element at typename or inserts it if missing
    insert(hint?, { typename, typename }): inserts element into the map
    if is already there it does nothing.
    @typename at(typename): returns element in typename, if found, otherwise
    throws exception

    @typename value | end() find(typename key): if element with key found return
    it otherwise returns end()

    @int count(typename): since keys are unique is either 1 or 0
    erase(typename key | range [fiterator, eiterator] | iterator): removes
    element found

    clear(): traverses the map and delete each pair
*/

std::map<int, std::string> m;
void methodsMap() {
  m.insert({2, "asdd"});
  std::string s = m.at(2);
  auto it = m.find(2);
  int c = m.count(1);
  m.erase(2);
  m.clear();
}

/*
 TODO: reference map explanaition

Multimap
is implemented the same way as map, but allows duplicated and
are placed next to each other

Methods:
    @{ fiterator, eiterator } equal_range(typename key): returns 2 iterators for
    the range of values with typename key

*/

std::multimap<int, std::string> mm;
void methodsMultimap() { ms.equal_range(3); }

/*
Bitset
fixed-size array of unsigned long ints, representing a sequence of bits

Methods:
    set(int i | void): sets all bits to 1, or bit in 1 to 1
    reset(int i | void): resets all bits to 0, or bit in 1 to 0
    flip(int i | void): flips all bits, or bit in i (1 = 0 | 0 = 1)
    @bool test(int i): return i = 1 ? return 1 : return 0
    @bool all(): return true if all bits are 1
    @bool any(): return true if any bit are 1
    @bool none(): return true if all bits are 9
    @int count(): count how many bits are 1
    @int size(): return number of bits
    to_string(): returns bits in a string "00101"
    ulong(): returns bits as a unsigned long
    ullong(): retuns bits as a unsigned long long

*/

std::bitset<8> b;
void methodBitset() {
  b.set();
  b.reset();
  b.flip(2);
  b.test(4);
  b.all();
  b.any();
  b.none();
  int c = b.count();
  int s = b.size();
  b.to_string();
  b.to_ulong();
  b.to_ullong();
}

/*
Find
Simple linear search algorith, implemented with iterators
returns first iterator pointing to the first element found or return
last iterator if nothing found. uses the operator== of the type

Params:
    @iterator first
    @iterator last
    Value | element to find


Find_if
Same as find but passes a predicate to compare elements with instead
of the value to find

Params:
    @iterator first
    @iterator last
    Lambda predicate

*/

void find() {
  std::vector<int> v;
  auto f = std::find(v.begin(), v.end(), 3);
  auto fif = std::find_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
}

/*
Count
uses a linear scan and counts values that are the same as the one passed

Params:
    @iterator first
    @iterator last
    Value | element to find

Count_if
same as count but includes a predicate to validate which elements counts

Params:
    @iterator first
    @iterator last
    Lambda predicate


*/

void count() {
  std::vector<int> v;
  int amount = std::count(v.begin(), v.end(), 5);
  int amountif =
      std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
}

/*
For_each
its a iterator-based loop that allows you to implement a method
on each element

Params:
    @iterator first
    @iterator last
    Function predicate

*/

void for_each() {
  std::vector<int> v;
  std::for_each(v.begin(), v.end(), [](int v) { return v * 2; });
}

/*
Sort
is a utility for containers using iterators and optional
sorting function, implements introsort:
    Quicksort: for average cases
    Heapsort: if the recursion gets too deep in quicksort
    InsertionSort: for small subarrays

params:
    @iterator first
    @iterator last
    function? predicate

*/

void sort() {
  struct Point {
    int x;
    int y;
  };

  std::vector<int> v = {3, 5, 6};
  std::vector<Point> points = {{3, 4}, {6, 7}};
  std::sort(v.begin(), v.end());
  std::sort(points.begin(), points.end(),
            [](const Point &p, const Point &b) { return p.x > b.x; });
}

/*
Stable sort
is implemented as a merge sort variant, the stable part is that
if two elements in the comparison are equal (a < b) && (a > b)
their places are not changed.

params:
    @iterator first
    @iterator last
    function? predicate

*/

void stable_sort() {
  struct P {
    int a;
    std::string n;
  };

  std::vector<P> ps = {{1, "asda"}, {5, "asdwrte"}, {10, "231"}};
  std::stable_sort(ps.begin(), ps.end(), [](P a, P b) { return a.a < b.a; });
}

/*

 TODO: add reference to nth_element

Partial Sort
it only sorts part of a range, is useful when you need the
top-k smallest/largest elements, or just finding the top-k elements

Params:
   @iterator first
   @iterator middle
   @iterator end

*/

void partial_sort() {
  std::vector<int> v = {9, 5, 6, 8, 3, 2, 4, 1};

  // Sorted 5 smallest elements
  std::partial_sort(v.begin(), v.begin() + 5, v.end());

  std::nth_element(v.begin(), v.begin() + 5, v.end());
  std::partial_sort(v.begin(), v.begin() + 5, v.end());
}

/*
Binary search
is a search algoritm implemented of binary search using lower_bound
it returns if the value is present no the value itself

Params:
    @iterator first
    @iterator last
    typename value

    returns @bool of found condition

*/

void binary_search() {
  std::vector<int> v = {9, 5, 6, 8, 3, 2, 4, 1};
  bool f = std::binary_search(v.begin(), v.end(), 6);
}
