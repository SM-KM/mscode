#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

// TODO: Add examples to everything as well as complexities for,
// operations

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

sorts smallest elements in a group

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

/*
Copy
copy elements in the range from first and end iterators passed
into the source that is the begin iterator of some other place
but i must contain enough space to copy
    otherwise theres a buffer overflow

Params:
    @iterator first
    @iterator last
    @iterator begin other container even classes with copy const

*/

void copy() {
  std::vector<int> v = {1, 2, 3, 4, 5};
  std::vector<int> dst(5);

  std::copy(v.begin(), v.end(), dst.begin());
}

/*
Transform
applies a function to each method of a continaer, unary or binary
by pairs, by passing the same iterator first of original container
it can be performed in place.

Params:
    @iterator first
    @iterator last
    @iterator? begin of other container
    @iterator begin of new container
    @function Predicate

*/

void transform() {
  std::vector<int> v = {1, 2, 3, 4, 5};

  // Unary transform
  std::vector<int> dst(v.size());
  std::transform(v.begin(), v.end(), dst.begin(), [](int a) { return a * a; });

  // Binary transform
  std::vector<int> b = {1, 5};
  std::vector<int> dstb(v.size());
  std::transform(v.begin(), v.end(), b.begin(), dst.begin(),
                 [](int x, int y) { return x * y; });

  // In-place transform
  std::transform(v.begin(), v.end(), v.begin(), [](int a) { return a * a; });
}

/*
Replace
it replaces in-place like a find and replace, if you want to keep
the original you can replace_copy, and can be done the replacement
with a predicate.

Params:
    @iterator first
    @iterator end
    @iterator? copy_result iterator container
    @typename | @function old_value
    @typename new_value
*/

void replace() {
  std::vector<int> v = {1, 2, 4, 7, 5, 2, 9, 0};
  std::vector<int> res;

  // Simple replace in-place
  std::replace(v.begin(), v.end(), 2, 20);

  // Replace with copy into res
  std::replace_copy(v.begin(), v.end(), std::back_inserter(res), 2, 10);

  // Replace with preficate
  std::replace_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }, -1);
}

/*
Partition
creates a partition of the elements in the container that match
the predicate function, meaning place them at the beggining
and after that part is the rest that does not match the condition

for presernving order but uses extra memory use stable_partition

Params:
    @iterator first
    @iterator end
    @function predicate

    @returns iterator to first element of unmatching elements

*/

void partition() {
  std::vector<int> v = {2, 3, 4, 6, 7, 3, 5, 2};

  // Partition without preserving order
  std::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

  // Stable partition
  std::stable_partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

  // Example use remove all negative numbers from v
  v.erase(std::partition(v.begin(), v.end(), [](int x) { return x > 0; }),
          v.end());
}

/*
Set Union
conbines two sorted ranges into 1 sorted range without duplicates

Params:
    @iterator firsta
    @iterator lasta
    @iterator firstb
    @iterator lastb
    @iterator firstres
    @compare comparator

*/

void set_union() {
  std::vector<int> a = {2, 5, 7, 3, 1, 2};
  std::vector<int> b = {8, 3, 2, 1};
  std::vector<int> res;

  // They must be sorted or the behavior is undefined
  std::sort(a.begin(), a.end(), std::greater<int>());
  std::sort(b.begin(), b.end(), std::greater<int>());

  std::set_union(a.begin(), a.end(), b.begin(), b.end(), res.begin(),
                 std::greater<int>());
}

/*
Set intersection
conbines same elements in both
two sorted ranges into 1 sorted range without duplicates

Params:
    @iterator firsta
    @iterator lasta
    @iterator firstb
    @iterator lastb
    @iterator firstres
    @compare comparator

*/

