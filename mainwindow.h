#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FormateurWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    FormateurWidget *m_formateurWidget = nullptr;
};

#endif // MAINWINDOW_H
