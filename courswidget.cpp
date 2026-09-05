#include "courswidget.h"
#include "coursdialog.h"
#include "courspdf.h"
#include "formateurdao.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFrame>
#include <QCheckBox>

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
    m_table->setSortingEnabled(true);

    construireFiltres();

    auto *btnFiltres = new QPushButton(QStringLiteral("Recherche / Filtres"));
    auto *btnAjouter = new QPushButton(QStringLiteral("Ajouter"));
    auto *btnModifier = new QPushButton(QStringLiteral("Modifier"));
    auto *btnSupprimer = new QPushButton(QStringLiteral("Supprimer"));
    auto *btnPdf = new QPushButton(QStringLiteral("Générer fiche PDF"));
    auto *btnRefresh = new QPushButton(QStringLiteral("Rafraîchir"));

    auto *boutons = new QHBoxLayout;
    boutons->addWidget(btnFiltres);
    boutons->addWidget(btnAjouter);
    boutons->addWidget(btnModifier);
    boutons->addWidget(btnSupprimer);
    boutons->addWidget(btnPdf);
    boutons->addStretch();
    boutons->addWidget(btnRefresh);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_panneauFiltres);
    layout->addWidget(m_table, 1);
    layout->addLayout(boutons);

    connect(btnFiltres, &QPushButton::clicked, this, &CoursWidget::basculerPanneauFiltres);
    connect(btnAjouter, &QPushButton::clicked, this, &CoursWidget::onAjouter);
    connect(btnModifier, &QPushButton::clicked, this, &CoursWidget::onModifier);
    connect(btnSupprimer, &QPushButton::clicked, this, &CoursWidget::onSupprimer);
    connect(btnPdf, &QPushButton::clicked, this, &CoursWidget::onGenererPdf);
    connect(btnRefresh, &QPushButton::clicked, this, &CoursWidget::rafraichir);
    connect(m_table, &QTableWidget::doubleClicked, this, &CoursWidget::onModifier);

    rafraichir();
}

