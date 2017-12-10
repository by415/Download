#ifndef DOWNLOADTOOLS_H
#define DOWNLOADTOOLS_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QStandardPaths>
#include <QDateTime>
#include "download.h"

#define G (1024 * 1024 * 1024)
#define M (1024 * 1024)
#define K (1024)

namespace Ui {
class downloadTools;
}

class downloadTools : public QWidget
{
    Q_OBJECT

public:
    explicit downloadTools(QWidget *parent = 0);
    ~downloadTools();

private slots:
    void on_Download_clicked();

    void on_stateTreeWidget_clicked(const QModelIndex &index);

    void on_URL_textChanged();

    void on_OpenFolder_clicked();

    void onTimerOut();

private:
    QList<Download*> download;
    QList<size_t> preSize;
    QTimer* timer;
    Ui::downloadTools *ui;
};

#endif // DOWNLOADTOOLS_H