void set_intersection() {
  std::vector<int> a = {2, 5, 7, 3, 1, 2};
  std::vector<int> b = {8, 3, 2, 1};
  std::vector<int> res;

  // They must be sorted or the behavior is undefined
  std::sort(a.begin(), a.end(), std::greater<int>());
  std::sort(b.begin(), b.end(), std::greater<int>());

  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), res.begin(),
                        std::greater<int>());
}

/*
Set difference
it returns new range of elements that are in the first range but not
in the second one. they both MUST be sorted


Params:
    @iterator firsta
    @iterator lasta
    @iterator firstb
    @iterator lastb
    @iterator firstres
    @compare comparator


*/

void set_difference() {
  std::vector<int> a = {2, 5, 7, 3, 1, 2};
  std::vector<int> b = {8, 3, 2, 1};
  std::vector<int> res;

  // They must be sorted or the behavior is undefined
  std::sort(a.begin(), a.end(), std::greater<int>());
  std::sort(b.begin(), b.end(), std::greater<int>());

  std::set_difference(a.begin(), a.end(), b.begin(), b.end(), res.begin(),
                      std::greater<int>());
}

/*
Make Heap
it turns a random acces range into a heap

Params:
    @iterator first
    @iterator end

Methods:
    pop_heap(iterator, iterator): move the top element to the end
    push_heap(iterator, iterator): checks and restore the heap property

*/

void make_heap() {
  std::vector<int> v = {2, 4, 7, 4, 1, 8, 3};
  std::make_heap(v.begin(), v.end());

  // Methods
  std::pop_heap(v.begin(), v.end());
  v.pop_back();

  v.push_back(9);
  std::push_heap(v.begin(), v.end());
}

/*
Accumulate
give default value, and default op is addition but can be specified,
or even passed a function.

Params:
    @iterator
    @iterator
    @typename default_val
    @function? accumulate

*/

void accumulate() {
  std::vector<int> v = {2, 3, 6, 1};

  // Example sum of squares
  std::accumulate(v.begin(), v.end(), 0,
                  [](int a, int b) { return a * b + b; });
}

/*
Inner Product
Starting with an initial value of p,
multiplies successive elements from the two ranges
and adds each product into the accumulated value using operator+().
The values in the ranges are processed in order.

Params:
    @iterator firsta
    @iterator enda
    @iterator firstb
    @typename accumulate_val

*/

void inner_product() {
  std::vector<int> v = {2, 3, 6, 1};
  std::vector<int> b = {2, 3, 6, 1};

  std::inner_product(v.begin(), v.end(), b.begin(), 0);
}

/*
Iota
fill elements in range in sequence first = value + 1;

Params:
    @iterator first
    @iterator end
    @typename starting_val

*/

void iota() {
  std::vector<int> v(10);
  std::iota(v.begin(), v.end(), 0);
}

/*
Pair
specific case of a tuple that creates a pair of two elements
can use also make_pair()

Params:
    @typename T1
    @typename T2

*/

void pair() {
  std::pair<uint, uint> pair = std::pair<uint, uint>(3, 4);
  std::cout << pair.first << pair.second;

  std::pair<ulong, ulong> p = std::make_pair(32, 56);
}

/*
Tuple
is a fixed size collection of heterogeneous values

Params:
    @typename...elements

*/

std::tuple<std::string, char, uint> getT(uint id) { return {"Name", 'A', id}; }
void tuple() {
  std::string name;
  char l;
  uint ag;

  std::tie(name, l, ag) = getT(5);

  // Structure bindings C++17
  auto [n, le, a] = getT(4);
}

/*
Optional
contins a value that may or may not be there, to handle exceptions
better and values that are modified constantly

*/

std::optional<std::string> create(bool b) {
  if (b)
    return "God";
  return {};
};

void optional() { std::cout << create(false).value_or("emp"); }

/*
Variant
holds multiple posible values for the same v

*/

void variant() {
  std::variant<uint, void const *> variant;
  std::holds_alternative<void const *>(variant);
}

/*
Any
Holds any type of data

*/

void any() { std::any p = 3; }

