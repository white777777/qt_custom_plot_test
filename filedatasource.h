#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include <vector>
#include <QString>
#include "interfaces.h"

class FileDataSource: public IDataSource
{
public:
    FileDataSource(const std::string & filename);
    timevalue GetData() const override;
    DataInfo GetDataInfo() const override;
    std::string GetErrInfo() const
    {
        return errStr;
    }
protected:
    void readData(DLimits& limits, std::ifstream& f);

private:
    timevalue _data;
    DataInfo _dataInfo;
    size_t _dataPos;
    std::string errStr;
};


#endif // FILEDATASOURCE_H
