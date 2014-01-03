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


function Calcul() {

	var obj = this;
	this.ratings = ratings;
	this.manches = [];
	this.equipages = [];
	this.typeClassement = $("#typeClassement").get("value");
	this.typeRatings = $("#typeRatings").get("value");
	this.typeBateaux = $("#typeBateaux").get("value");
	this.manchesRetirees = 0;
	this.nbManche = nbManche;
	this.nbEquipage = nbEquipage;
	this.nomRegate = "";
	this.abrs = ["DNC", "DNS", "OCS", "BFD", "DNF", "RAF", "DSQ"];
	this.abrs2 = ["362", "367", "627", "233", "363", "723", "377"];


		// Initialisation des variables
	this.initialisation = function() {

FenPrincipale.progression(1);

	// On retire les anciens problèmes détectés
$("#etape2 td").set("$", "-probleme");

	// Le nombre de manches à retirer
obj.manchesRetirees = parseInt($("#manchesRetirees").get("value"), 10);
if (isNaN(obj.manchesRetirees) || obj.manchesRetirees >= obj.nbManche) {
	obj.manchesRetirees = 0;
}

	// Le nom de la régate
obj.nomRegate = $("#nomRegate").get("value");

FenPrincipale.progression(2);

setTimeout(obj.verification, 0);

	};



	var i = 0;

		// Vérification des champs de la table
	this.verification = function() {

var tr = $($("#etape2 table tbody tr")[i]);

var nom = tr.select(".nom").get("value");
if (nom === "") {
	FenPrincipale.calculer_callback("Vous devez donner un nom à tous les équipages.");
	tr.select(".eqgInputs").set("$", "+probleme");
	$.gotoEtape(2);
	return false;
}

var len = obj.equipages.length;
for (var j=0; j<len; j++) {
	if (obj.equipages[j].nom == nom) {
		FenPrincipale.calculer_callback("Deux équipages ne peuvent pas avoir le même nom.");
		tr.select(".eqgInputs").set("$", "+probleme");
		$($(".eqgInputs")[j]).set("$", "+probleme");
		$.gotoEtape(2);
		return false;
	}
}

var coef = 1;
var aff = "";
if (obj.typeClassement == "temps") {
	var code = tr.select(".rating").get("value");
	var tpBateau = $.isBateauOk(code);
	if (tpBateau !== false) {
		// coef ne devrait jamais valoir `null`
		if (obj.typeRatings == "ffv") {
			if (obj.typeBateaux == "deriveurs_habitables") {
				coef = obj.ratings[code]["ffvc"];
			}
			else {
				coef = obj.ratings[code]["ffv"];
			}
		}
		else if (obj.typeRatings == "rya") {
			coef = obj.ratings[code]["rya"];
		}
		else {
			coef = obj.ratings[code]["deri"];
		}
		aff = tpBateau;
	}
	else if (code.match(/[0-9](\.|,)[0-9]*$/) !== null) {
		coef = parseFloat(code.replace(/,/, "."));
		if (isNaN(coef)) { coef = 1; }
		aff = "coef : "+coef.toString().replace(/\./, ",");
	}
	else {
		FenPrincipale.calculer_callback("Vous devez renseigner tous les types de bateau.");
		tr.select(".eqgInputs").set("$", "+probleme");
		$.gotoEtape(2);
		return false;
	}
	if (isNaN(coef)) {
		FenPrincipale.calculer_callback("Vous devez renseigner tous les types de bateau.");
		tr.select(".eqgInputs").set("$", "+probleme");
		$.gotoEtape(2);
		return false;
	}
}

obj.equipages[i] = {
	nom: nom,
	aff: aff,
	coef: coef,
	points: 0,
	pointsTries: [],
	pointsOrdonnes: [],
	pointsRetires: []
};

for (j=0; j<obj.nbManche; j++) {
	if (i === 0) { obj.manches[j] = []; }

	if (obj.typeClassement == "scratch") {
		var pl = $(tr.select(".pl")[j]).get("value").toUpperCase();
		if (obj.inArray(pl, obj.abrs) !== false) {
			obj.manches[j][i] = {
				reel: pl,
				compense: pl,
				points: obj.nbEquipage+1
			};
		}
		else {
			var index = obj.inArray(pl, obj.abrs2);
			if (index !== false) {
				obj.manches[j][i] = {
					reel: obj.abrs[index],
					compense: obj.abrs[index],
					points: obj.nbEquipage+1
				};
			}
			else {
				pl = parseInt(pl, 10);
				if (!isNaN(pl)) {
					obj.manches[j][i] = {
						reel: pl,
						compense: pl,
						points: 0
					};
				}
				else {
					obj.manches[j][i] = {
						reel: "DNF",
						compense: "DNF",
						points: obj.nbEquipage+1
					};
				}
			}
		}
	}
	else {
		var h = $(tr.select(".h")[j]).get("value").toUpperCase();
		var min = $(tr.select(".min")[j]).get("value").toUpperCase();
		var s = $(tr.select(".s")[j]).get("value").toUpperCase();
		if (obj.inArray(h, obj.abrs) !== false) {
			obj.manches[j][i] = {
				reel: h,
				compense: h,
				points: obj.nbEquipage+1
			};
		}
		else if (obj.inArray(min, obj.abrs) !== false) {
			obj.manches[j][i] = {
				reel: min,
				compense: min,
				points: obj.nbEquipage+1
			};
		}
		else if (obj.inArray(s, obj.abrs) !== false) {
			obj.manches[j][i] = {
				reel: s,
				compense: s,
				points: obj.nbEquipage+1
			};
		}
		else {
			var index1 = obj.inArray(h, obj.abrs2);
			var index2 = obj.inArray(min, obj.abrs2);
			var index3 = obj.inArray(s, obj.abrs2);
			if (index1 !== false) {
				obj.manches[j][i] = {
					reel: obj.abrs[index1],
					compense: obj.abrs[index1],
					points: obj.nbEquipage+1
				};
			}
			else if (index2 !== false) {
				obj.manches[j][i] = {
					reel: obj.abrs[index2],
					compense: obj.abrs[index2],
					points: obj.nbEquipage+1
				};
			}
			else if (index3 !== false) {
				obj.manches[j][i] = {
					reel: obj.abrs[index3],
					compense: obj.abrs[index3],
					points: obj.nbEquipage+1
				};
			}
			else {
				h = parseInt(h, 10);
				min = parseInt(min, 10);
				s = parseInt(s, 10);
				if (isNaN(h)) { h = 0; }
				if (isNaN(min)) { min = 0; }
				if (isNaN(s)) { s = 0; }
				if (h === 0 && min === 0 && s === 0) {
					obj.manches[j][i] = {
						reel: "DNF",
						compense: "DNF",
						points: obj.nbEquipage+1
					};
				}
				else {
					var h2 = parseInt(
						$($("#etape2 table tfoot .h")[j]).get("value"), 10
					);
					var min2 = parseInt(
						$($("#etape2 table tfoot .min")[j]).get("value"), 10
					);
					var s2 = parseInt(
						$($("#etape2 table tfoot .s")[j]).get("value"), 10
					);
					if (isNaN(h2)) { h2 = 0; }
					if (isNaN(min2)) { min2 = 0; }
					if (isNaN(s2)) { s2 = 0; }
					tps = (h*60*60+min*60+s) - (h2*60*60+min2*60+s2);
					if (tps <= 0) {
						FenPrincipale.calculer_callback("L'heure de départ indiquée pour la manche n°"+(j+1)+" est après l'heure d'arrivée de l'équipage nommé « "+nom+" », ce qui est impossible ! Merci de corriger ce problème.");
						$($("#etape2 table tfoot .tpsInputs")[j])
							.set("$", "+probleme");
						$(tr.select(".tpsInputs")[j]).set("$", "+probleme");
						$.gotoEtape(2);
						return false;
					}
					else {
						obj.manches[j][i] = {
							reel: tps,
							compense: tps*coef,
							points: 0
						};
					}
				}
			}
		}
	}
}

FenPrincipale.progression(parseInt((i+1)*38/obj.nbEquipage, 10)+2);

if (++i < obj.nbEquipage){
	setTimeout(obj.verification, 0);
}
else {
	setTimeout(obj.initialisationTextes, 0);
}

	};



		// Initialisation des textes
	this.initialisationTextes = function() {

var tds = [
	EE("td", "Place"),
	EE("td", "Équipage"),
	EE("td", "Points")
];
var manches = [];
var opts = [];
for (var j=0; j<obj.nbManche; j++) {
	tds.push(EE("td", "M"+(j+1)));
	manches.push(EE("div", {"className": "clm hidden", "id": "manche"+(j+1)}, [
		EE("h2", "Manche n°"+(j+1)),
		EE("table", [
			EE("thead", [
				EE("tr", [
					EE("td", "Place"),
					EE("td", "Équipage"),
					EE("td", "Points"),
					EE("td", {"className": "temps"}, "Tps réel"),
					EE("td", {"className": "temps"}, "Tps compensé")
				])
			]),
			EE("tbody")
		])
	]));
	opts.push(EE("option",
		{"value": "manche"+(j+1), "className": "toBeRemoved"},
	"Manche n°"+(j+1)));
}

$("#showResult .toBeRemoved").remove();
$("#showResult").add(opts);

var stats = obj.nbEquipage+" équipages classés.";
if (obj.nbManche < 2) { stats += " "+obj.nbManche+" manche courue"; }
else { stats += " "+obj.nbManche+" manches courues"; }
if (obj.manchesRetirees === 0) { stats += " (aucune manche retirée)."; }
else if (obj.manchesRetirees == 1) { stats += " (1 manche retirée)."; }
else { stats += " ("+obj.manchesRetirees+" manches retirées)."; }

$(".clm").remove();

$("#results").add(manches);
$("#results").addFront(EE("div", {"className": "clm visible", "id": "general"}, [
	EE("p", {"id": "stats"}, stats),
	EE("table", {"className": "normal"}, [
		EE("thead", [
			EE("tr", tds)
		]),
		EE("tbody")
	])
]));

if (obj.nomRegate !== "") {
	$("#general").addFront(EE("h2", [
		"Résultats de la régate",
		EE("br"),
		"« "+obj.nomRegate+" »"
	]));
}
else {
	$("#general").addFront(EE("h2", "Résultats de la régate"));
}

i = 0;
setTimeout(obj.pointsManches, 0);

	};



		// Affichage des résultats de chaque manche 
	this.pointsManches = function() {

// Traitement des équipages avec un temps
for (var j=0; j<obj.nbEquipage; j++) {
	var min = 999999999999;
	var id = [];
	for (var k=0; k<obj.nbEquipage; k++) {
		// Récupération des id de(s) meilleur(s) équipage(s) (à égalité)
		// non encore traités
		if (obj.manches[i][k].points === 0) {
			// L'équipage k a correctement terminé la manche i
			if (obj.manches[i][k].compense < min) {
				// Il a un temps inférieur à celui de ceux déjà parcourus
				min = obj.manches[i][k].compense;
				id = [k];
			}
			else if (obj.manches[i][k].compense == min) {
				// Il a un temps égal à celui d'un équipage déjà parcouru
				id.push(k);
			}
		}
	}
	if (id.length === 0) {
		// Peut se produire lorsque certains équipages n'ont pas de temps
		// (DNF, DSQ, ...) : tous les équipages ont été parcourus alors que la
		// boucle n'est pas encore terminée.
		break;
	}
	for (k=0; k<id.length; k++) {
		// On calcule les points pour cet équipage
		var points = (id.length-1)/2+j+1;
		obj.manches[i][id[k]].points = points;
		obj.equipages[id[k]].points += points;
		obj.equipages[id[k]].pointsOrdonnes.unshift(points);
		obj.addArraySort(points, obj.equipages[id[k]].pointsTries);
		// On affiche ce résultat
		var classe = "pos";
		var place = j+1;
		if (id.length > 1) {
			if (k > 0) {
				classe += " b-top";
				place = "";
			}
			if (k < (id.length - 1)) {
				classe += " b-bottom";
			}
		}
		$("#manche"+(i+1)+" tbody").add(EE("tr", [
			EE("td", {"className": classe}, place),
			EE("td", {"className": "eqg"}, [
				obj.equipages[id[k]].nom,
				EE("span", {"className": "bateau"}, obj.equipages[id[k]].aff)
			]),
			EE("td", points),
			EE("td", {"className": "temps"},
				obj.formateTemps(obj.manches[i][id[k]].reel)),
			EE("td", {"className": "temps"},
				obj.formateTemps(obj.manches[i][id[k]].compense))
		]));
	}
	j = j+id.length-1;
}
// Traitement des équipages sans temps (DNF, DNC, BFD, ...)
for (j=0; j<obj.nbEquipage; j++) {
	if (isNaN(obj.manches[i][j].reel)) {
		// On calcule les points pour cet équipage
		obj.equipages[j].points += (obj.nbEquipage+1);
		obj.equipages[j].pointsOrdonnes.unshift(obj.nbEquipage+1);
		obj.addArraySort((obj.nbEquipage+1), obj.equipages[j].pointsTries);
		// On affiche ce résultat
		$("#manche"+(i+1)+" tbody").add(EE("tr", [
			EE("td", {"className": "pos"}, obj.manches[i][j].reel),
			EE("td", {"className": "eqg"}, [
				obj.equipages[j].nom,
				EE("span", {"className": "bateau"}, obj.equipages[j].aff)
			]),
			EE("td", (obj.nbEquipage+1)),
			EE("td", {"className": "temps"}, obj.manches[i][j].reel),
			EE("td", {"className": "temps"}, obj.manches[i][j].reel)
		]));
	}
}

FenPrincipale.progression(parseInt((i+1)*20/obj.nbManche, 10)+40);

if (++i < obj.nbManche) {
	setTimeout(obj.pointsManches, 0);
}
else {
	i = 0;
	setTimeout(obj.retirerManches, 0);
}

	};



		//  Traitement du nombre de manches retirées
	this.retirerManches = function() {

var points = 0;
for (var j=0; j<obj.manchesRetirees; j++) {
	// On récupère et supprime le dernier élément
	points = obj.equipages[i].pointsTries.pop();
	obj.equipages[i].points -= points;
	obj.equipages[i].pointsRetires.push(points);
}

FenPrincipale.progression(parseInt((i+1)*10/obj.nbEquipage, 10)+60);

if (++i < obj.nbEquipage) {
	setTimeout(obj.retirerManches, 0);
}
else {
	i = 0;
	setTimeout(obj.classement, 0);
}

	};



		// Calcul et affichage du classement final
	this.classement = function() {

// Recherche du (des) équipage(s) suivant à afficher
var min = 999999999999;
var id = 0;
var ids = [];
for (var j=0; j<obj.nbEquipage; j++) {
	if (obj.equipages[j].points === 0) {
		// Cet équipage est déjà dans le classement final
		continue;
	}
	if (obj.equipages[j].points < min) {
		min = obj.equipages[j].points;
		id = j;
	}
	else if (obj.equipages[j].points == min) {
		for (var k=0; k<(obj.nbManche-obj.manchesRetirees); k++) {
			if (obj.equipages[j].pointsTries[k] <
				obj.equipages[id].pointsTries[k]
			) {
				id = j; // Pas besoin de mettre à jour `min`
				break;
			}
			else if (obj.equipages[j].pointsTries[k] >
				obj.equipages[id].pointsTries[k]
			) {
				break;
			}
			else if (k == (obj.nbManche-obj.manchesRetirees-1)) {
				for (var l=0; l<obj.nbManche; l++) {
					if (obj.equipages[j].pointsOrdonnes[l] <
						obj.equipages[id].pointsOrdonnes[l]
					) {
						id = j; // Pas besoin de mettre à jour `min`
						break;
					}
					else if (obj.equipages[j].pointsOrdonnes[l] >
						obj.equipages[id].pointsOrdonnes[l]
					) {
						break;
					}
					else if (l == obj.nbManche-1) {
						// Égalité parfaite
						ids.push(j);
					}
				}
			}
		}
	}
}

// Affichage du (des) équipages dans le classement final
ids.push(id);
for (j=0; j<ids.length; j++) {
	var classe = "pos";
	var place = i+1;
	if (ids.length > 1) {
		if (j > 0) {
			classe += " b-top";
			place = "";
		}
		if (j < (ids.length - 1)) {
			classe += " b-bottom";
		}
	}
	var tr = [
		EE("td", {"className": classe}, place),
		EE("td", {"className": "eqg"}, [
			obj.equipages[ids[j]].nom,
			EE("span", {"className": "bateau"}, obj.equipages[ids[j]].aff)
		]),
		EE("td", obj.equipages[ids[j]].points)
	];
	for (var m=0; m<obj.nbManche; m++) {
		var t = "";
		if (obj.manches[m][ids[j]].points == obj.nbEquipage+1) {
			t = obj.manches[m][ids[j]].reel;
		}
		else {
			t = obj.manches[m][ids[j]].points;
		}
		for (var n=0; n<obj.manchesRetirees; n++) {
			if (obj.manches[m][ids[j]].points ==
				obj.equipages[ids[j]].pointsRetires[n]
			) {
				t = EE("span", {"className": "rm-pts"}, t);
				obj.equipages[ids[j]].pointsRetires[n] = 0;
				break;
			}
		}
		tr.push(EE("td", t));
	}
	$("#general tbody").add(EE("tr", tr));
	obj.equipages[ids[j]].points = 0;
}

FenPrincipale.progression(parseInt((i+1)*28/obj.nbEquipage, 10)+70);

i = i+ids.length;
if (i < obj.nbEquipage) {
	setTimeout(obj.classement, 0);
}
else {
	setTimeout(obj.affichage, 0);
}

	};



		// Affichage des résultats
	this.affichage = function() {

if (obj.typeClassement != "temps") {
	$(".temps").set("$display", "none");
	$(".bateau").set("$display", "none");
}

FenPrincipale.progression(100);

FenPrincipale.calculer_callback("");

$.gotoEtape(4);

	};

	this.inArray = function(e, arr) {
		for (var j=0; j<arr.length; j++) {
			if (arr[j] == e) { return j; }
		}
		return false;
	};

	this.formateTemps = function(tps) {
		var h = parseInt(tps/3600, 10);
		if (h/10 < 1) { h = "0"+h; }
		var min = parseInt((tps-h*3600)/60, 10);
		if (min/10 < 1) { min = "0"+min; }
		var s = parseInt(tps-h*3600-min*60, 10);
		if (s/10 < 1) { s = "0"+s; }
		return h+":"+min+":"+s;
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
		if (array.length === 0) { array.push(nb); }
		return array;
	};

}