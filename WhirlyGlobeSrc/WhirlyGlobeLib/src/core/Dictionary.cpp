/*
 *  Dictionary.cpp
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 12/16/13.
 *  Copyright 2011-2013 mousebird consulting
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#import <sstream>
#import "Dictionary.h"

namespace WhirlyKit
{
    
int Dictionary::StringValue::asInt()
{
    std::stringstream convert(val);
    int res;
    if (!(convert >> res))
        res = 0;
    
    return res;
}

double Dictionary::StringValue::asDouble()
{
    std::stringstream convert(val);
    double res;
    if (!(convert >> res))
        res = 0;
    
    return res;
}
    
Dictionary::Dictionary()
{
}
    
Dictionary::Dictionary(const Dictionary &that)
{
    for (FieldMap::const_iterator it = that.fields.begin();it != that.fields.end();++it)
        fields[it->first] = it->second->copy();
}
    
Dictionary::~Dictionary()
{
    clear();
}

void Dictionary::clear()
{
    for (FieldMap::iterator it = fields.begin();it != fields.end();++it)
        delete it->second;
    fields.clear();
}
    
Dictionary &Dictionary::operator = (const Dictionary &that)
{
    clear();
    for (FieldMap::const_iterator it = that.fields.begin();it != that.fields.end();++it)
        fields[it->first] = it->second->copy();
    
    return *this;
}
    
bool Dictionary::hasField(const std::string &name) const
{
    FieldMap::const_iterator it = fields.find(name);
        return (it != fields.end());
}
    
DictionaryType Dictionary::getType(const std::string &name) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return DictTypeNone;
    
    return it->second->type();
}
    
void Dictionary::removeField(const std::string &name)
{
    FieldMap::iterator it = fields.find(name);
    if (it != fields.end())
    {
        delete it->second;
        fields.erase(it);
    }
}
    
int Dictionary::getInt(const std::string &name,int defVal) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return defVal;
    
    return it->second->asInt();
}
    
bool Dictionary::getBool(const std::string &name,bool defVal) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return defVal;
    
    return (bool)it->second->asInt();
}

RGBAColor Dictionary::getColor(const std::string &name,const RGBAColor &defVal) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return defVal;

    switch (it->second->type())
    {
        case DictTypeString:
        {
            std::string str;
            it->second->asString(str);
            // We're looking for a #RRGGBBAA
            if (str.length() < 1 || str[0] != '#')
                return defVal;
            
            int iVal = atoi(&str.c_str()[1]);
            RGBAColor ret;
            ret.b = iVal & 0xFF;
            ret.g = (iVal >> 8) & 0xFF;
            ret.r = (iVal >> 16) & 0xFF;
            ret.a = (iVal >> 24) & 0xFF;
            return ret;
        }
            break;
        case DictTypeInt:
        {
            int iVal = it->second->asInt();
            RGBAColor ret;
            ret.b = iVal & 0xFF;
            ret.g = (iVal >> 8) & 0xFF;
            ret.r = (iVal >> 16) & 0xFF;
            ret.a = (iVal >> 24) & 0xFF;
            return ret;
        }
            break;
        // No idea what this means
        case DictTypeDouble:
        default:
            return defVal;
            break;
    }
    
    return defVal;
}
    
double Dictionary::getDouble(const std::string &name,double defVal) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return defVal;
    
    return it->second->asDouble();
}
    
std::string Dictionary::getString(const std::string &name) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return "";
    
    std::string retStr;
    it->second->asString(retStr);
    return retStr;
}

std::string Dictionary::getString(const std::string &name,const std::string &defVal) const
{
    FieldMap::const_iterator it = fields.find(name);
    if (it == fields.end())
        return defVal;
    
    std::string retStr;
    it->second->asString(retStr);
    return retStr;
}

void Dictionary::setInt(const std::string &name,int val)
{
    removeField(name);
    
    IntValue *iVal = new IntValue();
    iVal->val = val;
    fields[name] = iVal;
}

void Dictionary::setDouble(const std::string &name,double val)
{
    removeField(name);
    
    DoubleValue *dVal = new DoubleValue();
    dVal->val = val;
    fields[name] = dVal;
}

void Dictionary::setString(const std::string &name,const std::string &val)
{
    removeField(name);
    
    StringValue *sVal = new StringValue();
    sVal->val = val;
    fields[name] = sVal;
}
    
}
