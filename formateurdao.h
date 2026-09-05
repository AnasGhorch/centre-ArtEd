#ifndef FORMATEURDAO_H
#define FORMATEURDAO_H

#include "formateur.h"

#include <QList>
#include <QMap>
#include <QString>

// Toutes les requêtes SQL sur la table FORMATEUR
class FormateurDAO
{
public:
    QList<Formateur> listerTous();
    Formateur trouverParId(int idFormateur);
    bool ajouter(const Formateur &f, QString *messageErreur = nullptr);
    bool modifier(const Formateur &f, QString *messageErreur = nullptr);
    bool supprimer(int idFormateur, QString *messageErreur = nullptr);

    // Filtre combiné : chaînes vides / valeurs -1 = critère ignoré
    QList<Formateur> filtrer(const QString &nom,
                             const QString &discipline,
                             const QString &statut,
                             int expMin,
                             int expMax);

    // Pour les statistiques : discipline → nombre de formateurs
    QMap<QString, int> compterParDiscipline();

    QString derniereErreur() const { return m_derniereErreur; }

private:
    Formateur lireLigne(class QSqlQuery &query) const;
    void setErreur(const QString &msg);

    QString m_derniereErreur;
};

#endif // FORMATEURDAO_H