/*
Bind
it lets you create a new bindable

Params:
    @callable
    @args...

*/

void bind() {
  using namespace std::placeholders;
  struct subs {
    int mult(int a, int b) const { return a * b; };
    int operator()(int a, int b) const { return a - b; };
  };

  subs sub;
  std::bind(&subs::mult, &sub, _1, _2);
}

/*
Function<typename (typename...)>
it basically stores with type-safe something that can be
called like a function
    - free function
    - lambda
    - functor (function object)
    - member function pointer (std::bind | lambda)

*/

int add(int a, int b) { return a + b; };
void function() {
  // free function
  std::function<int(int, int)> f = add;

  // lambda
  std::function<int(int, int)> flam = [](int a, int b) { return a + b; };

  // functor
  struct subs {
    int mult(int a, int b) const { return a * b; };
    int operator()(int a, int b) const { return a - b; };
  };
  std::function<int(int a, int b)> functor = subs();

  // member function pointer
  subs sub;
  std::function<int(int a, int b)> fmemb = std::bind(
      &subs::mult, &sub, std::placeholders::_1, std::placeholders::_2);

  std::cout << f(4, 5) << flam(5, 8);
}

/*
Initializer_list
is a utility to make posible initalization of containers with {args..}
so instead of doing:

std::vector<int> v;
v.push_back(4);

You can do:

std::vector<int> = { 1, 3, 4, 5 };

*/

void p(std::initializer_list<std::variant<int, std::string>> v) {
  for (auto &e : v) {
    std::visit([](auto &&a) { std::cout << a; }, e);
  };
};

void initializer_list() { p({2, 4, "asads"}); }

/*
Visit
is a util for working with variants, like a type-safe switch
to work with the values

 */

std::variant<uint, bool> av = (uint)5;
std::variant<std::string, long> bv = (uint)5;

// TODO: add in deep example
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

// C++17 later
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// Gets its type at compile time, dont evaluate
decltype(av) decav = av;

void visit() {
  std::visit(
      overloaded{
          [](auto &&x, auto &&y) { std::cout << x << " " << y; },
      },
      av, bv);
};

/*
Move
calls the move constructor without creating a copy and allocating
new memory, so it basically steals it.

the old container or var, becomes unspecified state, meaning it can
be deleted or reassign but the value can not be sure to be there
the only thing it garantees is that it wont crash

*/

void move() {
  std::string s1 = "asda";
  std::string s2 = s1;            // Calls the copy constructor
  std::string s3 = std::move(s1); // Calls the move constructor
}

/*
Forward - used in perfect forwarding
rvalue reference &&

is used to preserve the value category, meaning lvalue or rvalue

*/

void print(int &i) {};  // lvalue
void print(int &&i) {}; // rvalue

template <typename V> void wrapper(V &&v) { print(std::forward<V>(v)); }

void forward() {
  int n = 9;
  wrapper(n);  // lvalue
  wrapper(10); // rvalue
}

// Smart pointers
// handle deletion of objects when out of scope or not needed anymore

/*
unique_ptr
in ensures unique ownership, it cannot be copied only moved
only ensures destruction when out of scope

*/

void unique_ptr() {
  std::unique_ptr<int> ptr = std::make_unique<int>(20);
  std::unique_ptr<int> ptr2 = std::move(ptr);

  // Custom deleters -> when ever file goes out of scope, fclose is called
  std::unique_ptr<FILE, decltype(&fclose)> file(fopen("adas.txt", "r"),
                                                &fclose);
};

/*
shared_ptr
smart pointer that shares ownership, is destroy the object is pointing
to when the last shared_ptr is destroyed, it keeps count of the owners

*/

void shared_ptr() {
  std::shared_ptr<uint> ptr = std::make_shared<uint>(20);
  std::shared_ptr<uint> ptr2 = ptr;
}

/*
weak_ptr
is a smart pointer used with shared_ptr that does not increment
owner count, to prevent loops - cycling references.

*/

