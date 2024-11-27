cimport cython
from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdio cimport printf

cdef extern from "ctrewrapper.h" nogil :
    ctypedef struct mydate:
        string year;
        string month;
        string day;
    ctypedef mydate date 
    vector[string] extract_number_and_letter(const string& mystring)
    date extract_date (const string& mystring)
    string extract_number (const string& mystring)

cdef int functioncalltest(string strtocheck):
    cdef:
        string result0
        vector[string] result1
        date result2
    result0=extract_number(strtocheck);
    result1=extract_number_and_letter(strtocheck);
    result2=extract_date(strtocheck);
    printf("result0: %s\n", result0.c_str())
    if result1.size()==2:
        printf("result1: %s\t%s\n", result1[0].c_str(), result1[1].c_str())
    printf("result2: %s\t%s\t%s\n", result2.year.c_str(), result2.month.c_str(), result2.day.c_str())
    return 1

cdef:
    string teststring = <string>b"123345"
    string teststring2 = <string>b"1B"
    string datestring = <string>b"here is a date 2021/01/01 somewhere"
    string datestring2 = <string>b"here is NO date anywhere"
    int t1=functioncalltest(teststring)
    int t2=functioncalltest(teststring2)
    int t3=functioncalltest(datestring)
    int t4=functioncalltest(datestring2)


