#include "courswidget.h"
#include "coursdialog.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QAbstractItemView>

CoursWidget::CoursWidget(QWidget *parent)
    : QWidget(parent)
{
    m_table = new QTableWidget;
    m_table->setColumnCount(13);
    m_table->setHorizontalHeaderLabels({
        QStringLiteral("ID"), QStringLiteral("Titre"), QStringLiteral("Description"),
        QStringLiteral("Discipline"), QStringLiteral("Niveau"), QStringLiteral("Durée (h)"),
        QStringLiteral("Début"), QStringLiteral("Fin"), QStringLiteral("Tarif"),
        QStringLiteral("Capacité"), QStringLiteral("Salle"), QStringLiteral("Statut"),
        QStringLiteral("Formateur")
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

    connect(btnAjouter, &QPushButton::clicked, this, &CoursWidget::onAjouter);
    connect(btnModifier, &QPushButton::clicked, this, &CoursWidget::onModifier);
    connect(btnSupprimer, &QPushButton::clicked, this, &CoursWidget::onSupprimer);
    connect(btnRefresh, &QPushButton::clicked, this, &CoursWidget::rafraichir);
    connect(m_table, &QTableWidget::doubleClicked, this, &CoursWidget::onModifier);

    rafraichir();
}

void CoursWidget::rafraichir()
{
    remplirTableau(m_dao.listerTous());
}

void CoursWidget::remplirTableau(const QList<Cours> &liste)
{
    m_table->setRowCount(liste.size());
    for (int row = 0; row < liste.size(); ++row) {
        const Cours &c = liste[row];

        auto *idItem = new QTableWidgetItem(QString::number(c.idCours));
        // On stocke id_formateur dans UserRole pour le retrouver au Modifier
        idItem->setData(Qt::UserRole, c.idFormateur);
        m_table->setItem(row, 0, idItem);

        m_table->setItem(row, 1, new QTableWidgetItem(c.titre));
        m_table->setItem(row, 2, new QTableWidgetItem(c.description));
        m_table->setItem(row, 3, new QTableWidgetItem(c.disciplineArtistique));
        m_table->setItem(row, 4, new QTableWidgetItem(c.niveau));
        m_table->setItem(row, 5, new QTableWidgetItem(QString::number(c.dureeHeures)));
        m_table->setItem(row, 6, new QTableWidgetItem(c.dateDebut.toString("dd/MM/yyyy")));
        m_table->setItem(row, 7, new QTableWidgetItem(c.dateFin.toString("dd/MM/yyyy")));
        m_table->setItem(row, 8, new QTableWidgetItem(QString::number(c.tarif, 'f', 2)));
        m_table->setItem(row, 9, new QTableWidgetItem(QString::number(c.capaciteMax)));
        m_table->setItem(row, 10, new QTableWidgetItem(c.salle));
        m_table->setItem(row, 11, new QTableWidgetItem(c.statut));
        m_table->setItem(row, 12, new QTableWidgetItem(c.nomFormateur));
    }
    m_table->resizeColumnsToContents();
}

Cours CoursWidget::coursSelectionne() const
{
    const int row = m_table->currentRow();
    if (row < 0)
        return Cours();

    Cours c;
    c.idCours = m_table->item(row, 0)->text().toInt();
    c.idFormateur = m_table->item(row, 0)->data(Qt::UserRole).toInt();
    c.titre = m_table->item(row, 1)->text();
    c.description = m_table->item(row, 2)->text();
    c.disciplineArtistique = m_table->item(row, 3)->text();
    c.niveau = m_table->item(row, 4)->text();
    c.dureeHeures = m_table->item(row, 5)->text().toDouble();
    c.dateDebut = QDate::fromString(m_table->item(row, 6)->text(), "dd/MM/yyyy");
    c.dateFin = QDate::fromString(m_table->item(row, 7)->text(), "dd/MM/yyyy");
    c.tarif = m_table->item(row, 8)->text().toDouble();
    c.capaciteMax = m_table->item(row, 9)->text().toInt();
    c.salle = m_table->item(row, 10)->text();
    c.statut = m_table->item(row, 11)->text();
    c.nomFormateur = m_table->item(row, 12)->text();
    return c;
}

void CoursWidget::onAjouter()
{
    CoursDialog dlg(Cours(), this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString err;
    if (!m_dao.ajouter(dlg.resultat(), &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}

void CoursWidget::onModifier()
{
    Cours c = coursSelectionne();
    if (c.idCours == 0) {
        QMessageBox::information(this, QStringLiteral("Modification"),
                                 QStringLiteral("Sélectionne d'abord un cours."));
        return;
    }
    CoursDialog dlg(c, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    QString err;
    if (!m_dao.modifier(dlg.resultat(), &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}

void CoursWidget::onSupprimer()
{
    Cours c = coursSelectionne();
    if (c.idCours == 0) {
        QMessageBox::information(this, QStringLiteral("Suppression"),
                                 QStringLiteral("Sélectionne d'abord un cours."));
        return;
    }
    if (QMessageBox::question(this, QStringLiteral("Confirmer"),
                              QStringLiteral("Supprimer le cours « %1 » ?").arg(c.titre))
        != QMessageBox::Yes)
        return;
    QString err;
    if (!m_dao.supprimer(c.idCours, &err)) {
        QMessageBox::warning(this, QStringLiteral("Erreur"), err);
        return;
    }
    rafraichir();
}
