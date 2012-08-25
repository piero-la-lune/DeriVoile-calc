/*

###################    DériVoile calc' - Français    ###################

Version : v6-1
Date : 25/08/2012
Licence : dans le fichier « COPYING »
Site web : http://calc.derivoile.fr

Copyright © 2011-2012 Pierre Monchalin, http://derivoile.fr
Pensez à me laisser un petit mot si vous distribuez une copie !

DériVoile calc' est un logiciel libre : vous pouvez le redistribuer ou
le modifier selon les termes de la GNU General Public Licence tels que
publiés par la Free Software Foundation : à votre choix, soit la
version 3 de la licence, soit une version ultérieure quelle qu'elle
soit.

DériVoile calc' est distribué dans l'espoir qu'il sera utile, mais SANS
AUCUNE GARANTIE ; sans même la garantie implicite de QUALITÉ MARCHANDE
ou D'ADÉQUATION À UNE UTILISATION PARTICULIÈRE. Pour plus de détails,
reportez-vous à la GNU General Public License.

Vous devez avoir reçu une copie de la GNU General Public License avec
DériVoile calc'. Si ce n'est pas le cas, consultez
<http://www.gnu.org/licenses/>

###################    DériVoile calc' - English    ###################

Version : v6-1
Date : 25/08/2012
Licence : see file “COPYING”
Web site : http://calc.derivoile.fr

Copyright © 2011-2012 Pierre Monchalin, http://derivoile.fr
Remember to leave me a note if you distribute a copy!

DériVoile calc' is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

DériVoile calc' is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with DériVoile calc'. If not, see
<http://www.gnu.org/licenses/>

*/

#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QMainWindow>
#include <QtWebKit>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QString>
#include <QVariantList>
#include <QPrinter>
#include <QPrintDialog>
#include <QLabel>
#include <QProgressBar>
#include <QToolBar>

namespace Ui {
    class FenPrincipale;
}

class FenPrincipale : public QMainWindow {
	Q_OBJECT

public:
	explicit FenPrincipale(QWidget *parent = 0);
	~FenPrincipale();
	QString version;

private:
	Ui::FenPrincipale *ui;
	QWebView *pageWeb;
	QWebFrame *pageWebFrame;
	QString classement;
	QProgressBar *progres;
	QLabel *texteProgres;
	int etapeActuelle;
	bool hasModif;

private slots:
	void attachObject();
	void chargementTermine(bool ok);
	void addProgressBar(QString texte);
	void removeProgressBar();
	void setTitre();
	bool ouvrirClassement(QString fileName);
	void on_actionNouveau_triggered();
	bool on_actionOuvrir_triggered();
	void on_actionEnregistrer_triggered();
	void on_actionEnregistrer_sous_triggered();
	void on_actionAide_triggered();
	void on_actionConsulter_les_ratings_triggered();
	void on_actionExtraits_des_R_gles_de_Course_triggered();
	void on_action_propos_triggered();
	void on_actionImprimer_triggered();
	void on_actionExporter_triggered();
	void on_actionExporter_en_HTML_triggered();
	void closeEvent(QCloseEvent *event);
	void updater(QNetworkReply *reply);

public slots:
	void calculer();
	void calculM(QString message);
	void progression(int nb);
	bool enregistrer(QString donnees);
	void ouvrir(QString message);
	bool exporterHTML(QString html);
	void setEtape(int nb);
	void modif();
};

#endif // FENPRINCIPALE_H
