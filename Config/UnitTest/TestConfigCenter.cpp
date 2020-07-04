#include "ConfigCenter.h"
#include <iostream>
#include <string>
using namespace std;

void intChange(const int theValue)
{
    cout << "change to value:" << theValue << endl;
}

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

    g_cfg->
        registValueWatcher("test.test0", (void*)0, nd::IntParameter::Watcher(intChange));

    g_cfg->set("test.test0", test0 + 1);
    g_cfg->set("test.test1", test1 + 1);
    g_cfg->set("test.str", "write back");
    g_cfg->saveXml(".saved_config.xml");
    return 0;
}