void weak_ptr() {
  struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;
  };

  auto n1 = std::make_shared<Node>();
  auto n2 = std::make_shared<Node>();

  n1->next = n2;
  n2->prev = n1; // does not add an extra ref count of owner
                 // avoiding cycling refs
}

/*
Streams
[ source ] -> [ stream ] -> [ destination ]
is like a pipeline to move data

Base Structure
ios_base
    -> basic_ios<CharT, Traits>
        -> basic_istream<CharT, Traits> -> input [std::istream]
        -> basic_ostream<CharT, Traits> -> output [std::ostream]
        -> basic_iostream<CharT, Traits> -> input / output [std::iostream]

std::istream = Pipe bringing data in [input]
std::ostream = Pipe sending data out [output]
std::fstream = Pipe connected to a file [input / output]
std::stringstream = Pipe `

all use <<, >>, get, put

*/

/*
stringstream
works with strings in memory, with a string buffer

std::ostringstream == basic_ostream [Write to string]
std::istringstream == basic_istream [Read from string]
std::iostringstream == basic_iostream [Read and write to same string]

*/

void stringstream() {
  std::ostringstream out;
  out << "Hello " << 123;
  std::string s = out.str();

  std::istringstream in;
  std::string word;
  int num;

  in >> word >> num;
  std::cout << word << " + " << num;
};

/*
File streams
std::ifstream [input - read from file]
std::ofstream [output - write to file]
std::fstream [read and write file]

Flags
std::ios::in = open for reading
std::ios:out = open for writting
std::ios::app = append to file, append to the end
std::ios::trunc = truncate file, clears the file
std::ios::binary = binary mode, ignores \n..\r\n etc, for like images
std::ios::ate

*/

void filestream() {
  std::ofstream out("ex.txt");
  out << "File here\n";
  out.close();
  std::ifstream in("ex.txt");
  std::string line;
  std::getline(in, line);
  std::cout << line << "\n";
  std::fstream file("x.txt", std::ios::in | std::ios::out | std::ios::binary |
                                 std::ios::app | std::ios::trunc);
}

/*
static assert = validates something at compile time
assert = validates something at runtime

*/

void static_ast() {
  assert(sizeof(int) == 4);
  static_assert(sizeof(int) == 4, "Int must be 4 bytes");
}

/*
Decay, Decay_t
transform the type into a form to what it would become if you passed it
to a function

decay_t is just a type alias for convinience to use with decay
typeid = gives you info about a type or expression at runtime
volatile = dont cache or optimize actually read it from memory every time

*/

template <typename T> void showType() {
  std::cout << typeid(typename std::decay<T>::type).name() << "\n";
}

void decay() {
  volatile int flag = 0;
  while (flag == 0) {
  };

  showType<int>();
  showType<const int *>();
  showType<int[5]>();
}

/*
Iomanip header
is used for formatting and manipulation of I/O streams

Floating point
std::fixed = forces to use fixed point notation
std::scientific = forces scientific notation
std::setprecision(n) = sets how many digits after the point are printed
std::showpoint = always use showpoint
std::uppercase = uses scientific letters in uppercase
std::showpos = adds + before positive numbers

Integer
std::dec = prints number in decimal
std::hex = prints number in hexadecimal
std::oct = prints number in octal
std::showbase = shows base of number ex...hex(0x)
std::setw(int n) = sets minimum width of next output field
std::setfill(char c) = sets fill character for unused width spaces
std::left, std::right, std::internal = controls pos within width

Input
std::ws = eats leading whitespace
std::skipws / std::noskipws = enable/disable skipping whitespace

*/

void iomanip() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::showpoint << std::uppercase;
  std::cout << std::showpos;
  std::cout << std::dec << std::hex << std::oct;
  std::cout << std::showbase << std::setw(50);
  std::cout << std::setfill('r') << std::left;
}
