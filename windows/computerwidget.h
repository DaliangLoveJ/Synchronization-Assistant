#ifndef COMPUTERWIDGET
#define COMPUTERWIDGET
#include "windows/pushbutton.h"
#include "windows/sysinfolabel.h"
#include "windows/progresslabel.h"
#include <QWidget>
#include <QSettings>
class QLabel;
class QPushButton;
class QRoundProgressBar;

class ComputerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComputerWidget(QWidget *parent = 0);
    ~ComputerWidget();
    void _init(void);
signals:
    void exportOnekey();

public slots:
    void progressBarSlot();

public:
    void setValues(double value);

private:
    SysInfoLabel *compuLabel;
    //ProgressLabel *ballLabel;
    QRoundProgressBar *progressBar;
    QLabel *sysverLabel;
    QPushButton *onebtnExport;
};

#endif // COMPUTERWIDGET

