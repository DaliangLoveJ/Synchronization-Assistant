#include "userdata.h"

UserData::UserData(QObject *parent) : QObject(parent)
{

}

void UserData::UserDataCopyToPath()
{
    bool copyFlag=false;
    QString singleFile;
    int j=0;
    int cou=CopyList.count();
    int ov=0;
    for(int i=0;i<CopyList.count();i++)
    {
        singleFile=CopyList.at(i);
        QFileInfo singleFileInfo(singleFile);
        QString singleFileName = singleFileInfo.fileName();
        QString DstPath=CopyToDst[CopyList.at(i)];
        DstPath.append(singleFileName);
        emit UserData_current_copy_name(current_index,singleFileName);
        copyFlag = Util::copyFileTo(singleFile, DstPath, true);
        if(!copyFlag)
        {
            emit UserData_error_name(current_index,singleFileName);
            j+=1;
        }
        else
        {
            emit UserData_over_name(current_index,singleFileName);
        }
        DstPath.remove(singleFileName);
        ov++;
        emit UserData_progress_num(current_index,cou,ov);
    }
    emit UserData_copy_num(current_index,j);
}

