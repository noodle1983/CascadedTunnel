#include "ConfigCenter.h"
#include "XmlGroup.h"
#include "Log.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

using namespace nd;
using namespace rapidxml;
using namespace std;

const std::string ConfigCenter::TOP_XMLNODE_NAME = "Configuration";
//-----------------------------------------------------------------------------

mutex ConfigCenter::configCenterMutexM;
ConfigCenterPtr ConfigCenter::configCenterM;

//-----------------------------------------------------------------------------

ConfigCenterPtr ConfigCenter::instance()
{
    if (NULL == configCenterM.get())
    {
        lock_guard<mutex> lock(configCenterMutexM);
        if (NULL == configCenterM.get())
        {
            configCenterM.reset(new ConfigCenter());
            if (0 == configCenterM->loadXml("config.xml"))
            {
                CFG_DEBUG("loaded config file:config.xml");
            }
        }
    }
    lock_guard<mutex> lock(configCenterMutexM);
    return configCenterM;
}

//-----------------------------------------------------------------------------

int ConfigCenter::loadConfig(const std::string& theInputXmlFile)
{
    ConfigCenterPtr newConfigCenter(new ConfigCenter());
    if (0 == newConfigCenter->loadXml(theInputXmlFile))
    {
        {
            lock_guard<mutex> lock(configCenterMutexM);
            configCenterM = newConfigCenter;
        }
        CFG_DEBUG("loaded config file:" << theInputXmlFile);
        return 0;
        
    }
    else if (NULL == configCenterM.get())
    {
        CFG_ERROR("load xml file failed, the default config will be applied.");
        lock_guard<mutex> lock(configCenterMutexM);
        configCenterM->borrowFrom(newConfigCenter);
        return -1;
    }
    else 
    {
        CFG_ERROR("config center is not changed.");
        return -1;
    }
    return -1;

}

//-----------------------------------------------------------------------------

void ConfigCenter::borrowFrom(ConfigCenterPtr theOtherConfig)
{
    {
        IntParamMap::iterator it1 = intParamMapM.begin();
        IntParamMap::iterator last1 = intParamMapM.end();
        IntParamMap::iterator it2 = theOtherConfig->intParamMapM.begin();
        IntParamMap::iterator last2 = theOtherConfig->intParamMapM.end();
        IntParamMap::key_compare cmp = intParamMapM.key_comp();
        IntParamMap moreIntParams;
        while (it1!=last1 && it2!=last2)
        {
            if (cmp(it1->first, it2->first))
            {
                ++it1;
            }
            else if (cmp(it2->first,it1->first))
            {
                moreIntParams.insert(*it2);
                ++it2;
            }
            else 
            { 
                if (it2->second._getCheckRange())
                {
                    it1->second.setRange(it2->second._getMinValue(), it2->second._getMaxValue());
                }
                it2->second.set(it1->second._getValue());
                it1++; it2++; 
            }
        }
        intParamMapM.insert(moreIntParams.begin(), moreIntParams.end());
    }
    {
        StringParamMap::iterator it1 = strParamMapM.begin();
        StringParamMap::iterator last1 = strParamMapM.end();
        StringParamMap::iterator it2 = theOtherConfig->strParamMapM.begin();
        StringParamMap::iterator last2 = theOtherConfig->strParamMapM.end();
        StringParamMap::key_compare cmp = strParamMapM.key_comp();
        StringParamMap moreStringParams;
        while (it1!=last1 && it2!=last2)
        {
            if (cmp(it1->first, it2->first))
            {
                ++it1;
            }
            else if (cmp(it2->first,it1->first))
            {
                moreStringParams.insert(*it2);
                ++it2;
            }
            else 
            { 
                it2->second.set(it1->second._getValue());
                it1++; it2++; 
            }
        }
        strParamMapM.insert(moreStringParams.begin(), moreStringParams.end());
    }
    *topGroupM = *theOtherConfig->topGroupM;
}

//-----------------------------------------------------------------------------

