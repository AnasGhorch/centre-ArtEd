#include "courspdf.h"

#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QMessageBox>

bool CoursPdf::genererFiche(QWidget *parent, const Cours &cours, const Formateur &formateur)
{
    const QString chemin = QFileDialog::getSaveFileName(
        parent,
        QStringLiteral("Enregistrer la fiche PDF"),
        QStringLiteral("fiche_%1.pdf").arg(cours.titre).replace(QLatin1Char(' '), QLatin1Char('_')),
        QStringLiteral("PDF (*.pdf)"));

    if (chemin.isEmpty())
        return false; // utilisateur a annulé

    QPdfWriter writer(chemin);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setTitle(QStringLiteral("Fiche cours — %1").arg(cours.titre));

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(parent, QStringLiteral("PDF"),
                             QStringLiteral("Impossible de créer le fichier PDF."));
        return false;
    }

    // Coordonnées en points PDF (A4 ≈ 595 x 842)
    const int margeG = 50;
    int y = 60;

    // --- En-tête stylisé ---
    painter.fillRect(0, 0, writer.width(), 90, QColor(43, 43, 43));
    painter.setPen(QColor(255, 106, 0));
    QFont titreFont = painter.font();
    titreFont.setPointSize(18);
    titreFont.setBold(true);
    painter.setFont(titreFont);
    painter.drawText(margeG, 55, QStringLiteral("ArtEd — Fiche cours"));

    painter.setPen(QColor(255, 255, 255));
    QFont sous = painter.font();
    sous.setPointSize(10);
    sous.setBold(false);
    painter.setFont(sous);
    painter.drawText(margeG, 78, QStringLiteral("Centre de formation artistique"));

    y = 120;
    painter.setPen(QColor(20, 20, 20));

    auto section = [&](const QString &titre) {
        QFont f = painter.font();
        f.setPointSize(12);
        f.setBold(true);
        painter.setFont(f);
        painter.setPen(QColor(255, 106, 0));
        painter.drawText(margeG, y, titre);
        y += 8;
        painter.setPen(QColor(255, 106, 0));
        painter.drawLine(margeG, y, writer.width() - margeG, y);
        y += 22;
        painter.setPen(QColor(20, 20, 20));
        f.setBold(false);
        f.setPointSize(10);
        painter.setFont(f);
    };

    auto ligne = [&](const QString &label, const QString &valeur) {
        painter.drawText(margeG, y, label);
        painter.drawText(margeG + 160, y, valeur);
        y += 20;
    };

    section(QStringLiteral("Informations du cours"));
    ligne(QStringLiteral("Titre :"), cours.titre);
    ligne(QStringLiteral("Discipline :"), cours.disciplineArtistique);
    ligne(QStringLiteral("Niveau :"), cours.niveau);
    ligne(QStringLiteral("Date début :"), cours.dateDebut.toString(QStringLiteral("dd/MM/yyyy")));
    ligne(QStringLiteral("Date fin :"), cours.dateFin.toString(QStringLiteral("dd/MM/yyyy")));
    ligne(QStringLiteral("Tarif :"), QStringLiteral("%1 DT").arg(cours.tarif, 0, 'f', 2));
    ligne(QStringLiteral("Capacité max :"), QString::number(cours.capaciteMax));
    ligne(QStringLiteral("Salle :"), cours.salle);
    ligne(QStringLiteral("Statut :"), cours.statut);
    if (!cours.description.isEmpty()) {
        y += 6;
        painter.drawText(margeG, y, QStringLiteral("Description :"));
        y += 18;
        // Texte multiligne simple
        QRect textRect(margeG, y, writer.width() - 2 * margeG, 80);
        painter.drawText(textRect, Qt::TextWordWrap, cours.description);
        y += 90;
    }

    y += 10;
    section(QStringLiteral("Formateur qui anime le cours"));
    ligne(QStringLiteral("Nom :"),
          QStringLiteral("%1 %2").arg(formateur.prenom, formateur.nom));
    ligne(QStringLiteral("Discipline :"), formateur.discipline);
    ligne(QStringLiteral("Email :"), formateur.email);
    ligne(QStringLiteral("Téléphone :"), formateur.telephone);
    ligne(QStringLiteral("Statut :"), formateur.statut);

    // Pied de page
    painter.setPen(QColor(120, 120, 120));
    QFont pied = painter.font();
    pied.setPointSize(8);
    painter.setFont(pied);
    painter.drawText(margeG, writer.height() - 40,
                     QStringLiteral("Document généré par l'application ArtEd"));

    painter.end();
    QMessageBox::information(parent, QStringLiteral("PDF"),
                             QStringLiteral("Fiche enregistrée :\n%1").arg(chemin));
    return true;
}
