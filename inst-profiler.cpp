/* 28 Jan 2022
   Instrumentation profiling Example
*/
#include <iostream>
#include <chrono>
#include <vector>
#include <string>

using namespace std;

class ScopedTimer
{
    public:
    using ClockType = std::chrono::steady_clock; // there is system_clock() and high_precision_clock() also but stady_clock() is recommended for calculating time duration
    ScopedTimer(const char* func) : _funcname{func}, _start{ClockType::now()} { }
    // delete copy and move semantics
    ScopedTimer(const char&) = delete;
    ScopedTimer(char&&) = delete;
    ScopedTimer& operator=(const char&) = delete;
    ScopedTimer& operator=(char&&) = delete;
    ~ScopedTimer()
    {
        auto stop = ClockType::now();
        /* std::chrono::duration
           template <class Rep, class Period = ratio<1> >
           class duration;

            Duration
            A duration object expresses a time span by means of a count and a period.
            Internally, the object stores the count as an object of member type rep (an alias of the first template parameter, Rep), 
            which can be retrieved by calling member function count.
        */

        auto duration = (stop - _start); 
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << ms << " ms " << _funcname <<  '\n';
    }

    private:
        const char* _funcname;
        // std::chrono::steady_clock::time_point  -->  Class template std::chrono::time_point represents a point in time. 
        // It is implemented as if it stores a value of type Duration indicating the time interval from the start of the Clock's epoch
        ClockType::time_point _start{};   
};

class Grid
{
public:
    Grid(int id = 0, int pid = 0, float x = 0.f, float y = 0.f, float z = 0.f) : _id{id}, _pid{pid}
    { 
        _xyz[0] = x;
        _xyz[1] = y;
        _xyz[2] = z;
    } 
   
private:
    int _id{}, _pid{};
    float _xyz[3];
};

int vec_alloc(int n)
{
    ScopedTimer tmr("vec_alloc");
    std::vector<Grid> v;
    for(int i = 0; i < n; i++)
        v.emplace_back(Grid(3, 4, 0.5f, 0.6f, 0.7f));
}

int vec_alloc_by_reserve(int n)
{
    ScopedTimer tmr("vec_alloc_by_reserve");
    std::vector<Grid> v;
    v.reserve(n);
    for(int i = 0; i < n; i++)
        v.emplace_back(Grid(3, 4, 0.5f, 0.6f, 0.7f));
}

int main()
{
    std::cout << "Enter a number\n"; 
    int n = 0;
    cin >> n;
    vec_alloc(n);
    vec_alloc_by_reserve(n);
    return 0;
}

/*
   For inserting integer values
   [thapa@gns101 C++]$ ./a.out 
Enter a number
100
0 ms vec_alloc
0 ms vec_alloc_by_reserve
[thapa@gns101 C++]$ ./a.out 
Enter a number
1000
0 ms vec_alloc
0 ms vec_alloc_by_reserve
[thapa@gns101 C++]$ ./a.out 
Enter a number
10000
1 ms vec_alloc
0 ms vec_alloc_by_reserve
[thapa@gns101 C++]$ ./a.out 
Enter a number
100000
6 ms vec_alloc
2 ms vec_alloc_by_reserve
Enter a number
1000000
55 ms vec_alloc
21 ms vec_alloc_by_reserve

For Grid Class
100
0 ms vec_alloc
0 ms vec_alloc_by_reserve
[thapa@gns101 C++]$ ./a.out 
Enter a number
1000
0 ms vec_alloc
0 ms vec_alloc_by_reserve
[thapa@gns101 C++]$ ./a.out 
Enter a number
10000
3 ms vec_alloc
1 ms vec_alloc_by_reserve

*/