ConfigCenter::ConfigCenter()
    : topGroupM(NULL)
{
}

//-----------------------------------------------------------------------------

ConfigCenter::~ConfigCenter()
{
    if (topGroupM)
    {
        delete topGroupM;
        topGroupM = NULL;
    }
}

//-----------------------------------------------------------------------------


int ConfigCenter::loadXml(const std::string& theXmlPath)
{
    try
    {
        file<> fdoc(theXmlPath.c_str());  
        xml_document<>  doc;      
        doc.parse<0>(fdoc.data());   

        xml_node<>* root = doc.first_node();  
        if (!root)
        {
            CFG_ERROR("can't load xml file:" << theXmlPath);
            return -1;
        }
        if (root->name() != TOP_XMLNODE_NAME)
        {
            CFG_ERROR("not a xml configuration file:" << theXmlPath);
            return -1;
        }

        XmlGroup* group = new XmlGroup();
        if (0 != group->parse(root, TOP_XMLNODE_NAME))
        {
            CFG_ERROR("can't parse configuration file:" << theXmlPath);
            return -1;
        }

        if (topGroupM)
        {
            delete topGroupM;
            topGroupM = NULL;
        }
        topGroupM = group;
        topGroupM->convertToMap(intParamMapM, strParamMapM);
    }
    catch (std::exception& e)
    {
        CFG_ERROR("can't parse configuration file:" << theXmlPath
                << ". exception:" << e.what());
        return -1;
    }
        


    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::saveXml(const std::string& theXmlPath)
{
    if (!topGroupM || theXmlPath.empty())
    {
        return -1;
    }
    xml_document<> doc;  

    xml_node<>* rot = doc.allocate_node(node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(rot);

    //xml_node<>* node =  doc.allocate_node(node_element, TOP_XMLNODE_NAME.c_str(),NULL);  
    //doc.append_node(node);

    topGroupM->refreshFromMap(intParamMapM, strParamMapM);
    doc.append_node(topGroupM->genNode(&doc, TOP_XMLNODE_NAME));

    std::ofstream out(theXmlPath.c_str());
    out  <<  doc;
    out.close();
    CFG_DEBUG("saved to config file:" << theXmlPath);
    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::get(const std::string& theKey, const int theDefault)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        return it->second.get();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::set(const std::string& theKey, const int theValue)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("can't set value, config not found:" << theKey);
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::setInt(const std::string& theKey, const std::string& theValue)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("can't set value, config not found:" << theKey);
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::registValueWatcher(
        const std::string& theKey, 
        void* theWatcherKey,
        IntWatcher theWatcher)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.registerWatcher(theWatcherKey, theWatcher);
    }
    else
    {
        CFG_ERROR("can't regist watcher, config not found:" << theKey);
    }

}

//-----------------------------------------------------------------------------

void ConfigCenter::unregistIntValueWatcher(
        const std::string& theKey, 
        void* theWatcherKey)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.unregisterWatcher(theWatcherKey);
    }
    else
    {
        CFG_ERROR("can't regist watcher, config not found:" << theKey);
    }

}

//-----------------------------------------------------------------------------

const std::string ConfigCenter::get(const std::string& theKey, const std::string& theDefault)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        return it->second.get();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::set(const std::string& theKey, const std::string& theValue)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("can't regist watcher, config not found:" << theKey);
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::registValueWatcher(
        const std::string& theKey, 
        void* theWatcherKey,
        StringWatcher theWatcher)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        it->second.registerWatcher(theWatcherKey, theWatcher);
    }
    else
    {
        CFG_ERROR("can't regist watcher, config not found:" << theKey);
    }

}

//-----------------------------------------------------------------------------

void ConfigCenter::unregistStrValueWatcher(
        const std::string& theKey, 
        void* theWatcherKey)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        it->second.unregisterWatcher(theWatcherKey);
    }
    else
    {
        CFG_ERROR("can't regist watcher, config not found:" << theKey);
    }

}

