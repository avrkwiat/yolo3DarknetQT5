#include "mainwindow.h"
#include <QApplication>

#include <QtWidgets>


class DarknetWidget: public QWidget{
    Q_OBJECT
public:
    DarknetWidget(QWidget *parent=nullptr): QWidget(parent)
    {
        QGridLayout *lay = new QGridLayout(this);

        lay->addWidget(new QLabel("Darknet directory:"), 0, 0);
        directory_le = new QLineEdit;
        directory_le->setText("/home/jetson/darknet");
        lay->addWidget(directory_le, 1, 0);
        QPushButton *directory_button = new QPushButton("Select");
        connect(directory_button, &QPushButton::clicked, this, &DarknetWidget::onSelectDirectory);
        lay->addWidget(directory_button, 1, 1);

        lay->addWidget(new QLabel("Command:"), 2, 0);
        command_le = new QLineEdit;
        //big model but slow
        //command_le->setText("darknet detect cfg/yolov3.cfg");
        //fast light model
        command_le->setText("darknet detector demo cfg/coco.data cfg/yolov3-tiny.cfg");

        lay->addWidget(command_le, 3, 0, 1, 2);

        lay->addWidget(new QLabel("weights:"), 4, 0);
        weight_le = new QLineEdit;
        //big model but slow
        //weight_le->setText("yolov3.weights");
        //fast light model
        weight_le->setText("yolov3-tiny.weights");

        lay->addWidget(weight_le, 5, 0);
        QPushButton *weight_button = new QPushButton("Select");
        connect(weight_button, &QPushButton::clicked, this, &DarknetWidget::onSelectWeight);
        lay->addWidget(weight_button, 5, 1);


        lay->addWidget(new QLabel("Input image:"), 6, 0);
        image_le = new QLineEdit;
        //image_le->setText("/home/jetson/data/dog.jpg");
        //HikVision camera IP
        image_le->setText("rtsp://admin:password@192.168.88.65:554/Streaming/channels/1/?transportmode=multicast");
        //IP camera
        //image_le->setText("rtsp://192.168.88.17:554/live/0/Main");

        lay->addWidget(image_le, 7, 0);
        QPushButton *image_button = new QPushButton("Select");
        connect(image_button, &QPushButton::clicked, this, &DarknetWidget::onSelectImage);
        lay->addWidget(image_button, 7, 1);

        run_btn = new QPushButton;
        run_btn->setText("Run");
        connect(run_btn, &QPushButton::clicked, this, &DarknetWidget::runDarket);
        lay->addWidget(run_btn, 8, 0, 1, 2);

        log_te = new QPlainTextEdit;
        log_te->setReadOnly(true);
        lay->addWidget(log_te, 9, 0, 1, 2);

        resize(640, 480);

        process = new QProcess(this);
        process->setProcessChannelMode(QProcess::MergedChannels);
        connect(process, &QProcess::stateChanged, this, &DarknetWidget::onStateChanged);
        connect(process, &QProcess::readyReadStandardOutput, this, &DarknetWidget::onReadyReadStandardOutput);
    }
protected:
    void closeEvent(QCloseEvent *event){
        process->kill();
        QWidget::closeEvent(event);
    }
private Q_SLOTS:
    void onSelectDirectory(){
        QString filename = QFileDialog::getExistingDirectory(this, tr("Open Darknet directory"));
        if(!filename.isEmpty()){
            directory_le->setText(filename);
        }
    }
    void onSelectWeight(){
        QString filename = QFileDialog::getOpenFileName(this, tr("Open weights path"));
        if(!filename.isEmpty()){
            weight_le->setText(filename);
        }
    }
    void onSelectImage(){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("Open image path"),
            QDir::currentPath(),
            tr("Image Files (*.jpg)"));
        if(!filename.isEmpty()){
            image_le->setText(filename);
        }
    }

    void runDarket(){
        log_te->clear();
        process->setWorkingDirectory(directory_le->text());
        QString command = QString("%1 %2 %3 %4")
            .arg(command_le->text())
            .arg(weight_le->text())
            .arg(image_le->text())
            //Uncomment if want only consol log
            //.arg("-dont_show")
            ;
        process->start(command);
    }
    void onReadyReadStandardOutput(){
        log_te->insertPlainText(process->readAllStandardOutput());
        log_te->verticalScrollBar()->setValue(log_te->verticalScrollBar()->maximum());
    }
    void onStateChanged(QProcess::ProcessState state){
        if(state == QProcess::Running){
            run_btn->setEnabled(false);
        }
        else if(state == QProcess::NotRunning){
            run_btn->setEnabled(true);
        }
    }
private:
    QLineEdit *directory_le;
    QLineEdit *command_le;
    QLineEdit *weight_le;
    QLineEdit *image_le;
    QPushButton *run_btn;
    QPlainTextEdit *log_te;

    QProcess *process;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    DarknetWidget w;
    w.show();
    return app.exec();
}

#include "main.moc"
