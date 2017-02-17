#include "filedatasource.h"
#include <fstream>
#include <sstream>

const char COMMENT_LITERAL = '#';

void removeUnnecesary(std::string & result)
{
    result = result.substr(1);
    size_t beg = result.find_first_not_of(" \t");
    result = result.substr(beg);
}

bool extractHeaderLine(std::ifstream & f, std::string & result)
{
    std::getline(f, result);
    if(result.size()>0 && result[0] == COMMENT_LITERAL)
    {
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


FileDataSource::FileDataSource(const std::__cxx11::string &filename)
{
    std::ifstream f(filename);
    if(!f.good())
        throw std::runtime_error("Can't open file");

    extractHeaderLine(f, _dataInfo.header);
    extractHeaderLine(f, _dataInfo.timeStampStart);
    extractHeaderLine(f, _dataInfo.measuerType);
    readParams(f, _dataInfo.measureParams);

    _dataPos = f.tellg();

    DLimits & limits = _dataInfo.limits;
    limits.ybegin = std::numeric_limits<double>::max();
    limits.yend = std::numeric_limits<double>::min();

    f.seekg(_dataPos);
    timevalue::value_type val;
    while(f>>val.first>>val.second)
    {
        if(val.second>limits.yend)
            limits.yend = val.second;
        if(val.second<limits.ybegin)
            limits.ybegin = val.second;
        _data.push_back(val);
    }

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
