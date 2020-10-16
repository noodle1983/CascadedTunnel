#include "ConfigCenter.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int test0 = g_cfg->get("test.test0", -1);
    cout << "test0:" << test0 << endl;
    int test1 = g_cfg->get("test.test1", -1);
    cout << "test1:" << test1 << endl;
    int notexist = g_cfg->get("notexist", -1);
    cout << "notexist:" << notexist << endl;
    std::string str = g_cfg->get("test.str", "notexist");
    cout << "str:" << str << endl;

    return 0;
}

