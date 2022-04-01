//
// Created by Antonio Salsi on 27/01/22.
//

#pragma once
#include <string>
#include <cstdint>
//generic no copy no move constructor
#define HGARDENPI_NO_COPY_NO_MOVE(clazz)      \
    clazz(clazz &) = delete;                  \
    clazz &operator=(const clazz &) = delete; \
    clazz(clazz &&) = delete;                 \
    clazz &operator=(clazz &&) = delete;

namespace hgarden::test
{
using namespace std;

inline constexpr char DB_PATH[] = "./hgarden_test.db";

inline string what(const exception_ptr &exceptionPtr = current_exception())
{
    if (!exceptionPtr) { throw bad_exception(); }

    try { rethrow_exception(exceptionPtr); }
    catch (const exception &e) { return e.what(); }
    catch (const string &e) { return e; }
    catch (const char *e) { return e; }
    catch (...) { return "who knows"; }
}

}