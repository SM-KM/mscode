/*
Exception:
    logic_error
        invalid_argument
        domain_error
        length_error
        out_of_range

    runtime_error
        range_error
        overflow_error
        underflow_error

    bad_alloc
    bad_function_call
    bad_cast

*/

#include <functional>
#include <new>
#include <stdexcept>
#include <typeinfo>

/*
This errors is used to find branches or parts of code
that if it gets or in the moment that it shoudlnt find the
error in the logic

*/

std::logic_error logic_error = std::logic_error("");
std::domain_error domain_error = std::domain_error("");
std::length_error length_error = std::length_error("");
std::out_of_range out_of_range = std::out_of_range("");

/*
This errors is used to specify errors that occur during runtime
*/

std::runtime_error runtime_err = std::runtime_error("");
std::overflow_error overflow_err = std::overflow_error("");
std::underflow_error underflow_err = std::underflow_error("");

// Errors allocating memory
std::bad_alloc bad_alloc = std::bad_alloc();

// Errors when calling function
std::bad_function_call bad_function_call = std::bad_function_call();

// Errors not executing a bad cast
std::bad_cast bad_cast = std::bad_cast();
