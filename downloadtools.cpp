#include "downloadtools.h"
#include "ui_downloadtools.h"

downloadTools::downloadTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::downloadTools)
{
    ui->setupUi(this);
    //设置treeWidget不显示边框和滚动条
    ui->stateTreeWidget->setFrameStyle(QFrame::NoFrame);
    ui->stateTreeWidget->setHeaderHidden(true);
    ui->OpenFolder->setVisible(false);
    ui->DisplayFolder->setVisible(false);
    ui->FileInfoWidget->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);
    ui->Download->setEnabled(false);

    //设置tableWidget不显示边框并初始化表头和列宽
    ui->downloadingTableWidget->setColumnCount(4);
    ui->downloadingTableWidget->setColumnWidth(0, 330);
    ui->downloadingTableWidget->setColumnWidth(1, 100);
    ui->downloadingTableWidget->setColumnWidth(2, 60);
    ui->downloadingTableWidget->setColumnWidth(3, 120);
    //设置列宽不留空
    ui->downloadingTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->downloadingTableWidget->setFrameStyle(QFrame::NoFrame);
    ui->downloadingTableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromLocal8Bit("文件名")<<QString::fromLocal8Bit("速度")<<QString::fromLocal8Bit("进度")<<QString::fromLocal8Bit("已下载大小/总大小"));

    //设置每一行的内容不可编辑
    ui->downloadedTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->downloadedTableWidget->setColumnCount(3);
    ui->downloadedTableWidget->setColumnWidth(0, 380);
    ui->downloadedTableWidget->setColumnWidth(1, 80);
    ui->downloadedTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->downloadedTableWidget->setFrameStyle(QFrame::NoFrame);
    ui->downloadedTableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromLocal8Bit("文件名")<<QString::fromLocal8Bit("文件大小")<<QString::fromLocal8Bit("完成时间"));

    ui->FileInfoWidget->horizontalHeader()->setStretchLastSection(true);
    ui->FileInfoWidget->setColumnWidth(0, 380);
    ui->FileInfoWidget->setColumnWidth(1, 80);
    ui->FileInfoWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromLocal8Bit("文件名")<<QString::fromLocal8Bit("文件类型")<<QString::fromLocal8Bit("文件大小"));

    //设置不显示行号和列名左对齐
    QHeaderView *headerView = ui->FileInfoWidget->verticalHeader();
    headerView->setHidden(true);
    headerView = ui->FileInfoWidget->horizontalHeader();
    headerView->setDefaultAlignment(Qt::AlignLeft);
    headerView = ui->downloadingTableWidget->verticalHeader();
    headerView->setHidden(true);
    headerView = ui->downloadingTableWidget->horizontalHeader();
    headerView->setDefaultAlignment(Qt::AlignLeft);
    headerView = ui->downloadedTableWidget->verticalHeader();
    headerView->setHidden(true);
    headerView = ui->downloadedTableWidget->horizontalHeader();
    headerView->setDefaultAlignment(Qt::AlignLeft);

    this->setStyleSheet("background-color:white;");
    this->show();

    //新建一个QTimer对象，并启动定时器，设置定时器每个1秒发送一个timeout()信号
    timer = new QTimer(this);
    timer->setInterval(1000);
    //QTimer的信号与槽函数
    connect(ui->Download, SIGNAL(clicked()), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

downloadTools::~downloadTools()
{
    timer->stop();
    delete ui;
}

void downloadTools::on_Download_clicked()
{
    //将界面转换到正在下载页
    ui->stackedWidget->setCurrentIndex(2);

    //设置每一行的内容不可编辑
    ui->downloadingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置每一行的内容
    //插入一行
    int index = ui->downloadingTableWidget->rowCount();
    ui->downloadingTableWidget->insertRow(index);

    QTableWidgetItem *itemFileName = new QTableWidgetItem(ui->FileInfoWidget->item(0, 0)->text());
    ui->downloadingTableWidget->setItem(index, 0, itemFileName);
    QTableWidgetItem *itemSpeed = new QTableWidgetItem(QString::fromLocal8Bit("正在连接资源"));
    ui->downloadingTableWidget->setItem(index, 1, itemSpeed);
    QTableWidgetItem *itemProgress  = new QTableWidgetItem("0%");
    ui->downloadingTableWidget->setItem(index, 2, itemProgress);
    QString size = "0//";
    QTableWidgetItem *itemFileSize = new QTableWidgetItem(size+ui->FileInfoWidget->item(0, 2)->text());
    ui->downloadingTableWidget->setItem(index, 3, itemFileSize);

    //开始去下载
    download.back()->StartDownloadFile();

    //开始一次下载后，重新调整开始界面
    ui->URL->clear();
    ui->OpenFolder->setVisible(false);
    ui->DisplayFolder->setVisible(false);
    ui->FileInfoWidget->setVisible(false);
    ui->Download->setEnabled(false);
    ui->FileInfoWidget->setRowCount(0);
    ui->FileInfoWidget->clearContents();
}

void downloadTools::on_stateTreeWidget_clicked(const QModelIndex &index)
{
    int nCurrenIndex = index.row();
    switch (nCurrenIndex)
    {
    case 0:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(2);
        break;
    case 2:
        ui->stackedWidget->setCurrentIndex(3);
        break;
    default:
        break;
    }
}

QString FileSize(size_t fileSize)
{
    QString retSize = "";
    if((fileSize / G) > 0)
    {
        float size = (float)fileSize / G;
        retSize = QString::number(size, 'f', 1);
        retSize += 'G';
    }
    else if((fileSize / M) > 0)
    {
        float size = (float)fileSize / M;
        retSize = QString::number(size, 'f', 1);
        retSize += 'M';
    }
    else if((fileSize / K) > 0)
    {
        float size = (float)fileSize / K;
        retSize = QString::number(size, 'f', 1);
        retSize += 'K';
    }
    else
    {
        float size = fileSize;
        retSize = QString::number(size);
        retSize += 'B';
    }

    return retSize;
}

void downloadTools::on_URL_textChanged()
{
    if(ui->URL->toPlainText().isEmpty())
    {
        ui->OpenFolder->setVisible(false);
        ui->DisplayFolder->setVisible(false);
        ui->FileInfoWidget->setVisible(false);
        ui->Download->setEnabled(false);
    }
    else
    {
        download.append(new Download);
        preSize.append(0);
        //Download tempDownload;
        string url = ui->URL->toPlainText().toStdString();
        string fileName = "";
        fileName = download.back()->GetFileName(url);
        string extensionName = "";
        extensionName = download.back()->GetFileExtensionName(fileName);
        size_t fileSize = download.back()->GetFileTotalLen(url);
        if((fileSize > 0) && (fileName.size() > 0))
        {

            ui->OpenFolder->setVisible(true);
            ui->DisplayFolder->setVisible(true);
            ui->FileInfoWidget->setVisible(true);
            ui->Download->setEnabled(true);
            QStringList desktopPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
            //ui->DisplayFolder->setCurrentIndex(0);
            ui->DisplayFolder->addItem(desktopPath.at(0));

            //插入一行
            int row = ui->FileInfoWidget->rowCount();
            ui->FileInfoWidget->insertRow(row);

            //设置每一行的内容
            QTableWidgetItem *itemFileName = new QTableWidgetItem(QString::fromStdString(fileName));
            ui->FileInfoWidget->setItem(row, 0, itemFileName);
            QTableWidgetItem *itemExtensionName = new QTableWidgetItem(QString::fromStdString(extensionName));
            ui->FileInfoWidget->setItem(row, 1, itemExtensionName);
            QTableWidgetItem *itemFileSize = new QTableWidgetItem(FileSize(fileSize));
            ui->FileInfoWidget->setItem(row, 2, itemFileSize);

            //设置文件类型与文件大小栏不可编辑
            QTableWidgetItem *itemSetFlags = ui->FileInfoWidget->item(row, 1);
            itemSetFlags->setFlags(itemSetFlags->flags() & (~Qt::ItemIsEditable));
            itemSetFlags = ui->FileInfoWidget->item(row, 2);
            itemSetFlags->setFlags(itemSetFlags->flags() & (~Qt::ItemIsEditable));

            QString savePath = ui->DisplayFolder->currentText() + '//' + ui->FileInfoWidget->item(0, 0)->text();
            download.back()->Init(ui->URL->toPlainText().toStdString(), savePath.toStdString());
        }
        else
        {
            ui->URL->clear();
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("链接有误，不能根据链接解析出有效内容！"), QMessageBox::Ok);
        }
    }
}

