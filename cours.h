#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QDate>

// Représente une ligne de la table COURS (+ nom du formateur pour l'affichage)
struct Cours
{
    int idCours = 0;
    QString titre;
    QString description;
    QString disciplineArtistique;
    QString niveau;
    double dureeHeures = 0.0;
    QDate dateDebut;
    QDate dateFin;
    double tarif = 0.0;
    int capaciteMax = 0;
    QString salle;
    QString statut;
    int idFormateur = 0;          // clé étrangère
    QString nomFormateur;         // rempli par le JOIN (affichage uniquement)
};

#endif // COURS_H
