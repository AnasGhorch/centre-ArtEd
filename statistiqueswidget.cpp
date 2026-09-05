#include "statistiqueswidget.h"
#include "simplecharts.h"
#include "formateurdao.h"
#include "coursdao.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

StatistiquesWidget::StatistiquesWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *hint = new QLabel(
        QStringLiteral("Statistiques ArtEd — se mettent à jour après chaque ajout / modification / suppression."));
    hint->setWordWrap(true);

    m_pieFormateurs = new PieChartWidget;
    m_pieFormateurs->setTitre(QStringLiteral("Formateurs par discipline"));
    m_pieFormateurs->setStyleSheet(
        QStringLiteral("background:#ffffff; border:1px solid #d0d0d0; border-radius:6px;"));

    m_barCours = new BarChartWidget;
    m_barCours->setTitre(QStringLiteral("Nombre de cours par discipline"));
    m_barCours->setStyleSheet(
        QStringLiteral("background:#ffffff; border:1px solid #d0d0d0; border-radius:6px;"));

    m_barRevenu = new BarChartWidget;
    m_barRevenu->setTitre(QStringLiteral("Revenu total (somme des tarifs) par discipline"));
    m_barRevenu->setStyleSheet(
        QStringLiteral("background:#ffffff; border:1px solid #d0d0d0; border-radius:6px;"));

    auto *btnRefresh = new QPushButton(QStringLiteral("Rafraîchir les graphiques"));
    connect(btnRefresh, &QPushButton::clicked, this, &StatistiquesWidget::rafraichir);

    auto *charts = new QHBoxLayout;
    charts->addWidget(m_pieFormateurs, 1);
    charts->addWidget(m_barCours, 1);
    charts->addWidget(m_barRevenu, 1);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(hint);
    layout->addLayout(charts, 1);
    layout->addWidget(btnRefresh, 0, Qt::AlignRight);

    rafraichir();
}

void StatistiquesWidget::rafraichir()
{
    FormateurDAO fDao;
    CoursDAO cDao;
    m_pieFormateurs->setData(fDao.compterParDiscipline());
    m_barCours->setDataInt(cDao.compterParDiscipline());
    m_barRevenu->setDataDouble(cDao.revenuParDiscipline());
}
