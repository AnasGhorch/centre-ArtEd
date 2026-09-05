#include "formateurwidget.h"
#include "formateurdialog.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFrame>

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
    // Tri cliquable sur les en-têtes de colonnes
    m_table->setSortingEnabled(true);

    construireFiltres();

    auto *btnFiltres = new QPushButton(QStringLiteral("Recherche / Filtres"));
    auto *btnAjouter = new QPushButton(QStringLiteral("Ajouter"));
    auto *btnModifier = new QPushButton(QStringLiteral("Modifier"));
    auto *btnSupprimer = new QPushButton(QStringLiteral("Supprimer"));
    auto *btnRefresh = new QPushButton(QStringLiteral("Rafraîchir"));

    auto *boutons = new QHBoxLayout;
    boutons->addWidget(btnFiltres);
    boutons->addWidget(btnAjouter);
    boutons->addWidget(btnModifier);
    boutons->addWidget(btnSupprimer);
    boutons->addStretch();
    boutons->addWidget(btnRefresh);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_panneauFiltres);
    layout->addWidget(m_table, 1);
    layout->addLayout(boutons);

    connect(btnFiltres, &QPushButton::clicked, this, &FormateurWidget::basculerPanneauFiltres);
    connect(btnAjouter, &QPushButton::clicked, this, &FormateurWidget::onAjouter);
    connect(btnModifier, &QPushButton::clicked, this, &FormateurWidget::onModifier);
    connect(btnSupprimer, &QPushButton::clicked, this, &FormateurWidget::onSupprimer);
    connect(btnRefresh, &QPushButton::clicked, this, &FormateurWidget::rafraichir);
    connect(m_table, &QTableWidget::doubleClicked, this, &FormateurWidget::onModifier);

    rafraichir();
}

void FormateurWidget::construireFiltres()
{
    m_panneauFiltres = new QFrame;
    m_panneauFiltres->setFrameShape(QFrame::StyledPanel);
    m_panneauFiltres->setVisible(false); // caché jusqu'au clic sur le bouton

    m_filtreNom = new QLineEdit;
    m_filtreNom->setPlaceholderText(QStringLiteral("Nom…"));

    m_filtreDiscipline = new QComboBox;
    m_filtreDiscipline->setEditable(true);
    m_filtreDiscipline->addItem(QStringLiteral("(toutes)"), QString());
    // Valeurs courantes + celles déjà en base seront utiles ; on laisse editable
    m_filtreDiscipline->addItems({
        QStringLiteral("Danse"), QStringLiteral("Musique"), QStringLiteral("Théâtre"),
        QStringLiteral("Peinture"), QStringLiteral("Photographie")
    });

    m_filtreStatut = new QComboBox;
    m_filtreStatut->addItem(QStringLiteral("(tous)"), QString());
    m_filtreStatut->addItem(QStringLiteral("Actif"), QStringLiteral("Actif"));
    m_filtreStatut->addItem(QStringLiteral("Inactif"), QStringLiteral("Inactif"));

    m_filtreExpMin = new QSpinBox;
    m_filtreExpMin->setRange(-1, 50);
    m_filtreExpMin->setSpecialValueText(QStringLiteral("(min)"));
    m_filtreExpMin->setValue(-1); // -1 = pas de filtre

    m_filtreExpMax = new QSpinBox;
    m_filtreExpMax->setRange(-1, 50);
    m_filtreExpMax->setSpecialValueText(QStringLiteral("(max)"));
    m_filtreExpMax->setValue(-1);

    auto *btnAppliquer = new QPushButton(QStringLiteral("Appliquer"));
    auto *btnReset = new QPushButton(QStringLiteral("Réinitialiser"));

    auto *grid = new QGridLayout(m_panneauFiltres);
    grid->addWidget(new QLabel(QStringLiteral("Nom")), 0, 0);
    grid->addWidget(m_filtreNom, 0, 1);
    grid->addWidget(new QLabel(QStringLiteral("Discipline")), 0, 2);
    grid->addWidget(m_filtreDiscipline, 0, 3);
    grid->addWidget(new QLabel(QStringLiteral("Statut")), 1, 0);
    grid->addWidget(m_filtreStatut, 1, 1);
    grid->addWidget(new QLabel(QStringLiteral("Exp. min")), 1, 2);
    grid->addWidget(m_filtreExpMin, 1, 3);
    grid->addWidget(new QLabel(QStringLiteral("Exp. max")), 2, 0);
    grid->addWidget(m_filtreExpMax, 2, 1);
    grid->addWidget(btnAppliquer, 2, 2);
    grid->addWidget(btnReset, 2, 3);

    connect(btnAppliquer, &QPushButton::clicked, this, &FormateurWidget::onRechercher);
    connect(btnReset, &QPushButton::clicked, this, &FormateurWidget::onResetFiltres);
}

