#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Connexion Oracle ouverte une seule fois au démarrage
    QString err;
    if (!Database::ouvrir(&err)) {
        QMessageBox::critical(
            nullptr,
            QStringLiteral("Connexion Oracle"),
            QStringLiteral("Impossible de se connecter via QODBC (DSN XE_SYSTEM).\n\n%1")
                .arg(err));
        return 1;
    }

    MainWindow fenetre;
    fenetre.show();

    const int code = app.exec();

    Database::fermer();
    return code;
}
