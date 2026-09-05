#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FormateurWidget;
class CoursWidget;
class StatistiquesWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    FormateurWidget *m_formateurWidget = nullptr;
    CoursWidget *m_coursWidget = nullptr;
    StatistiquesWidget *m_statsWidget = nullptr;
};

#endif // MAINWINDOW_H
