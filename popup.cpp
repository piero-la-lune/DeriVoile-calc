/*

#-#-#-#-#-#-#-#-#-#-#-#    DériVoile calc'    #-#-#-#-#-#-#-#-#-#-#-#

VERSION : @v
DATE : @date
LICENCE : ./COPYING
SITE WEB : http://calc.derivoile.fr

Copyright (C) 2011-2012 Pierre Monchalin, http://derivoile.fr
Vous êtes libre de distribuer et modifier ce programme.
Pensez à me laisser un petit mot si vous distribuez une copie !

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "popup.h"
#include "ui_popup.h"

popup::popup(QString url, QString titre, QWidget *parent) : QDialog(parent), ui(new Ui::popup) {
	ui->setupUi(this);

	QHBoxLayout *layout = new QHBoxLayout;

	QWebView *popup = new QWebView;
	popup->load(QUrl(url));
	popup->setGeometry(0,0,700,500);
	layout->addWidget(popup);

	setLayout(layout);
	setWindowTitle(titre);
}

popup::~popup() {
	delete ui;
}
