#include "formateurdialog.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QRegularExpression>

bool FormateurDialog::validerEmail(const QString &email)
{
    // Contrôle simple : texte@texte.extension
    static const QRegularExpression re(
        QStringLiteral("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"));
    return re.match(email.trimmed()).hasMatch();
}

FormateurDialog::FormateurDialog(const Formateur &f, QWidget *parent)
    : QDialog(parent)
    , m_idFormateur(f.idFormateur)
{
    setWindowTitle(m_idFormateur == 0 ? QStringLiteral("Ajouter un formateur")
                                      : QStringLiteral("Modifier un formateur"));
    resize(420, 360);

    m_nom = new QLineEdit(f.nom);
    m_prenom = new QLineEdit(f.prenom);
    m_email = new QLineEdit(f.email);
    m_telephone = new QLineEdit(f.telephone);
    m_discipline = new QLineEdit(f.discipline);

    m_dateEmbauche = new QDateEdit(
        f.dateEmbauche.isValid() ? f.dateEmbauche : QDate::currentDate());
    m_dateEmbauche->setCalendarPopup(true);
    m_dateEmbauche->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));

    m_statut = new QComboBox;
    m_statut->addItems({QStringLiteral("Actif"), QStringLiteral("Inactif")});
    if (!f.statut.isEmpty())
        m_statut->setCurrentText(f.statut);

    m_annees = new QSpinBox;
    m_annees->setRange(0, 50);
    m_annees->setValue(f.anneesExperience);

    auto *form = new QFormLayout;
    form->addRow(QStringLiteral("Nom *"), m_nom);
    form->addRow(QStringLiteral("Prénom *"), m_prenom);
    form->addRow(QStringLiteral("Email *"), m_email);
    form->addRow(QStringLiteral("Téléphone"), m_telephone);
    form->addRow(QStringLiteral("Discipline"), m_discipline);
    form->addRow(QStringLiteral("Date d'embauche *"), m_dateEmbauche);
    form->addRow(QStringLiteral("Statut"), m_statut);
    form->addRow(QStringLiteral("Années d'expérience"), m_annees);

    auto *boutons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(boutons, &QDialogButtonBox::accepted, this, &FormateurDialog::valider);
    connect(boutons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(boutons);
}

void FormateurDialog::valider()
{
    if (m_nom->text().trimmed().isEmpty()
        || m_prenom->text().trimmed().isEmpty()
        || m_email->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Champs obligatoires"),
                             QStringLiteral("Nom, prénom et email sont obligatoires."));
        return;
    }
    if (!validerEmail(m_email->text())) {
        QMessageBox::warning(this, QStringLiteral("Email invalide"),
                             QStringLiteral("Format attendu : nom@domaine.com"));
        return;
    }
    if (!m_dateEmbauche->date().isValid()) {
        QMessageBox::warning(this, QStringLiteral("Date invalide"),
                             QStringLiteral("La date d'embauche n'est pas valide."));
        return;
    }
    accept();
}

Formateur FormateurDialog::resultat() const
{
    Formateur f;
    f.idFormateur = m_idFormateur;
    f.nom = m_nom->text().trimmed();
    f.prenom = m_prenom->text().trimmed();
    f.email = m_email->text().trimmed();
    f.telephone = m_telephone->text().trimmed();
    f.discipline = m_discipline->text().trimmed();
    f.dateEmbauche = m_dateEmbauche->date();
    f.statut = m_statut->currentText();
    f.anneesExperience = m_annees->value();
    return f;
}