void CoursWidget::construireFiltres()
{
    m_panneauFiltres = new QFrame;
    m_panneauFiltres->setFrameShape(QFrame::StyledPanel);
    m_panneauFiltres->setVisible(false);

    m_filtreDiscipline = new QComboBox;
    m_filtreDiscipline->setEditable(true);
    m_filtreDiscipline->addItem(QStringLiteral("(toutes)"), QString());
    m_filtreDiscipline->addItems({
        QStringLiteral("Danse"), QStringLiteral("Musique"), QStringLiteral("Théâtre"),
        QStringLiteral("Peinture"), QStringLiteral("Photographie")
    });

    m_filtreNiveau = new QComboBox;
    m_filtreNiveau->addItem(QStringLiteral("(tous)"), QString());
    m_filtreNiveau->addItem(QStringLiteral("Débutant"), QStringLiteral("Débutant"));
    m_filtreNiveau->addItem(QStringLiteral("Intermédiaire"), QStringLiteral("Intermédiaire"));
    m_filtreNiveau->addItem(QStringLiteral("Avancé"), QStringLiteral("Avancé"));

    m_filtreStatut = new QComboBox;
    m_filtreStatut->addItem(QStringLiteral("(tous)"), QString());
    m_filtreStatut->addItem(QStringLiteral("Ouvert"), QStringLiteral("Ouvert"));
    m_filtreStatut->addItem(QStringLiteral("Complet"), QStringLiteral("Complet"));
    m_filtreStatut->addItem(QStringLiteral("Annulé"), QStringLiteral("Annulé"));
    m_filtreStatut->addItem(QStringLiteral("Terminé"), QStringLiteral("Terminé"));

    m_filtreDateMin = new QDateEdit(QDate::currentDate().addYears(-2));
    m_filtreDateMin->setCalendarPopup(true);
    m_filtreDateMin->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    m_filtreDateMax = new QDateEdit(QDate::currentDate().addYears(2));
    m_filtreDateMax->setCalendarPopup(true);
    m_filtreDateMax->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));

    auto *cbDates = new QCheckBox(QStringLiteral("Filtrer par dates"));
    m_cbDates = cbDates;
    cbDates->setChecked(false);
    connect(cbDates, &QCheckBox::toggled, this, [this](bool on) {
        m_filtreDateMin->setEnabled(on);
        m_filtreDateMax->setEnabled(on);
    });
    m_filtreDateMin->setEnabled(false);
    m_filtreDateMax->setEnabled(false);

    m_filtreTarifMin = new QDoubleSpinBox;
    m_filtreTarifMin->setRange(-1, 99999);
    m_filtreTarifMin->setDecimals(2);
    m_filtreTarifMin->setSpecialValueText(QStringLiteral("(min)"));
    m_filtreTarifMin->setValue(-1);

    m_filtreTarifMax = new QDoubleSpinBox;
    m_filtreTarifMax->setRange(-1, 99999);
    m_filtreTarifMax->setDecimals(2);
    m_filtreTarifMax->setSpecialValueText(QStringLiteral("(max)"));
    m_filtreTarifMax->setValue(-1);

    auto *btnAppliquer = new QPushButton(QStringLiteral("Appliquer"));
    auto *btnReset = new QPushButton(QStringLiteral("Réinitialiser"));

    auto *grid = new QGridLayout(m_panneauFiltres);
    grid->addWidget(new QLabel(QStringLiteral("Discipline")), 0, 0);
    grid->addWidget(m_filtreDiscipline, 0, 1);
    grid->addWidget(new QLabel(QStringLiteral("Niveau")), 0, 2);
    grid->addWidget(m_filtreNiveau, 0, 3);
    grid->addWidget(new QLabel(QStringLiteral("Statut")), 1, 0);
    grid->addWidget(m_filtreStatut, 1, 1);
    grid->addWidget(cbDates, 1, 2);
    auto *datesLay = new QHBoxLayout;
    datesLay->addWidget(m_filtreDateMin);
    datesLay->addWidget(m_filtreDateMax);
    grid->addLayout(datesLay, 1, 3);
    grid->addWidget(new QLabel(QStringLiteral("Tarif min")), 2, 0);
    grid->addWidget(m_filtreTarifMin, 2, 1);
    grid->addWidget(new QLabel(QStringLiteral("Tarif max")), 2, 2);
    grid->addWidget(m_filtreTarifMax, 2, 3);
    grid->addWidget(btnAppliquer, 3, 2);
    grid->addWidget(btnReset, 3, 3);

    connect(btnAppliquer, &QPushButton::clicked, this, &CoursWidget::onRechercher);
    connect(btnReset, &QPushButton::clicked, this, &CoursWidget::onResetFiltres);
}

void CoursWidget::basculerPanneauFiltres()
{
    m_panneauFiltres->setVisible(!m_panneauFiltres->isVisible());
}

void CoursWidget::rafraichir()
{
    remplirTableau(m_dao.listerTous());
}

void CoursWidget::onRechercher()
{
    QString discipline = m_filtreDiscipline->currentText().trimmed();
    if (m_filtreDiscipline->currentIndex() == 0
        || discipline == QStringLiteral("(toutes)"))
        discipline.clear();

    const QString niveau = m_filtreNiveau->currentData().toString();
    const QString statut = m_filtreStatut->currentData().toString();

    QDate dateMin;
    QDate dateMax;
    if (m_cbDates && m_cbDates->isChecked()) {
        dateMin = m_filtreDateMin->date();
        dateMax = m_filtreDateMax->date();
    }

    const double tarifMin = m_filtreTarifMin->value(); // -1 = ignore
    const double tarifMax = m_filtreTarifMax->value();

    remplirTableau(m_dao.filtrer(discipline, niveau, statut, dateMin, dateMax, tarifMin, tarifMax));
}

void CoursWidget::onResetFiltres()
{
    m_filtreDiscipline->setCurrentIndex(0);
    m_filtreNiveau->setCurrentIndex(0);
    m_filtreStatut->setCurrentIndex(0);
    if (m_cbDates)
        m_cbDates->setChecked(false);
    m_filtreDateMin->setEnabled(false);
    m_filtreDateMax->setEnabled(false);
    m_filtreTarifMin->setValue(-1);
    m_filtreTarifMax->setValue(-1);
    rafraichir();
}

