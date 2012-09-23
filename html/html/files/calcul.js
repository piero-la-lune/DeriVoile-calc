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


function Calcul() {
	var obj = this;
	this.ratings = ratings;
	this.manches = new Array();
	this.equipages = new Array();
	this.manchesRetirees = 0;
	this.nbManche = nbManche;
	this.nbEquipage = nbEquipage;
	this.nomRegate = '';
	this.abrs = ['DNC', 'DNS', 'OCS', 'BFD', 'DNF', 'RAF', 'DSQ'];
	this.abrs2 = ['362', '367', '627', '233', '363', '723', '377'];



		// ----------------------------------- Initialisation des variables ----------------------------------- //
	this.initialisation = function() {

FenPrincipale.progression(1);
$("#etape2 td").removeClass('probleme');

this.manchesRetirees = parseInt($("#manchesRetirees").val(), 10);
if (isNaN(this.manchesRetirees) || this.manchesRetirees >= this.nbManche) { this.manchesRetirees = 0; }
this.nomRegate = $("#nomRegate").val();

FenPrincipale.progression(2);
setTimeout(function() { obj.verification(0); }, 0);

	};



		// --------------------- Vérification des champs du formulaire ---------------------//
	this.verification = function(index) {

var tr = $("#etape2 table tbody tr").eq(index);

var nom = tr.find(".nom").val();
if (nom == '') {
	FenPrincipale.calculM('Vous devez donner un nom à tous les équipages.');
	tr.find("td:eq(0)").addClass('probleme');
	$.gotoEtape('2');
	return false;
}

var l = this.equipages.length;
for (i=0; i<l; i++) {
	if (this.equipages[i]['nom'] == nom) {
		FenPrincipale.calculM('Deux équipages ne peuvent pas avoir le même nom.');
		tr.find("td:eq(0)").addClass('probleme');
		$("#etape2 table tbody tr").eq(i).find("td:eq(0)").addClass('probleme');
		$.gotoEtape('2');
		return false;
	}
}

var type = tr.find("select").val();
var typeClassement = $("#typeClassement").val();
if (typeClassement != 'temps') {
	coef = 1;
}
else if (type == 'autre') {
	var reg = new RegExp("([,]+)", "g");
	var coef = parseFloat(tr.find(".rating").val().replace(reg, "."));
	if (isNaN(coef)) { coef = 1; }
}
else {
	var coef = parseFloat(this.ratings[type]);
}
if (isNaN(coef)) {
	FenPrincipale.calculM('Vous devez renseigner tous les types de bateau.');
	tr.find("td:eq(0)").addClass('probleme');
	$.gotoEtape('2');
	return false;
}

this.equipages[index] = new Array();
this.equipages[index]['nom'] = nom;
this.equipages[index]['type'] = type;
this.equipages[index]['coef'] = coef;
this.equipages[index]['points'] = 0;
this.equipages[index]['pointsTries'] = new Array();
this.equipages[index]['pointsOrdonnes'] = new Array();
this.equipages[index]['pointsRetires'] = new Array();


for (i=0; i<this.nbManche; i++) {
	if (index == 0) { this.manches[i] = new Array(); }
	this.manches[i][index] = new Array;
	
	var a1 = $.inArray(tr.find(".h").eq(i).val(), this.abrs2);
	var a2 = $.inArray(tr.find(".min").eq(i).val(), this.abrs2);
	var a3 = $.inArray(tr.find(".s").eq(i).val(), this.abrs2);
	if (a1 >= 0) {
		tr.find(".h").eq(i).val('');
		tr.find(".min").eq(i).val(this.abrs[a1]);
		tr.find(".s").eq(i).val('');
	}
	else if (a2 >= 0) {
		tr.find(".h").eq(i).val('');
		tr.find(".min").eq(i).val(this.abrs[a2]);
		tr.find(".s").eq(i).val('');
	}
	else if (a3 >= 0) {
		tr.find(".h").eq(i).val('');
		tr.find(".min").eq(i).val(this.abrs[a3]);
		tr.find(".s").eq(i).val('');
	}
	var h = parseInt(tr.find(".h").eq(i).val(), 10)*60*60;
	if (isNaN(h)) { h = 0; }
	var m = parseInt(tr.find(".min").eq(i).val(), 10)*60;
	if (isNaN(m)) { m = 0; }
	var s = parseInt(tr.find(".s").eq(i).val(), 10);
	if (isNaN(s)) { s = 0; }
	var tot = h+m+s;
	var h = parseInt($("#etape2 table tfoot .h").eq(i).val(), 10)*60*60;
	if (isNaN(h)) {  h = 0; }
	var m = parseInt($("#etape2 table tfoot .min").eq(i).val(), 10)*60;
	if (isNaN(m)) {  m = 0; }
	var s = parseInt($("#etape2 table tfoot .s").eq(i).val(), 10);
	if (isNaN(s)) {  s = 0; }
	var heure = h+m+s;
	var tps = tot-heure;
	if (tps < 0) {
		FenPrincipale.calculM('L\'heure de départ indiquée pour la manche n°'+(i+1)+' est après l\'heure d\'arrivée de l\'équipage nommé "'+tr.find(".nom").val()+'", ce qui est impossible ! Merci de corriger ce problème.');
		$("#etape2 tfoot td").eq(i+1).addClass('probleme');
		tr.find("td").eq(i+1).addClass('probleme');
		$.gotoEtape('2');
		return false;
	}
	if (tps == 0 || tot == 0) {
		var abr = 'DNF';
		if ($.inArray(tr.find(".h").eq(i).val(), this.abrs) >= 0) { abr = tr.find(".h").eq(i).val(); }
		else if ($.inArray(tr.find(".min").eq(i).val(), this.abrs) >= 0) { abr = tr.find(".min").eq(i).val(); }
		else if ($.inArray(tr.find(".s").eq(i).val(), this.abrs) >= 0) { abr = tr.find(".s").eq(i).val(); }
		this.manches[i][index]['reel'] = abr;
		this.manches[i][index]['compense'] = abr;
		this.manches[i][index]['points'] = this.nbEquipage+1;
	}
	else {
		this.manches[i][index]['reel'] = tps;
		this.manches[i][index]['compense'] = tps*coef;
		this.manches[i][index]['points'] = 0;
	}
}

FenPrincipale.progression(parseInt((index+1)*38/this.nbEquipage)+2);

if (++index < this.nbEquipage){
	setTimeout(function() { obj.verification(index); }, 0);
}
else {
	setTimeout(function() { obj.initialisationTextes(); }, 0);
}

	};



		// ----------------------------------- Initialisation des textes ----------------------------------- //
	this.initialisationTextes = function() {

$("#manches").html('');
$("#etape4 #results thead tr").html('<td>Position</td><td style="min-width:200px">Équipage</td><td>Bateau</td><td>Points</td>');
for (i=0; i<this.nbManche; i++) {
	$("#etape4 #results thead tr").append('<td>M'+(i+1)+'</td>');
	$("#manches").append('<p>&nbsp;</p><h2>Manche n°'+(i+1)+'</h2><table><thead><tr><td>Position</td><td style="min-width:200px">Équipage</td><td>Points</td><td>Temps réel</td><td>Temps compensé</td></tr></thead><tbody></tbody></table><p>&nbsp;</p>');
}
$("#etape4 #results tbody").html('');
$("#etape4 #resultsSmall tbody").html('');
$("#etape4 #results").show()
$("#etape4 #resultsSmall").hide();
$("#etape4 h2 span").html(this.nomRegate);
var stats = this.nbEquipage+' équipages classés.';
if (this.nbManche < 2) { stats +=  ' '+this.nbManche+' manche courue'; }
else { stats +=  ' '+this.nbManche+' manches courues'; }
if (this.manchesRetirees == 0) { stats += ' (aucune manche retirée).'; }
else if (this.manchesRetirees == 1) { stats += ' (1 manche retirée).'; }
else { stats += ' ('+this.manchesRetirees+' manches retirées).'; }
$("#etape4 #stats").html(stats);

setTimeout(function() { obj.pointsManches(0); }, 0);

	};



		// ------------------------- Affichage des résultats de chaque manche ------------------------------ //
	this.pointsManches = function(i) {

for (j=0; j<this.nbEquipage; j++) {
	var min = 999999999999;
	var id = new Array();
	for (k=0; k<this.nbEquipage; k++) { // Récupération des id de(s) meilleur(s) équipage(s) (à égalité) non encore traités
		if (this.manches[i][k]['points'] == 0 && this.manches[i][k]['compense'] < min) {
			min = this.manches[i][k]['compense'];
			id = [k];
		}
		else if (this.manches[i][k]['points'] == 0 && this.manches[i][k]['compense'] == min) {
			id.push(k);
		}
	}
	if (id.length == 0) { break; }
	for (k=0; k<id.length; k++) { // Calcul/Ajout des points pour ces équipages
		this.manches[i][id[k]]['points'] = (id.length-1)/2+j+1;
		$("#manches table").eq(i).find('tbody').append('<tr><td data-label="Position"><b>'+(j+1)+'</b></td><td data-label="Équipage">'+this.equipages[id[k]]['nom']+'</td><td data-label="Points">'+this.manches[i][id[k]]['points']+'</td><td data-label="Tps réel">'+this.formateTemps(this.manches[i][id[k]]['reel'])+'</td><td data-label="Tps compensé">'+this.formateTemps(this.manches[i][id[k]]['compense'])+'</td></tr>');
		this.equipages[id[k]]['points'] += this.manches[i][id[k]]['points'];
		this.equipages[id[k]]['pointsOrdonnes'].unshift(this.manches[i][id[k]]['points']);
		this.addArraySort(this.manches[i][id[k]]['points'], this.equipages[id[k]]['pointsTries']);
	}
	j = j+id.length-1;
}
for (j=0; j<this.nbEquipage; j++) {  // Traitement des équipages sans temps (DNF, DNC, BFD, ...)
	if ($.inArray(this.manches[i][j]['reel'], this.abrs) >= 0) {
		$("#manches table").eq(i).find('tbody').append('<tr><td data-label="Position"><b>-</b></td><td data-label="Équipage">'+this.equipages[j]['nom']+'</td><td data-label="Points">'+(this.nbEquipage+1)+'</td><td data-label="Temps réel">'+this.manches[i][j]['reel']+'</td><td data-label="Temps compensé">'+this.manches[i][j]['reel']+'</td></tr>');
		this.equipages[j]['points'] += (this.nbEquipage+1);
		this.equipages[j]['pointsOrdonnes'].unshift(this.nbEquipage+1);
		this.equipages[j]['pointsTries'].push(this.nbEquipage+1);
	}
}

FenPrincipale.progression(parseInt((i+1)*20/this.nbManche)+40);

if (++i < this.nbManche) {
	setTimeout(function() { obj.pointsManches(i); }, 0);
}
else {
	setTimeout(function() { obj.retirerManches(0); }, 0);
}

	};



		// ----------------------------------- Traitement du nombre de manches retirées ----------------------------------- //
	this.retirerManches = function(i) {

for (j=0; j<this.manchesRetirees; j++) {
	this.equipages[i]['points'] -= this.equipages[i]['pointsTries'][this.nbManche-j-1];
	this.equipages[i]['pointsRetires'].push(this.equipages[i]['pointsTries'][this.nbManche-j-1]);
}
this.equipages[i]['pointsTries'] = this.equipages[i]['pointsTries'].slice(0, this.equipages[i]['pointsTries'].length-this.manchesRetirees);
FenPrincipale.progression(parseInt((i+1)*10/this.nbEquipage)+60);

if (++i < this.nbEquipage) {
	setTimeout(function() { obj.retirerManches(i); }, 0);
}
else {
	setTimeout(function() { obj.classement(0); }, 0);
}

	};



		// ----------------------------------- Calcul et affichage du classement final ----------------------------------- //
	this.classement = function(i) {

var min = 999999999999;
var id;
for (j=0; j<this.nbEquipage; j++) {
	if (this.equipages[j]['points'] == 0) { continue; } // Cet équipage est déjà dans le classement final
	if (this.equipages[j]['points'] < min) {
		min = this.equipages[j]['points'];
		id = j;
	}
	else if (this.equipages[j]['points'] == min) {
		for (k=0; k<this.equipages[j]['pointsTries'].length; k++) {
			if (this.equipages[j]['pointsTries'][k] < this.equipages[id]['pointsTries'][k]) {
				min = this.equipages[j]['points'];
				id = j;
				break;
			}
			else if (this.equipages[j]['pointsTries'][k] > this.equipages[id]['pointsTries'][k]) { break; }
			else if (k == this.equipages[j]['pointsTries'].length-1) {
				for (m=0; m<this.equipages[j]['pointsOrdonnes'].length; m++) {
					if (this.equipages[j]['pointsOrdonnes'][m] < this.equipages[id]['pointsOrdonnes'][m]) {
						min = this.equipages[j]['points'];
						id = j;
						break;
					}
					else if (this.equipages[j]['pointsOrdonnes'][m] > this.equipages[id]['pointsOrdonnes'][m]) { break; }
					else if (m == this.equipages[j]['pointsOrdonnes'].length-1) {
						alert('Il y a une égalité parfaite de points entre deux équipages. Ces équipages devraient normalement se retrouver à égalité dans le classement final, mais DériVoile calc\' ne sais pas gérer cela : un équipage sera donc classé après l\'autre, bien que les deux méritent la même place.');
					}
				}
			}
		}
	}
}
var texte = '<tr><td>'+(i+1)+'</td><td>'+this.equipages[id]['nom']+'</td><td>'+this.equipages[id]['type']+'</td><td>'+this.equipages[id]['points']+'</td>';
var texte2 = '<tr><td>'+(i+1)+'.</td><td>'+this.equipages[id]['nom']+'<span> ('+this.equipages[id]['type']+')</span></td><td>'+this.equipages[id]['points']+' points (';
// var points = new Array();
for (j=0; j<this.nbManche; j++) {
	var t = '';
	if (this.manches[j][id]['points'] == this.nbEquipage+1) { t = this.manches[j][id]['reel']; }
	else { t = this.manches[j][id]['points']; }
	for (k=0; k<this.manchesRetirees; k++) {
		if (this.manches[j][id]['points'] == this.equipages[id]['pointsRetires'][k]) {
			t = '('+t+')';
			this.equipages[id]['pointsRetires'][k] = 0;
			break;
		}
	}
// 	points.push(t);
	texte += '<td>'+t+'</td>';
	if (j < this.nbManche-1) { texte2 += t+', '; }
	else { texte2 += t+')</td>'; }
}
$("#etape4 #results tbody").append(texte+'</tr>');
$("#etape4 #resultsSmall tbody").append(texte2+'</tr>');
this.equipages[id]['points'] = 0;

FenPrincipale.progression(parseInt((i+1)*28/nbEquipage)+70);

if (++i < this.nbEquipage) {
	setTimeout(function() { obj.classement(i); }, 0);
}
else {
	setTimeout(function() { obj.affichage(); }, 0);
}

	};



		// ----------------------------------- Affichage des résultats ----------------------------------- //
	this.affichage = function() {
if ($("#typeClassement").val() != 'temps') {
	$("#etape4 #manches table tr").each(function() { $(this).find("td:gt(2)").hide(); });
	$("#etape4 #results tr").each(function() { $(this).find("td:eq(2)").hide(); });
	$("#etape4 #resultsSmall span").hide();
}
FenPrincipale.progression(100);
FenPrincipale.calculM('');
$.gotoEtape('4');
return true;
	};
	
	this.formateTemps = function(tps) {
		var h = parseInt(tps/3600);
		if (h/10 < 1) { h = '0'+h; }
		var min = parseInt((tps-h*3600)/60);
		if (min/10 < 1) { min = '0'+min; }
		var s = parseInt(tps-h*3600-min*60);
		if (s/10 < 1) { s = '0'+s; }
		return h+':'+min+':'+s;
	};

	this.addArraySort = function(nb, array) {
		for (l=0; l<array.length; l++) {
			if (nb <= array[l]) {
				array.splice(l,0,nb);
				break;
			}
			else if (l == array.length-1) {
				array.push(nb);
				break;
			}
		}
		if (array.length == 0) { array.push(nb); }
		return array;
	};

};