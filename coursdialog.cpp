#include "coursdialog.h"
#include "formateurdao.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>

CoursDialog::CoursDialog(const Cours &c, QWidget *parent)
    : QDialog(parent)
    , m_idCours(c.idCours)
{
    setWindowTitle(m_idCours == 0 ? QStringLiteral("Ajouter un cours")
                                  : QStringLiteral("Modifier un cours"));
    resize(480, 520);

    m_titre = new QLineEdit(c.titre);

    m_description = new QTextEdit;
    m_description->setPlainText(c.description);
    m_description->setMaximumHeight(80);

    m_discipline = new QLineEdit(c.disciplineArtistique);

    m_niveau = new QComboBox;
    m_niveau->setEditable(true);
    m_niveau->addItems({
        QStringLiteral("Débutant"),
        QStringLiteral("Intermédiaire"),
        QStringLiteral("Avancé")
    });
    if (!c.niveau.isEmpty())
        m_niveau->setCurrentText(c.niveau);

    m_duree = new QDoubleSpinBox;
    m_duree->setRange(0.5, 999.0);
    m_duree->setDecimals(1);
    m_duree->setSuffix(QStringLiteral(" h"));
    m_duree->setValue(c.dureeHeures > 0 ? c.dureeHeures : 1.0);

    m_dateDebut = new QDateEdit(
        c.dateDebut.isValid() ? c.dateDebut : QDate::currentDate());
    m_dateDebut->setCalendarPopup(true);
    m_dateDebut->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));

    m_dateFin = new QDateEdit(
        c.dateFin.isValid() ? c.dateFin : QDate::currentDate().addMonths(1));
    m_dateFin->setCalendarPopup(true);
    m_dateFin->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));

    m_tarif = new QDoubleSpinBox;
    m_tarif->setRange(0.0, 99999.99);
    m_tarif->setDecimals(2);
    m_tarif->setSuffix(QStringLiteral(" DT"));
    m_tarif->setValue(c.tarif);

    m_capacite = new QSpinBox;
    m_capacite->setRange(1, 500);
    m_capacite->setValue(c.capaciteMax > 0 ? c.capaciteMax : 20);

    m_salle = new QLineEdit(c.salle);

    m_statut = new QComboBox;
    m_statut->addItems({
        QStringLiteral("Ouvert"),
        QStringLiteral("Complet"),
        QStringLiteral("Annulé"),
        QStringLiteral("Terminé")
    });
    if (!c.statut.isEmpty())
        m_statut->setCurrentText(c.statut);

    m_formateur = new QComboBox;
    chargerFormateurs();
    // Sélectionne le formateur déjà lié (mode modification)
    if (c.idFormateur > 0) {
        const int idx = m_formateur->findData(c.idFormateur);
        if (idx >= 0)
            m_formateur->setCurrentIndex(idx);
    }

    auto *form = new QFormLayout;
    form->addRow(QStringLiteral("Titre *"), m_titre);
    form->addRow(QStringLiteral("Description"), m_description);
    form->addRow(QStringLiteral("Discipline artistique"), m_discipline);
    form->addRow(QStringLiteral("Niveau"), m_niveau);
    form->addRow(QStringLiteral("Durée (heures) *"), m_duree);
    form->addRow(QStringLiteral("Date début *"), m_dateDebut);
    form->addRow(QStringLiteral("Date fin *"), m_dateFin);
    form->addRow(QStringLiteral("Tarif *"), m_tarif);
    form->addRow(QStringLiteral("Capacité max *"), m_capacite);
    form->addRow(QStringLiteral("Salle"), m_salle);
    form->addRow(QStringLiteral("Statut"), m_statut);
    form->addRow(QStringLiteral("Formateur *"), m_formateur);

    auto *boutons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(boutons, &QDialogButtonBox::accepted, this, &CoursDialog::valider);
    connect(boutons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(boutons);
}

void CoursDialog::chargerFormateurs()
{
    FormateurDAO dao;
    const QList<Formateur> formateurs = dao.listerTous();
    m_formateur->clear();

    if (formateurs.isEmpty()) {
        m_formateur->addItem(QStringLiteral("(aucun formateur — crée-en un d'abord)"), 0);
        return;
    }

    for (const Formateur &f : formateurs) {
        // Texte visible : "Prénom Nom" — données cachées : id_formateur
        m_formateur->addItem(
            QStringLiteral("%1 %2").arg(f.prenom, f.nom),
            f.idFormateur);
    }
}

void CoursDialog::valider()
{
    if (m_titre->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Champ obligatoire"),
                             QStringLiteral("Le titre est obligatoire."));
        return;
    }
    if (!m_dateDebut->date().isValid() || !m_dateFin->date().isValid()) {
        QMessageBox::warning(this, QStringLiteral("Date invalide"),
                             QStringLiteral("Les dates de début et de fin doivent être valides."));
        return;
    }
    if (m_dateFin->date() < m_dateDebut->date()) {
        QMessageBox::warning(this, QStringLiteral("Dates incohérentes"),
                             QStringLiteral("La date de fin doit être après (ou égale à) la date de début."));
        return;
    }
    if (m_duree->value() <= 0) {
        QMessageBox::warning(this, QStringLiteral("Durée invalide"),
                             QStringLiteral("La durée doit être positive."));
        return;
    }
    if (m_tarif->value() < 0) {
        QMessageBox::warning(this, QStringLiteral("Tarif invalide"),
                             QStringLiteral("Le tarif ne peut pas être négatif."));
        return;
    }
    if (m_capacite->value() <= 0) {
        QMessageBox::warning(this, QStringLiteral("Capacité invalide"),
                             QStringLiteral("La capacité max doit être positive."));
        return;
    }
    if (m_formateur->currentData().toInt() <= 0) {
        QMessageBox::warning(this, QStringLiteral("Formateur manquant"),
                             QStringLiteral("Choisis un formateur pour animer ce cours."));
        return;
    }
    accept();
}

Cours CoursDialog::resultat() const
{
    Cours c;
    c.idCours = m_idCours;
    c.titre = m_titre->text().trimmed();
    c.description = m_description->toPlainText().trimmed();
    c.disciplineArtistique = m_discipline->text().trimmed();
    c.niveau = m_niveau->currentText().trimmed();
    c.dureeHeures = m_duree->value();
    c.dateDebut = m_dateDebut->date();
    c.dateFin = m_dateFin->date();
    c.tarif = m_tarif->value();
    c.capaciteMax = m_capacite->value();
    c.salle = m_salle->text().trimmed();
    c.statut = m_statut->currentText();
    c.idFormateur = m_formateur->currentData().toInt();
    c.nomFormateur = m_formateur->currentText();
    return c;
}
