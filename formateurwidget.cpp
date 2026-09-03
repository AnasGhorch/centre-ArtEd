#include "formateurwidget.h"
#include "formateurdialog.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QAbstractItemView>

FormateurWidget::FormateurWidget(QWidget *parent)
    : QWidget(parent)
{
    m_table = new QTableWidget;
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({
        QStringLiteral("ID"), QStringLiteral("Nom"), QStringLiteral("Prénom"),
        QStringLiteral("Email"), QStringLiteral("Téléphone"), QStringLiteral("Discipline"),
        QStringLiteral("Embauche"), QStringLiteral("Statut"), QStringLiteral("Exp. (ans)")
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setAlternatingRowColors(true);

    auto *btnAjouter = new QPushButton(QStringLiteral("Ajouter"));
    auto *btnModifier = new QPushButton(QStringLiteral("Modifier"));
    auto *btnSupprimer = new QPushButton(QStringLiteral("Supprimer"));
    auto *btnRefresh = new QPushButton(QStringLiteral("Rafraîchir"));

    auto *boutons = new QHBoxLayout;
    boutons->addWidget(btnAjouter);
    boutons->addWidget(btnModifier);
    boutons->addWidget(btnSupprimer);
    boutons->addStretch();
    boutons->addWidget(btnRefresh);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_table, 1);
    layout->addLayout(boutons);

    connect(btnAjouter, &QPushButton::clicked, this, &FormateurWidget::onAjouter);
    connect(btnModifier, &QPushButton::clicked, this, &FormateurWidget::onModifier);
    connect(btnSupprimer, &QPushButton::clicked, this, &FormateurWidget::onSupprimer);
    connect(btnRefresh, &QPushButton::clicked, this, &FormateurWidget::rafraichir);
    connect(m_table, &QTableWidget::doubleClicked, this, &FormateurWidget::onModifier);

    rafraichir();
}

void FormateurWidget::rafraichir()
{
    remplirTableau(m_dao.listerTous());
}

void FormateurWidget::remplirTableau(const QList<Formateur> &liste)
{
    m_table->setRowCount(liste.size());
    for (int row = 0; row < liste.size(); ++row) {
        const Formateur &f = liste[row];
        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(f.idFormateur)));
        m_table->setItem(row, 1, new QTableWidgetItem(f.nom));
        m_table->setItem(row, 2, new QTableWidgetItem(f.prenom));
        m_table->setItem(row, 3, new QTableWidgetItem(f.email));
        m_table->setItem(row, 4, new QTableWidgetItem(f.telephone));
        m_table->setItem(row, 5, new QTableWidgetItem(f.discipline));
        m_table->setItem(row, 6, new QTableWidgetItem(f.dateEmbauche.toString("dd/MM/yyyy")));
        m_table->setItem(row, 7, new QTableWidgetItem(f.statut));
        m_table->setItem(row, 8, new QTableWidgetItem(QString::number(f.anneesExperience)));
    }
    m_table->resizeColumnsToContents();
}

Formateur FormateurWidget::formateurSelectionne() const
{
    const int row = m_table->currentRow();
    if (row < 0)
        return Formateur();

    Formateur f;
    f.idFormateur = m_table->item(row, 0)->text().toInt();
    f.nom = m_table->item(row, 1)->text();
    f.prenom = m_table->item(row, 2)->text();
    f.email = m_table->item(row, 3)->text();
    f.telephone = m_table->item(row, 4)->text();
    f.discipline = m_table->item(row, 5)->text();
    f.dateEmbauche = QDate::fromString(m_table->item(row, 6)->text(), "dd/MM/yyyy");
    f.statut = m_table->item(row, 7)->text();
    f.anneesExperience = m_table->item(row, 8)->text().toInt();
    return f;
}

void FormateurWidget::onAjouter()
{
    FormateurDialog dlg(Formateur(), this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString err;
    if (!m_dao.ajouter(dlg.resultat(), &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}

void FormateurWidget::onModifier()
{
    Formateur f = formateurSelectionne();
    if (f.idFormateur == 0) {
        QMessageBox::information(this, QStringLiteral("Modification"),
                                 QStringLiteral("Sélectionne d'abord un formateur."));
        return;
    }
    FormateurDialog dlg(f, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString err;
    if (!m_dao.modifier(dlg.resultat(), &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}

void FormateurWidget::onSupprimer()
{
    Formateur f = formateurSelectionne();
    if (f.idFormateur == 0) {
        QMessageBox::information(this, QStringLiteral("Suppression"),
                                 QStringLiteral("Sélectionne d'abord un formateur."));
        return;
    }
    if (QMessageBox::question(this, QStringLiteral("Confirmer"),
                              QStringLiteral("Supprimer %1 %2 ?").arg(f.prenom, f.nom))
        != QMessageBox::Yes)
        return;
    QString err;
    if (!m_dao.supprimer(f.idFormateur, &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}