void CoursWidget::remplirTableau(const QList<Cours> &liste)
{
    const bool sorting = m_table->isSortingEnabled();
    m_table->setSortingEnabled(false);

    m_table->setRowCount(liste.size());
    for (int row = 0; row < liste.size(); ++row) {
        const Cours &c = liste[row];

        auto *idItem = new QTableWidgetItem;
        idItem->setData(Qt::DisplayRole, c.idCours);
        idItem->setData(Qt::UserRole, c.idFormateur);
        m_table->setItem(row, 0, idItem);

        m_table->setItem(row, 1, new QTableWidgetItem(c.titre));
        m_table->setItem(row, 2, new QTableWidgetItem(c.description));
        m_table->setItem(row, 3, new QTableWidgetItem(c.disciplineArtistique));
        m_table->setItem(row, 4, new QTableWidgetItem(c.niveau));

        auto *dureeItem = new QTableWidgetItem;
        dureeItem->setData(Qt::DisplayRole, c.dureeHeures);
        m_table->setItem(row, 5, dureeItem);

        m_table->setItem(row, 6, new QTableWidgetItem(c.dateDebut.toString("dd/MM/yyyy")));
        m_table->setItem(row, 7, new QTableWidgetItem(c.dateFin.toString("dd/MM/yyyy")));

        auto *tarifItem = new QTableWidgetItem;
        tarifItem->setData(Qt::DisplayRole, c.tarif);
        m_table->setItem(row, 8, tarifItem);

        auto *capItem = new QTableWidgetItem;
        capItem->setData(Qt::DisplayRole, c.capaciteMax);
        m_table->setItem(row, 9, capItem);

        m_table->setItem(row, 10, new QTableWidgetItem(c.salle));
        m_table->setItem(row, 11, new QTableWidgetItem(c.statut));
        m_table->setItem(row, 12, new QTableWidgetItem(c.nomFormateur));
    }
    m_table->resizeColumnsToContents();
    m_table->setSortingEnabled(sorting);
}

Cours CoursWidget::coursSelectionne() const
{
    const int row = m_table->currentRow();
    if (row < 0)
        return Cours();

    Cours c;
    c.idCours = m_table->item(row, 0)->data(Qt::DisplayRole).toInt();
    c.idFormateur = m_table->item(row, 0)->data(Qt::UserRole).toInt();
    c.titre = m_table->item(row, 1)->text();
    c.description = m_table->item(row, 2)->text();
    c.disciplineArtistique = m_table->item(row, 3)->text();
    c.niveau = m_table->item(row, 4)->text();
    c.dureeHeures = m_table->item(row, 5)->data(Qt::DisplayRole).toDouble();
    c.dateDebut = QDate::fromString(m_table->item(row, 6)->text(), "dd/MM/yyyy");
    c.dateFin = QDate::fromString(m_table->item(row, 7)->text(), "dd/MM/yyyy");
    c.tarif = m_table->item(row, 8)->data(Qt::DisplayRole).toDouble();
    c.capaciteMax = m_table->item(row, 9)->data(Qt::DisplayRole).toInt();
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
    emit donneesModifiees();
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
    emit donneesModifiees();
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
    emit donneesModifiees();
}

void CoursWidget::onGenererPdf()
{
    Cours c = coursSelectionne();
    if (c.idCours == 0) {
        QMessageBox::information(this, QStringLiteral("PDF"),
                                 QStringLiteral("Sélectionne d'abord un cours dans le tableau."));
        return;
    }
    FormateurDAO fDao;
    Formateur f = fDao.trouverParId(c.idFormateur);
    if (f.idFormateur == 0) {
        QMessageBox::warning(this, QStringLiteral("PDF"),
                             QStringLiteral("Impossible de charger le formateur associé."));
        return;
    }
    CoursPdf::genererFiche(this, c, f);
}
