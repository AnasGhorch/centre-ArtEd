#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QMessageBox>

// Thème clair ArtEd : fond blanc cassé pour que le logo (gris + orange) reste lisible.
// Accent orange #FF6A00 comme sur le logo.
static QString styleArtEd()
{
    return QStringLiteral(R"(
        QMainWindow, QWidget {
            background-color: #f5f5f5;
            color: #1a1a1a;
            font-size: 13px;
        }

        /* Barre logo + titre — fond clair pour contraste avec le logo */
        #headerBar {
            background-color: #ffffff;
            border-bottom: 2px solid #FF6A00;
        }
        #titreApp {
            color: #2b2b2b;
            font-size: 16px;
            font-weight: bold;
        }

        /* Onglets */
        QTabWidget::pane {
            border: 1px solid #d0d0d0;
            background-color: #ffffff;
            top: -1px;
        }
        QTabBar::tab {
            background-color: #e8e8e8;
            color: #444444;
            padding: 8px 18px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #ffffff;
            color: #FF6A00;
            font-weight: bold;
            border-bottom: 2px solid #FF6A00;
        }
        QTabBar::tab:hover:!selected {
            color: #1a1a1a;
            background-color: #f0f0f0;
        }

        /* Tableau */
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #fafafa;
            color: #1a1a1a;
            gridline-color: #e0e0e0;
            border: 1px solid #d0d0d0;
            selection-background-color: #FF6A00;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background-color: #2b2b2b;
            color: #ffffff;
            padding: 6px;
            border: none;
            border-right: 1px solid #3a3a3a;
            border-bottom: 2px solid #FF6A00;
            font-weight: bold;
        }

        /* Boutons */
        QPushButton {
            background-color: #ffffff;
            color: #1a1a1a;
            border: 1px solid #c0c0c0;
            border-radius: 4px;
            padding: 6px 14px;
            min-height: 24px;
        }
        QPushButton:hover {
            background-color: #fff3eb;
            border-color: #FF6A00;
            color: #FF6A00;
        }
        QPushButton:pressed {
            background-color: #FF6A00;
            color: #ffffff;
            border-color: #FF6A00;
        }

        /* Formulaires (dialogues) */
        QDialog {
            background-color: #f5f5f5;
            color: #1a1a1a;
        }
        QLineEdit, QTextEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox {
            background-color: #ffffff;
            color: #1a1a1a;
            border: 1px solid #c0c0c0;
            border-radius: 3px;
            padding: 4px 6px;
            selection-background-color: #FF6A00;
            selection-color: #ffffff;
        }
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateEdit:focus,
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #FF6A00;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #1a1a1a;
            selection-background-color: #FF6A00;
            selection-color: #ffffff;
        }

        /* Barre de statut */
        QStatusBar {
            background-color: #ffffff;
            color: #666666;
            border-top: 1px solid #d0d0d0;
        }

        QLabel {
            color: #1a1a1a;
            background: transparent;
        }
        QMessageBox {
            background-color: #f5f5f5;
        }
    )");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(styleArtEd());

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
