#ifndef CREATECONFIGURE
#define CREATECONFIGURE
/*该文件依赖windows平台*/
#include "functions/common.h"
#include <QString>

class ConfigureDirTool
{
public:
    ConfigureDirTool(QString organName, QString appName );
    ~ConfigureDirTool();
    void createIniFile();
    bool makeDstDir();
private:
    QString organization;
    QString application;
};

#endif // CREATECONFIGURE

