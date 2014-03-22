/*

###################    DériVoile calc' - Français    ###################

Version : v7-1
Date : 2014-03-22
Licence : dans le fichier « COPYING »
Site web : http://calc.derivoile.fr

Copyright © 2011-2014 Pierre Monchalin, http://derivoile.fr
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

Version : v7-1
Date : 2014-03-22
Licence : see file “COPYING”
Web site : http://calc.derivoile.fr

Copyright © 2011-2014 Pierre Monchalin, http://derivoile.fr
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
#include <QTableWidget>
#include <QCompleter>
#include <QLineEdit>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QScrollBar>

class MyLineEdit : public QLineEdit {
	Q_OBJECT
	
	public:
		MyLineEdit(QWidget *parent = 0): QLineEdit(parent) {};
		~MyLineEdit() {};

	signals:
		void focussed(bool hasFocus);
		void keyPressed(QKeyEvent *e);

	protected:
		virtual void focusInEvent(QFocusEvent *e) {
			QLineEdit::focusInEvent(e);
			emit(focussed(true));
		};
		virtual void focusOutEvent(QFocusEvent *e) {
			QLineEdit::focusOutEvent(e);
			emit(focussed(false));
		};
		virtual void keyPressEvent(QKeyEvent *e) {
			QLineEdit::keyPressEvent(e);
			emit(keyPressed(e));
		}
};
class MyLabel : public QLabel {
	Q_OBJECT
	
	public:
		MyLabel(QWidget *parent = 0): QLabel(parent) {};
		MyLabel(QString text, QWidget *parent = 0): QLabel(text, parent) {};
		~MyLabel() {};

	signals:
		void focussed(bool hasFocus);

	protected:
		virtual void focusInEvent(QFocusEvent *e) {
			QLabel::focusInEvent(e);
			emit(focussed(true));
		};
		virtual void focusOutEvent(QFocusEvent *e) {
			QLabel::focusOutEvent(e);
			emit(focussed(false));
		};
};
template <typename T, typename U>
class MyMap : public QMap<T,U> {
	public:
		MyMap(): QMap() {};
		~MyMap() {};
		void rm(int nb) {
			for (int i = nb+1; i < this->count(); ++i) {
				this->insert(i-1, this->value(i));
			}
			this->remove(this->count()-1);
		}
};

namespace Ui {
	class FenPrincipale;
};

struct Bateau {
	QString type;
	QString serie;
	double rating;
	double coef;
	int rya;
	double groupe;
};
struct Manche {
	int h;
	int min;
	int s;
	int pl;
	QString abr;
	int tpsReel;
	double tpsCompense;
	double points;
	Manche() : h(-1), min(-1), s(-1), pl(-1), abr(""), tpsReel(-1), tpsCompense(-1), points(0.0) {}
};
struct Equipage {
	QString nom;
	QString bateau;
	QString rating;
	double coef;
	MyMap<int, Manche> manches;
	double points;
	QList<double> pointsOrdonnes;
	QList<double> pointsTries;
	QList<double> pointsRetires;
	Equipage() : nom(""), bateau(""), rating(""), coef(0), points(0.0) {}
};
struct TpsWdg {
	MyLineEdit *h;
	MyLineEdit *min;
	MyLineEdit *s;
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
		bool confirm(QString title, QString text, QString icon, QString oui);
		void msg(QString title, QString text, QString icon);
		int getInt(QString title, QString text, int value, int min, int max);
		void restart();
		bool load_ratings(QString filename);
			// Régate
		static QString const CLMT_TEMPS;
		static QString const CLMT_SCRATCH;
		static QString const RT_FFV;
		static QString const RT_RYA;
		static QString const RT_DERI;
		static QString const BT_MUL;
		static QString const BT_DER;
		static QString const BT_QUI;
		static QString const BT_HAB;
		static QString const BT_MUL_DER_QUI;
		static QString const BT_DER_QUI_HAB;
		static QString const BT_ALL;
		static QString const ABRS;

	private:
		Ui::FenPrincipale *ui;
		QString filename;
		bool hasModif;
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
		QStringList abrs;
		void init();
		void update();
		void set_titre();
		void modif();
		void addProgressBar(QString text);
		void removeProgressBar();
		bool confirm_close();
			// Régate
		QString nomRegate;
		QString typeClmt;
		QString typeRt;
		QString typeBt;
		int manchesRetirees;
		int manchesRetireesMin;
		int nbManches;
		int nbEquipages;
		MyMap<int, Equipage> equipages;
		MyMap<int, Manche> manches;
			// Step1.cpp
		void reset_step1();
		void goto_step1();
		void leave_step1();
		int clmt_textToInt(QString text);
		QString clmt_intToText(int nb);
		int rt_textToInt(QString text);
		QString rt_intToText(int nb);
		int bt_textToInt(QString text);
		QString bt_intToText(int nb);
			// Step2.cpp
		QCompleter *completer;
		QMap<QString, Bateau> bateaux;
		QMap<QString, QString> bateauxInv;
		bool stopPropagation;
		void reset_step2();
		void goto_step2();
		void leave_step2();
		void equipages_resize();
		void update_completer();
		void update_nbBateaux();
		QList<QString> add_bateaux(QJsonArray bateaux, QString type);
			// Step3.cpp
		MyMap<int, MyMap<int, TpsWdg>> tpsWdgs;
		int currentManche;
		QTimer *rmMancheTimer;
		void reset_step3();
		void goto_step3();
		void leave_step3();
		void init_step3();
		void add_manche_inputs(int row, int col);
			// Step4.cpp
		void reset_step4();
		void goto_step4();
		void leave_step4();
			// Step5.cpp
		QList<QString> htmls;
		void reset_step5();
		void goto_step5();
		void leave_step5();
		bool verif();
		void calcul_manches();
		void calcul_retirermanches();
		void calcul_general();
		double get_coef(Bateau bt);
		QString formate_tps(int tps);
		QString get_abr(QString text);
			// Classement.cpp
		bool ouvrir(QString name);
		QJsonObject json_compatibilite(QJsonObject obj);
		QJsonArray json_toArray(QJsonObject obj);
		void update_recents();
		void load_recents();
		bool enregistrer(QString name);
		QString get_printed_html(QString html);
		QList<QPixmap> print_getPages();
			// MAJ.cpp
		bool version_greater(QString a, QString b, bool strict = false);
		QNetworkReply *download(QString url, const char *slot);
		bool check_reply(QNetworkReply *reply);
		void update_deri();
		void update_ratings();
		void download_ratings();

	public slots:
		void progression(int nb);

	private slots:
		void on_francais_triggered();
		void on_english_triggered();
		void on_ratings_triggered();
		void on_aide_triggered();
		void on_site_web_triggered();
		void on_propos_triggered();
		void closeEvent(QCloseEvent *event);
			// Step1.cpp
		void on_nomRegate_textChanged(QString text);
		void on_typeClassement_currentIndexChanged(int nb);
		void on_typeRatings_currentIndexChanged(int nb);
		void on_typeBateauxFfv_currentIndexChanged(int nb);
		void on_typeBateauxRya_currentIndexChanged(int nb);
		void on_typeBateauxDeri_currentIndexChanged(int nb);
		void on_btnStep1_clicked();
			// Step2.cpp
		void on_btnStep2_clicked();
		void on_addEquipage_clicked();
		void type_complete(QString type);
		void code_complete(QString code);
		void nom_changed(QString text);
		void code_changed(QString text);
		void type_changed(QString text);
		void deleteEquipage();
			// Step3.cpp
		void on_btnStep3_clicked();
		void on_addManche_clicked();
		void on_rmManche_pressed();
		void manche_focussed(bool hasFocus);
		void h_changed(QString text);
		void min_changed(QString tex);
		void s_changed(QString text);
		void h_pressed(QKeyEvent *ev);
		void min_pressed(QKeyEvent *ev);
		void s_pressed(QKeyEvent *ev);
		void tps_focussed(bool hasFocus);
		void disable_rmManche();
			// Step4.cpp
		void on_btnStep4_clicked();
		void on_manchesRetirees_valueChanged(int val);
		void on_manchesRetireesMin_valueChanged(int val);
			// Step5.cpp
		void on_btnStep5_clicked();
		void on_choisirResultat_currentIndexChanged(int index);
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
		void on_imprimer_triggered();
		void on_pdf_triggered();
		void on_html_triggered();
		void on_quitter_triggered();
		void print(QPrinter *printer);
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