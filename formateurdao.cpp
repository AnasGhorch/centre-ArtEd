#include "formateurdao.h"
#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

static const char *CONN = "formation_connexion";

Formateur FormateurDAO::lireLigne(QSqlQuery &query) const
{
    Formateur f;
    f.idFormateur = query.value(0).toInt();
    f.nom = query.value(1).toString();
    f.prenom = query.value(2).toString();
    f.email = query.value(3).toString();
    f.telephone = query.value(4).toString();
    f.discipline = query.value(5).toString();
    f.dateEmbauche = query.value(6).toDate();
    f.statut = query.value(7).toString();
    f.anneesExperience = query.value(8).toInt();
    return f;
}

void FormateurDAO::setErreur(const QString &msg)
{
    m_derniereErreur = msg;
}

QList<Formateur> FormateurDAO::listerTous()
{
    QList<Formateur> liste;
    if (!Database::estOuverte())
        return liste;

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare(
        "SELECT id_formateur, nom, prenom, email, telephone, discipline, "
        "date_embauche, statut, annees_experience "
        "FROM FORMATEUR ORDER BY id_formateur");

    if (!query.exec()) {
        setErreur(query.lastError().text());
        return liste;
    }
    while (query.next())
        liste.append(lireLigne(query));
    return liste;
}

Formateur FormateurDAO::trouverParId(int idFormateur)
{
    Formateur f;
    if (!Database::estOuverte())
        return f;

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare(
        "SELECT id_formateur, nom, prenom, email, telephone, discipline, "
        "date_embauche, statut, annees_experience "
        "FROM FORMATEUR WHERE id_formateur=?");
    query.addBindValue(idFormateur);
    if (query.exec() && query.next())
        f = lireLigne(query);
    return f;
}

QList<Formateur> FormateurDAO::filtrer(const QString &nom,
                                       const QString &discipline,
                                       const QString &statut,
                                       int expMin,
                                       int expMax)
{
    QList<Formateur> liste;
    if (!Database::estOuverte())
        return liste;

    // Une seule requête préparée : on ajoute les clauses seulement si le filtre est actif
    QString sql =
        "SELECT id_formateur, nom, prenom, email, telephone, discipline, "
        "date_embauche, statut, annees_experience FROM FORMATEUR WHERE 1=1";

    if (!nom.isEmpty())
        sql += " AND UPPER(nom) LIKE UPPER(?)";
    if (!discipline.isEmpty())
        sql += " AND discipline = ?";
    if (!statut.isEmpty())
        sql += " AND statut = ?";
    if (expMin >= 0)
        sql += " AND annees_experience >= ?";
    if (expMax >= 0)
        sql += " AND annees_experience <= ?";

    sql += " ORDER BY id_formateur";

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare(sql);

    if (!nom.isEmpty())
        query.addBindValue(QStringLiteral("%") + nom + QStringLiteral("%"));
    if (!discipline.isEmpty())
        query.addBindValue(discipline);
    if (!statut.isEmpty())
        query.addBindValue(statut);
    if (expMin >= 0)
        query.addBindValue(expMin);
    if (expMax >= 0)
        query.addBindValue(expMax);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        return liste;
    }
    while (query.next())
        liste.append(lireLigne(query));
    return liste;
}

QMap<QString, int> FormateurDAO::compterParDiscipline()
{
    QMap<QString, int> map;
    if (!Database::estOuverte())
        return map;

    QSqlQuery query(QSqlDatabase::database(CONN));
    if (!query.exec(
            "SELECT NVL(discipline, 'Non renseigné'), COUNT(*) "
            "FROM FORMATEUR GROUP BY discipline ORDER BY COUNT(*) DESC"))
        return map;
    while (query.next())
        map[query.value(0).toString()] = query.value(1).toInt();
    return map;
}

bool FormateurDAO::ajouter(const Formateur &f, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare(
        "INSERT INTO FORMATEUR (id_formateur, nom, prenom, email, telephone, "
        "discipline, date_embauche, statut, annees_experience) "
        "VALUES (seq_formateur.NEXTVAL, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(f.nom);
    query.addBindValue(f.prenom);
    query.addBindValue(f.email);
    query.addBindValue(f.telephone);
    query.addBindValue(f.discipline);
    query.addBindValue(f.dateEmbauche);
    query.addBindValue(f.statut);
    query.addBindValue(f.anneesExperience);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool FormateurDAO::modifier(const Formateur &f, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare(
        "UPDATE FORMATEUR SET nom=?, prenom=?, email=?, telephone=?, "
        "discipline=?, date_embauche=?, statut=?, annees_experience=? "
        "WHERE id_formateur=?");
    query.addBindValue(f.nom);
    query.addBindValue(f.prenom);
    query.addBindValue(f.email);
    query.addBindValue(f.telephone);
    query.addBindValue(f.discipline);
    query.addBindValue(f.dateEmbauche);
    query.addBindValue(f.statut);
    query.addBindValue(f.anneesExperience);
    query.addBindValue(f.idFormateur);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool FormateurDAO::supprimer(int idFormateur, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN));
    query.prepare("DELETE FROM FORMATEUR WHERE id_formateur=?");
    query.addBindValue(idFormateur);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}
