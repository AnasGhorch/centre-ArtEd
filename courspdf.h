#ifndef COURSPDF_H
#define COURSPDF_H

#include "cours.h"
#include "formateur.h"

#include <QString>
#include <QWidget>

// Génère un PDF "fiche cours" avec QPdfWriter + QPainter
namespace CoursPdf {

// Demande le chemin (QFileDialog) puis écrit le PDF.
// Retourne true si le fichier a bien été créé.
bool genererFiche(QWidget *parent, const Cours &cours, const Formateur &formateur);

} // namespace CoursPdf

#endif // COURSPDF_H
