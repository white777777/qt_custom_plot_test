#include "filedatasource.h"
#include <fstream>
#include <sstream>
#include <cstdio>
const char COMMENT_LITERAL = '#';

void removeUnnecesary(std::string & result)
{
    if(result.size()>0 && result[0] == COMMENT_LITERAL)
    result = result.substr(1);
    size_t beg = result.find_first_not_of(" \t");
    result = result.substr(beg);
}

bool extractHeaderLine(std::ifstream & f, std::string & result)
{
    if(f.peek() == COMMENT_LITERAL)
    {
        std::getline(f, result);
        removeUnnecesary(result);
        return true;
    }
    return false;
}


void readParams(std::ifstream & f, std::string & params)
{
    std::string line;
    while(f.peek() == COMMENT_LITERAL)
    {
        std::getline(f, line);
        removeUnnecesary(line);
        params += line + "\n";
    }
}


void skipWhiteSpace(std::ifstream& f)
{
    while(!f.eof() && isspace(f.peek()))
        f.get();
}

void FileDataSource::readData(DLimits& limits, std::ifstream& f)
{
    timevalue::value_type val;
    std::string line;
    bool wasThereWhiteSpace = false;
    bool isDataAfterWhiteSpace = false;
    while(!f.eof())
    {
        std::getline(f, line);
        bool whiteSpacesOnly = std::all_of(line.begin(),line.end(),isspace);
        if(whiteSpacesOnly)
        {
            wasThereWhiteSpace = true;
            continue;
        }
        if(wasThereWhiteSpace)
        {
            isDataAfterWhiteSpace = true;
            break;
        }

        char c;
        int res = sscanf(line.c_str(), "%lf %lf [%c]", &val.first, &val.second, &c);
        if(res != 2)
        {
            errStr += "Incorrect data format: expected '<x> <y>' line format\n";
            break;
        }
        if(val.second>limits.yend)
            limits.yend = val.second;
        if(val.second<limits.ybegin)
            limits.ybegin = val.second;
        _data.push_back(val);
    }
    if(isDataAfterWhiteSpace)
        errStr += "Skipped some whitespace inside data lines\n";
}

FileDataSource::FileDataSource(const std::__cxx11::string &filename)
{
    std::ifstream f(filename);
    if(!f.good())
        throw std::runtime_error("Can't open file");

    errStr = "";
    bool isValidHeader = true;
    isValidHeader &= extractHeaderLine(f, _dataInfo.header);
    isValidHeader &= extractHeaderLine(f, _dataInfo.timeStampStart);
    isValidHeader &= extractHeaderLine(f, _dataInfo.measuerType);
    readParams(f, _dataInfo.measureParams);
    if(!isValidHeader)
        errStr += "Invalid header\n";

    DLimits & limits = _dataInfo.limits;
    limits.ybegin = std::numeric_limits<double>::max();
    limits.yend = std::numeric_limits<double>::min();

    skipWhiteSpace(f);

    readData(limits, f);

    limits.xbegin = _data.size()>0 ? _data[0].first : 0.0;
    limits.xend = _data.size()>0 ? _data.back().first : 0.0;
    _dataInfo.nPoints = _data.size();
    f.close();
}

DataInfo FileDataSource::GetDataInfo() const
{
    return _dataInfo;
}

timevalue FileDataSource::GetData() const
{
    return _data;
}
