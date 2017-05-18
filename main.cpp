#include "windows/datamigration.h"
#include "functions/common.h"
#include "functions/createconfigure.h"
#include <QCompleter>
#include "tools/utils.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QTextCodec>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*加载国际化资源文件*/
    QTranslator translator;
    translator.load(":/lang/chinese");
    app.installTranslator(&translator);

    /*创建配置文件和目标路径*/
    ConfigureDirTool sysConf("NFSChina", "DataMigration");
    sysConf.createIniFile();
    sysConf.makeDstDir();

    /*进入运行主窗体*/
    DataMigration w;
    w.show();
    return app.exec();
}
