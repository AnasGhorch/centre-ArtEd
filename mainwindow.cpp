#include "mainwindow.h"
#include "formateurwidget.h"
#include "courswidget.h"
#include "statistiqueswidget.h"

#include <QTabWidget>
#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("ArtEd — Centre de formation artistique"));
    resize(1100, 650);

    auto *logo = new QLabel;
    QPixmap pix(QStringLiteral(":/resources/arted_logo.png"));
    logo->setPixmap(pix.scaledToHeight(48, Qt::SmoothTransformation));
    logo->setFixedHeight(52);

    auto *titre = new QLabel(QStringLiteral("Centre de formation artistique"));
    titre->setObjectName(QStringLiteral("titreApp"));

    auto *header = new QWidget;
    header->setObjectName(QStringLiteral("headerBar"));
    auto *headerLay = new QHBoxLayout(header);
    headerLay->setContentsMargins(12, 8, 12, 8);
    headerLay->setSpacing(14);
    headerLay->addWidget(logo);
    headerLay->addWidget(titre);
    headerLay->addStretch();

    auto *onglets = new QTabWidget;
    m_formateurWidget = new FormateurWidget;
    m_coursWidget = new CoursWidget;
    m_statsWidget = new StatistiquesWidget;

    onglets->addTab(m_formateurWidget, QStringLiteral("Formateurs"));
    onglets->addTab(m_coursWidget, QStringLiteral("Cours"));
    onglets->addTab(m_statsWidget, QStringLiteral("Statistiques"));

    // Après un CRUD → rafraîchir les graphiques automatiquement
    connect(m_formateurWidget, &FormateurWidget::donneesModifiees,
            m_statsWidget, &StatistiquesWidget::rafraichir);
    connect(m_coursWidget, &CoursWidget::donneesModifiees,
            m_statsWidget, &StatistiquesWidget::rafraichir);

    auto *page = new QWidget;
    auto *pageLay = new QVBoxLayout(page);
    pageLay->setContentsMargins(0, 0, 0, 0);
    pageLay->setSpacing(0);
    pageLay->addWidget(header);
    pageLay->addWidget(onglets, 1);

    setCentralWidget(page);
    statusBar()->showMessage(QStringLiteral("Connecté à Oracle (DSN XE_SYSTEM)"));
}
