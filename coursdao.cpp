#include "coursdao.h"
#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

static const char *CONN_COURS = "formation_connexion";

Cours CoursDAO::lireLigne(QSqlQuery &query) const
{
    Cours c;
    c.idCours = query.value(0).toInt();
    c.titre = query.value(1).toString();
    c.description = query.value(2).toString();
    c.disciplineArtistique = query.value(3).toString();
    c.niveau = query.value(4).toString();
    c.dureeHeures = query.value(5).toDouble();
    c.dateDebut = query.value(6).toDate();
    c.dateFin = query.value(7).toDate();
    c.tarif = query.value(8).toDouble();
    c.capaciteMax = query.value(9).toInt();
    c.salle = query.value(10).toString();
    c.statut = query.value(11).toString();
    c.idFormateur = query.value(12).toInt();
    c.nomFormateur = query.value(13).toString() + QStringLiteral(" ") + query.value(14).toString();
    return c;
}

void CoursDAO::setErreur(const QString &msg)
{
    m_derniereErreur = msg;
}

QList<Cours> CoursDAO::listerTous()
{
    QList<Cours> liste;
    if (!Database::estOuverte())
        return liste;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(
        "SELECT c.id_cours, c.titre, c.description, c.discipline_artistique, c.niveau, "
        "c.duree_heures, c.date_debut, c.date_fin, c.tarif, c.capacite_max, c.salle, c.statut, "
        "c.id_formateur, f.prenom, f.nom "
        "FROM COURS c "
        "JOIN FORMATEUR f ON c.id_formateur = f.id_formateur "
        "ORDER BY c.id_cours");

    if (!query.exec()) {
        setErreur(query.lastError().text());
        return liste;
    }
    while (query.next())
        liste.append(lireLigne(query));
    return liste;
}

QList<Cours> CoursDAO::filtrer(const QString &discipline,
                               const QString &niveau,
                               const QString &statut,
                               const QDate &dateMin,
                               const QDate &dateMax,
                               double tarifMin,
                               double tarifMax)
{
    QList<Cours> liste;
    if (!Database::estOuverte())
        return liste;

    QString sql =
        "SELECT c.id_cours, c.titre, c.description, c.discipline_artistique, c.niveau, "
        "c.duree_heures, c.date_debut, c.date_fin, c.tarif, c.capacite_max, c.salle, c.statut, "
        "c.id_formateur, f.prenom, f.nom "
        "FROM COURS c "
        "JOIN FORMATEUR f ON c.id_formateur = f.id_formateur "
        "WHERE 1=1";

    if (!discipline.isEmpty())
        sql += " AND c.discipline_artistique = ?";
    if (!niveau.isEmpty())
        sql += " AND c.niveau = ?";
    if (!statut.isEmpty())
        sql += " AND c.statut = ?";
    if (dateMin.isValid())
        sql += " AND c.date_debut >= ?";
    if (dateMax.isValid())
        sql += " AND c.date_fin <= ?";
    if (tarifMin >= 0)
        sql += " AND c.tarif >= ?";
    if (tarifMax >= 0)
        sql += " AND c.tarif <= ?";

    sql += " ORDER BY c.id_cours";

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(sql);

    if (!discipline.isEmpty())
        query.addBindValue(discipline);
    if (!niveau.isEmpty())
        query.addBindValue(niveau);
    if (!statut.isEmpty())
        query.addBindValue(statut);
    if (dateMin.isValid())
        query.addBindValue(dateMin);
    if (dateMax.isValid())
        query.addBindValue(dateMax);
    if (tarifMin >= 0)
        query.addBindValue(tarifMin);
    if (tarifMax >= 0)
        query.addBindValue(tarifMax);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        return liste;
    }
    while (query.next())
        liste.append(lireLigne(query));
    return liste;
}

QMap<QString, int> CoursDAO::compterParDiscipline()
{
    QMap<QString, int> map;
    if (!Database::estOuverte())
        return map;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    if (!query.exec(
            "SELECT NVL(discipline_artistique, 'Non renseigné'), COUNT(*) "
            "FROM COURS GROUP BY discipline_artistique ORDER BY COUNT(*) DESC"))
        return map;
    while (query.next())
        map[query.value(0).toString()] = query.value(1).toInt();
    return map;
}

QMap<QString, double> CoursDAO::revenuParDiscipline()
{
    QMap<QString, double> map;
    if (!Database::estOuverte())
        return map;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    if (!query.exec(
            "SELECT NVL(discipline_artistique, 'Non renseigné'), NVL(SUM(tarif), 0) "
            "FROM COURS GROUP BY discipline_artistique ORDER BY SUM(tarif) DESC"))
        return map;
    while (query.next())
        map[query.value(0).toString()] = query.value(1).toDouble();
    return map;
}

bool CoursDAO::ajouter(const Cours &c, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(
        "INSERT INTO COURS (id_cours, titre, description, discipline_artistique, niveau, "
        "duree_heures, date_debut, date_fin, tarif, capacite_max, salle, statut, id_formateur) "
        "VALUES (seq_cours.NEXTVAL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(c.titre);
    query.addBindValue(c.description);
    query.addBindValue(c.disciplineArtistique);
    query.addBindValue(c.niveau);
    query.addBindValue(c.dureeHeures);
    query.addBindValue(c.dateDebut);
    query.addBindValue(c.dateFin);
    query.addBindValue(c.tarif);
    query.addBindValue(c.capaciteMax);
    query.addBindValue(c.salle);
    query.addBindValue(c.statut);
    query.addBindValue(c.idFormateur);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool CoursDAO::modifier(const Cours &c, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(
        "UPDATE COURS SET titre=?, description=?, discipline_artistique=?, niveau=?, "
        "duree_heures=?, date_debut=?, date_fin=?, tarif=?, capacite_max=?, salle=?, "
        "statut=?, id_formateur=? WHERE id_cours=?");
    query.addBindValue(c.titre);
    query.addBindValue(c.description);
    query.addBindValue(c.disciplineArtistique);
    query.addBindValue(c.niveau);
    query.addBindValue(c.dureeHeures);
    query.addBindValue(c.dateDebut);
    query.addBindValue(c.dateFin);
    query.addBindValue(c.tarif);
    query.addBindValue(c.capaciteMax);
    query.addBindValue(c.salle);
    query.addBindValue(c.statut);
    query.addBindValue(c.idFormateur);
    query.addBindValue(c.idCours);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool CoursDAO::supprimer(int idCours, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare("DELETE FROM COURS WHERE id_cours=?");
    query.addBindValue(idCours);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}
