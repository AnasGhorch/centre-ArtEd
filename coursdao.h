#ifndef COURSDAO_H
#define COURSDAO_H

#include <QList>
#include <QString>

// Squelette pour le module Cours (à compléter à l'étape suivante).
// Même principe que FormateurDAO : toutes les requêtes SQL ici.
class CoursDAO
{
public:
    // Exemple de méthode future :
    // QList<Cours> listerTous();
    // bool ajouter(const Cours &c, QString *messageErreur = nullptr);

    QString derniereErreur() const { return m_derniereErreur; }

private:
    QString m_derniereErreur;
};

#endif // COURSDAO_H
