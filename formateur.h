#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QDate>

// Représente une ligne de la table FORMATEUR
struct Formateur
{
    int idFormateur = 0;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QString discipline;
    QDate dateEmbauche;
    QString statut;
    int anneesExperience = 0;
};

#endif // FORMATEUR_H