void downloadTools::on_OpenFolder_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("请选择文件夹..."),"C:/Users/ljx_m/desktop");
    if(filePath.isEmpty())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("选择路径失败，请重新选择！"), QMessageBox::Ok);
    }
    else
    {
        int index  = ui->DisplayFolder->count();
        for(int i = 0; i < index; i++)
        {
            if(strcmp(filePath.toStdString().c_str(), ui->DisplayFolder->itemText(i).toStdString().c_str()) == 0)
            {
                ui->DisplayFolder->setCurrentIndex(i);
                return;
            }
        }
        ui->DisplayFolder->insertItem(index, filePath);
        ui->DisplayFolder->setCurrentIndex(index);
    }
}

void downloadTools::onTimerOut()
{
    if(ui->downloadingTableWidget->rowCount() == 0)
    {
        timer->stop();
    }
    for(int i = 0; i < ui->downloadingTableWidget->rowCount(); i++)
    {
        float progress = (float)((download.at(i))->GetNowBytes()) / (download.at(i))->GetTotalBytes();
        progress *= 100;
        size_t downloaded = (download.at(i))->GetNowBytes() - preSize.at(i);
        preSize.replace(i, (download.at(i))->GetNowBytes());
        QString speed = FileSize(downloaded);
        speed += "/s";

        QTableWidgetItem *itemSpeed  = new QTableWidgetItem(speed);
        ui->downloadingTableWidget->setItem(i, 1, itemSpeed);
        QTableWidgetItem *itemProgress  = new QTableWidgetItem(QString::number(progress, 'f', 1) + '%');
        ui->downloadingTableWidget->setItem(i, 2, itemProgress);
        QString size = FileSize((download.at(i))->GetNowBytes());
        size += "//";
        size += FileSize((download.at(i))->GetTotalBytes());
        QTableWidgetItem *itemSize  = new QTableWidgetItem(size);
        ui->downloadingTableWidget->setItem(i, 3, itemSize);

        if((download.at(i))->GetNowBytes() == (download.at(i))->GetTotalBytes())
        {
            //删除正在下载页的文件信息，转入已完成页

            int index = ui->downloadedTableWidget->rowCount();
            ui->downloadedTableWidget->insertRow(index);

            QTableWidgetItem *itemFileName  = new QTableWidgetItem(ui->downloadingTableWidget->item(i, 0)->text());
            ui->downloadedTableWidget->setItem(index, 0, itemFileName);
            QTableWidgetItem *itemFileTotalSize  = new QTableWidgetItem(FileSize((download.at(i))->GetTotalBytes()));
            ui->downloadedTableWidget->setItem(index, 1, itemFileTotalSize);
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString current = currentDateTime.toString("yyyy-MM-dd hh:mm");
            QTableWidgetItem *itemCurrentDateTime  = new QTableWidgetItem(current);
            ui->downloadedTableWidget->setItem(index, 2, itemCurrentDateTime);

            ui->downloadingTableWidget->removeRow(i);
            download.removeAt(i);
            preSize.removeAt(i);
            --i;
        }
    }
}
