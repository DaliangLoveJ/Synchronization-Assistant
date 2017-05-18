#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QDateTime>

#define  DESTINATION_DIR        "C:\\DataMigration"
#define  DOCUMENT_GROUNP        "DOCUMENT"
#define  PICTURE_GROUNP         "PICTURE"
#define  VIDEO_GROUNP           "VIDEO"
#define  MUSIC_GROUNP           "MUSIC"
#define  DESTINATION_GROUNP     "DESTINATION"
#define  DEFAULT                "DEFAULT"
#define  CUSTOM                 "CUSTOM"


#define CHECK_BOX_COLUMN 0   //文件选中
#define FILE_NAME_COLUMN 1   //文件名
#define DATE_TIME_COLUMN 2   //文件修改日期
#define FILE_SIZE_COLUMN 3   //文件大小
#define FILE_PATH_COLUMN 4   //文件的当前存储路径
#define FILE_COPY_COLUMN 5   //文件拷贝是否完成


typedef struct FileRecord
{
    bool bChecked;
    QString   strFileName;   // 文件名
    QDateTime dateTime;      // 修改日期
    QString   nSize;         // 文件大小
    QString   strFilePath;   // 文件存储路径
    QString copyStart;          // 文件拷贝完成
}fileRecord, *pfileRecord;

#endif // COMMON_H