void FormateurWidget::basculerPanneauFiltres()
{
    m_panneauFiltres->setVisible(!m_panneauFiltres->isVisible());
}

void FormateurWidget::rafraichir()
{
    // Affiche tout (sans filtres actifs du panneau)
    remplirTableau(m_dao.listerTous());
}

void FormateurWidget::onRechercher()
{
    const QString nom = m_filtreNom->text().trimmed();
    QString discipline = m_filtreDiscipline->currentText().trimmed();
    if (discipline == QStringLiteral("(toutes)"))
        discipline.clear();
    // Si editable et texte = item "(toutes)" via currentData
    if (m_filtreDiscipline->currentIndex() == 0 && m_filtreDiscipline->currentData().toString().isEmpty())
        discipline.clear();

    const QString statut = m_filtreStatut->currentData().toString();
    const int expMin = m_filtreExpMin->value(); // -1 = ignore
    const int expMax = m_filtreExpMax->value();

    remplirTableau(m_dao.filtrer(nom, discipline, statut, expMin, expMax));
}

void FormateurWidget::onResetFiltres()
{
    m_filtreNom->clear();
    m_filtreDiscipline->setCurrentIndex(0);
    m_filtreStatut->setCurrentIndex(0);
    m_filtreExpMin->setValue(-1);
    m_filtreExpMax->setValue(-1);
    rafraichir();
}

void FormateurWidget::remplirTableau(const QList<Formateur> &liste)
{
    // Désactive temporairement le tri pendant le remplissage
    const bool sorting = m_table->isSortingEnabled();
    m_table->setSortingEnabled(false);

    m_table->setRowCount(liste.size());
    for (int row = 0; row < liste.size(); ++row) {
        const Formateur &f = liste[row];

        auto *idItem = new QTableWidgetItem;
        idItem->setData(Qt::DisplayRole, f.idFormateur);
        m_table->setItem(row, 0, idItem);

        m_table->setItem(row, 1, new QTableWidgetItem(f.nom));
        m_table->setItem(row, 2, new QTableWidgetItem(f.prenom));
        m_table->setItem(row, 3, new QTableWidgetItem(f.email));
        m_table->setItem(row, 4, new QTableWidgetItem(f.telephone));
        m_table->setItem(row, 5, new QTableWidgetItem(f.discipline));
        m_table->setItem(row, 6, new QTableWidgetItem(f.dateEmbauche.toString("dd/MM/yyyy")));
        m_table->setItem(row, 7, new QTableWidgetItem(f.statut));

        auto *expItem = new QTableWidgetItem;
        expItem->setData(Qt::DisplayRole, f.anneesExperience);
        m_table->setItem(row, 8, expItem);
    }
    m_table->resizeColumnsToContents();
    m_table->setSortingEnabled(sorting);
}

Formateur FormateurWidget::formateurSelectionne() const
{
    const int row = m_table->currentRow();
    if (row < 0)
        return Formateur();

    Formateur f;
    f.idFormateur = m_table->item(row, 0)->data(Qt::DisplayRole).toInt();
    f.nom = m_table->item(row, 1)->text();
    f.prenom = m_table->item(row, 2)->text();
    f.email = m_table->item(row, 3)->text();
    f.telephone = m_table->item(row, 4)->text();
    f.discipline = m_table->item(row, 5)->text();
    f.dateEmbauche = QDate::fromString(m_table->item(row, 6)->text(), "dd/MM/yyyy");
    f.statut = m_table->item(row, 7)->text();
    f.anneesExperience = m_table->item(row, 8)->data(Qt::DisplayRole).toInt();
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
    emit donneesModifiees();
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
    emit donneesModifiees();
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
    emit donneesModifiees();
}
