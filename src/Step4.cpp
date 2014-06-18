/*

###################    DériVoile calc' - Français    ###################

Version : v7-2
Date : 2014-06-18
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

Version : v7-2
Date : 2014-06-18
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

void FenPrincipale::reset_step4() {
	ui->manchesRetirees->setValue(this->manchesRetirees);
	ui->manchesRetireesMin->setValue(this->manchesRetireesMin);
	qApp->processEvents();
	this->progression(99);
	qApp->processEvents();
}

void FenPrincipale::on_btnStep4_clicked() {
	this->goto_step4();
}
void FenPrincipale::goto_step4() {
	this->leave_step1();
	this->leave_step2();
	this->leave_step3();
	this->leave_step5();
	ui->btnStep4->setFlat(true);
	ui->step4->setVisible(true);
}
void FenPrincipale::leave_step4() {
	ui->btnStep4->setFlat(false);
	ui->step4->setVisible(false);
}

void FenPrincipale::on_manchesRetirees_valueChanged(int val) {
	this->manchesRetirees = val;
	this->modif();
}

void FenPrincipale::on_manchesRetireesMin_valueChanged(int val) {
	this->manchesRetireesMin = val;
	this->modif();
}