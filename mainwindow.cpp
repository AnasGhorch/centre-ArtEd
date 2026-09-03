#include "mainwindow.h"
#include "formateurwidget.h"
#include "courswidget.h"

#include <QTabWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Centre de formation artistique"));
    resize(950, 550);

    auto *onglets = new QTabWidget(this);
    m_formateurWidget = new FormateurWidget;
    onglets->addTab(m_formateurWidget, QStringLiteral("Formateurs"));
    onglets->addTab(new CoursWidget, QStringLiteral("Cours"));

    setCentralWidget(onglets);
    statusBar()->showMessage(QStringLiteral("Connecté à Oracle (DSN XE_SYSTEM)"));
}
