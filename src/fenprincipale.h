/*

###################    DériVoile calc' - Français    ###################

Version : v6-5
Date : 2013-06-19
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

Version : v6-5
Date : 2013-06-19
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
#include <QString>
#include <QActionGroup>
#include <QSettings>
#include <QSplashScreen>
#include <QtNetwork>
#include <QtWebKit>
#include <QWebView>
#include <QWebFrame>
#include <QWebInspector>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>

namespace Ui {
	class FenPrincipale;
};

class FenPrincipale : public QMainWindow {
	Q_OBJECT

	public:
		explicit FenPrincipale(
			QSettings *preferences, QSplashScreen *splash, QWidget *parent = 0
		);
		~FenPrincipale();
		static int const EXIT_CODE_REBOOT;
		static QString const VERSION;
		bool confirm(QString title, QString text, QString icon);
		void msg(QString title, QString text, QString icon);
		void restart();
		bool load_ratings(QString filename);
		void update_ratings_js();

	private:
		Ui::FenPrincipale *ui;
		QString filename;
		bool hasModif;
		int etapeActuelle;
		QSettings *preferences;
		QSplashScreen *splash;
		int splashDone;
		QByteArray ratings;
		QString file_ratings;
		QString version_ratings;
		QWebView *webView;
		QWebFrame *webFrame;
		QProgressBar *progress;
		QLabel *progressText;
		QProgressDialog *progressDialog;
		QWebInspector *inspector;
		QMainWindow *fenRatings;
		QMainWindow *fenAide;
		QString data;
		void set_titre();
		void addProgressBar(QString text);
		void removeProgressBar();
		bool confirm_close();
			// Classement.cpp
		void ouvrir(QString name);
		void ouvrir_failed(bool msg);
		void load_recents();
		void enregistrer(QString name);
		void enregistrer_failed(bool msg);
		QString get_printed_html(QString html);
			// MAJ.cpp
		QNetworkReply *download(QString url, const char *slot);
		bool check_reply(QNetworkReply *reply);
		void update_deri();
		void update_ratings();
		void download_ratings();

	public slots:
		void modif();
		void set_etape(int nb);
		void calculer();
		void calculer_callback(QString msg);
		void progression(int nb);
		QString get_ratings();
		QString get_data();
			// Classement.cpp
		void ouvrir_callback(bool ok);
		void enregistrer_callback(QString data);
		void pdf_callback(QString html);
		void html_callback(QString html);

	private slots:
		void attach_javascript();
		void load_finished(bool ok);
		void on_francais_triggered();
		void on_english_triggered();
		void on_ratings_triggered();
		void on_aide_triggered();
		void on_site_web_triggered();
		void on_propos_triggered();
		void closeEvent(QCloseEvent *event);
			// Classement.cpp
		void on_nouveau_triggered();
		void on_ouvrir_triggered();
		void on_recent1_triggered();
		void on_recent2_triggered();
		void on_recent3_triggered();
		void on_recent4_triggered();
		void on_recent5_triggered();
		void on_recents_clean_triggered();
		void on_enregistrer_triggered();
		void on_enregistrer_sous_triggered();
		void on_pdf_triggered();
		void on_html_triggered();
		void on_quitter_triggered();
			// MAJ.cpp
				// Menu
		void on_maj_deri_triggered();
		void on_maj_deri_auto_triggered();
		void on_maj_deri_manu_triggered();
		void on_maj_ratings_triggered();
		void on_maj_ratings_auto_triggered();
		void on_maj_ratings_manu_triggered();
				// Network
		void maj_deri_done(QNetworkReply *reply);
		void update_deri_done(QNetworkReply *reply);
		void maj_ratings_done(QNetworkReply *reply);
		void update_ratings_done(QNetworkReply *reply);
		void download_progress(qint64, qint64);
		void download_ratings_done(QNetworkReply *reply);
		void download_canceled();

};


/*class pageRatings : public QWebPage {
	Q_OBJECT

	public slots:
		bool shouldInterruptJavaScript() {
			QApplication::processEvents();
			return false;
		}
};*/

#endif // FENPRINCIPALE_H