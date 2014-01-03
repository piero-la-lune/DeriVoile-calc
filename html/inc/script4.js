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


	var version = "v6-5";

	var nbManche = 0;
	var nbEquipage = 0;

	var ratings = {};
	var selects = {ffv: {}, rya:{}};

	var selectTdEquipage = null;
	var oldBateau = "";
	var oldCode = "";
	var btLength = 0;

	var etapeActuelle = 1;

	Number.prototype.fround = function(precision) {
		return Math.round(this*(Math.pow(10, precision))) / Math.pow(10, precision);
	};

$(function() {

var data = $.parseJSON(FenPrincipale.get_ratings());

var bateau = {};
var ffv = null;
var ffvc = null;
var rya = null;
var deri = null;
var SM = null;

ratings.multicoques = [];
selects.ffv.multicoques = [EE("div", "Multicoques")];
selects.rya.multicoques = [EE("div", "Multicoques")];
for (i=0; i<data.multicoques.length; i++) {
	bateau = data.multicoques[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1/bateau.rating).fround(4);
		deri = (1000/(bateau.rating*692.7)).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		deri = (1000/(692.7/bateau.coef)).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings['multicoques'].push({
		id: bateau.code,
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	});
	if (ffv !== null) {
		selects.ffv.multicoques.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
	if (rya !== null) {
		selects.rya.multicoques.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
}
ratings.deriveurs = [];
selects.ffv.deriveurs = [EE("div", "Dériveurs")];
selects.rya.deriveurs = [EE("div", "Dériveurs")];
for (i=0; i<data.deriveurs.length; i++) {
	bateau = data.deriveurs[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1000/bateau.rating).fround(4);
		ffvc = (768/bateau.rating).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.rating/1000, 0.913))).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		ffvc = (768*bateau.coef/1000).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.coef, -0.913))).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings['deriveurs'].push({
		id: bateau.code,
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	});
	if (ffv !== null) {
		selects.ffv.deriveurs.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
	if (rya !== null) {
		selects.rya.deriveurs.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
}
ratings.quillards = [];
selects.ffv.quillards = [EE("div", "Quillards de sport")];
selects.rya.quillards = [EE("div", "Quillards de sport")];
for (i=0; i<data.quillards.length; i++) {
	bateau = data.quillards[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1000/bateau.rating).fround(4);
		ffvc = (768/bateau.rating).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.rating/1000, 0.913))).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		ffvc = (768*bateau.coef/1000).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.coef, -0.913))).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings['quillards'].push({
		id: bateau.code,
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	});
	if (ffv !== null) {
		selects.ffv.quillards.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
	if (rya !== null) {
		selects.rya.quillards.push(
			EE("span", {
				"%id":bateau.code,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
}
ratings.habitables = [];
selects.ffv.habitables = [EE("div", "Habitables")];
selects.rya.habitables = [EE("div", "Habitables")];
for (i=0; i<data.habitables.length; i++) {
	bateau = data.habitables[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	SM = null;
	if (bateau.groupe <= 11) { SM = 290-bateau.groupe*15; }
	else if (bateau.groupe <= 30) { SM = 235-bateau.groupe*10; }
	else if (bateau.groupe <= 45) { SM = 175-bateau.groupe*8; }
	else { SM = 85-bateau.groupe*6; }
	ffv = (600/(SM+683)).fround(4);
	ffvc = ffv;
	deri = (1000/(-11.742*bateau.groupe+1163.1)).fround(4);
	ratings['habitables'].push({
		id: "#"+bateau.id,
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri,
		cvl: bateau.cvl
	});
	if (ffv !== null) {
		selects.ffv.habitables.push(
			EE("span", {
				"%id":"#"+bateau.id,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
	if (rya !== null) {
		selects.ffv.habitables.push(
			EE("span", {
				"%id":"#"+bateau.id,
				"%match":bateau.serie.toLowerCase(),
				"@class":"visible"
			}, bateau.serie)
		);
	}
}

$.setSelectBateaux = function() {
	var bateaux = [];
	var typeRatings = $("#typeRatings").get("value");
	var typeBateaux = $("#typeBateaux").get("value");
	if (typeRatings == "rya") {
		if (typeBateaux == "multicoques") { bateaux = selects.rya.multicoques; }
		else if (typeBateaux == "deriveurs") { bateaux = selects.rya.deriveurs; }
		else if (typeBateaux == "quillards") { bateaux = selects.rya.quillards; }
		else {
			bateaux = selects.rya.multicoques
				.concat(selects.rya.deriveurs, selects.rya.quillards);
		}
	}
	else {
		if (typeRatings == "ffv") {
			if (typeBateaux == "multicoques") { bateaux = selects.ffv.multicoques; }
			else if (typeBateaux == "deriveurs") { bateaux = selects.ffv.deriveurs; }
			else if (typeBateaux == "quillards") { bateaux = selects.ffv.quillards; }
			else if (typeBateaux == "habitables") { bateaux = selects.ffv.habitables; }
			else {
				bateaux = selects.ffv.deriveurs
					.concat(selects.ffv.quillards, selects.ffv.habitables);
			}
		}
		else {
			if (typeBateaux == "deriveurs_multicoques") {
				bateaux = selects.ffv.multicoques
					.concat(selects.ffv.deriveurs, selects.ffv.quillards);
			}
			else {
				bateaux = selects.ffv.multicoques
					.concat(selects.ffv.deriveurs, selects.ffv.quillards, selects.ffv.habitables);
			}
		}
	}
	$(".selectBateau").fill(bateaux);
};

$.getTpsInputs = function() {
	return EE("td", [
		EE("span", {"@class":"tps"}, [
			EE("input", {"@type":"text", "@class":"h", "@placeholder":"h", "@pattern":"[0-9]*"}),
			EE("span", {"@class":"sep"}, " : "),
			EE("input", {"@type":"text", "@class":"min", "@placeholder":"min", "@pattern":"[0-9]*"}),
			EE("span", {"@class":"sep"}, " : "),
			EE("input", {"@type":"text", "@class":"s", "@placeholder":"s", "@pattern":"[0-9]*"})
		]),
		EE("span", {"@class":"place"},
			EE("input", {"type":"text", "@class":"pl", "@pattern":"[0-9]*"})
		)
	]);
};

$.getEquipageInputs = function() {
	return EE("td", {"@class": "eqgInputs"}, [
		EE("input", {"@type":"text", "@class":"non", "@placeholder":"Nom de l'équipage"}),
		EE("span", {"@class":"rm rmEquipage", "@title":"Supprimer cet équipage"}, "x", function(e) {
			e.on("click", function() {
				$(this).trav("parentNode", "tr").remove();
				nbEquipage--;
			});
		}),
		EE("br"),
		EE("input", {"@type":"text", "@class":"tpBateau", "placeholder":"rechercher bateau..."}, [],function(e) {
			e.on("|focus", function() {
				selectTdEquipage = e.trav("parentNode", "td");
				var pos = $$(e).getBoundingClientRect();
				$$(".selectBateau").style.left = pos.left+"px";
				$$(".selectBateau").style.top = (pos.bottom)+"px";
				$(".selectBateau").set("$display", "block");
				oldBateau = e.get("value");
				oldCode = selectTdEquipage.select(".rating").get("value");
				selectTdEquipage.select(".rating").set("value", "");
				e.trigger("keyup");
			});
			e.on("|blur", function() {
				$$(".selectBateau").scrollTop = 0;
				$(".selectBateau").set("$display", "none");
				if (oldBateau == e.get("value") &&
					selectTdEquipage.select(".rating").get("value") === ""
				) { // No change
					selectTdEquipage.select(".rating").set("value", oldCode);
				}
			});
			e.on("|keydown", function() {
				btLength = e.get("value").length;
			});
			e.on("|keyup", function() {
				var val = e.get("value").toLowerCase();
				if (val.length > 1) {
					if (val.length > btLength) {
						$(".selectBateau .visible").each(function(i) {
							if ($(i).get("%match").indexOf(val) === -1) {
								i.className = "hidden";
							}
						});
					}
					else {
						$(".selectBateau .hidden").each(function(i) {
							if ($(i).get("%match").indexOf(val) !== -1) {
								i.className = "visible";
							}
						});
					}
				}
				else {
					$(".selectBateau .hidden").each(function(e) {
						e.className = "visible";
					});
				}
			});
		}),
		EE("input", {"@type":"text", "@class":"rating", "@placeholder":"Code"})
	]);
};

$.getMancheHead = function(no) {
	return EE("td", [
		"M"+no,
		EE("span", {"@class":"rm rmManche", "@title":"Supprimer cette manche"}, "x", function(e) {
			e.on("click", function() {
				$("#etape2 tbody tr td, #etape2 tfoot tr td").each(function(e, index) {
					if ((index%(nbManche+1)) == no) { $(e).remove(); }
				});
				$($("#etape2 thead td")[nbManche]).remove();
				$($("#showManche option")[nbManche]).remove();
				nbManche--;
				/*if ($("#showManche").val() != 'all') { $("#showManche").val('1').change(); }*/
			});
		})
	]);
};

$.getManchePick = function(no) {
	return EE("option", {"@value":no}, "la manche n°"+no);
};

$.addEquipage = function(nb) {
	var tds = [$.getEquipageInputs()];
	for (i=0; i<nbManche; i++) { tds.push($.getTpsInputs()); }
	var trs = [];
	for (i=0; i<nb; i++) {
		nbEquipage++;
		trs.push(EE("tr", tds));
	}
	$("#etape2 table tbody").add(trs);
	/*if ($("#showManche").val() != 'all') { $("#showManche").change(); }*/
};

$.addManche = function(nb) {
	var tds = [];
	var tdsHead = [];
	var opts = [];
	for (i=0; i<nb; i++) {
		nbManche++;
		tdsHead.push($.getMancheHead(nbManche));
		tds.push($.getTpsInputs());
		opts.push($.getManchePick(nbManche));
	}
	$("#etape2 thead tr").add(tdsHead);
	$("#etape2 tbody tr").add(tds);
	$($("#etape2 tfoot tr")[0]).add(tds);
	$("#showManche").add(opts);
	/*$(window).resize();
	if ($("#showManche").val() != 'all') { $("#showManche").val(nbManche).change(); }*/
};

$.gotoEtape = function(etape) {
	if (etapeActuelle == etape) { return false; }
	$("body").set("$", "-etape1 -etape2 -etape3 -etape4 +etape"+etape);
	etapeActuelle = etape;
	//$.resizeTable();
	//FenPrincipale.setEtape(etape);
};

$(".navEtapes li").on("click", function(e, index) {
	$.gotoEtape(index+1);
});






/* ########################## Étape 1 ########################## */

$("#typeClassement").on("change", function() {
	if ($(this).get("value") == 'temps') {
		$("#etapes").set("$", "-clmScratch +clmTemps");
	}
	else {
		$("#etapes").set("$", "-clmTemps +clmScratch");
	}
	/*$("#showManche").set("value", "all").change();*/
});

$.setSelectBateaux();
$("#typeRatings").on("change", $.setSelectBateaux);
$("#typeBateaux").on("change", $.setSelectBateaux);




/* ########################## Étape 2 ########################## */

$(".selectBateau").on("mousedown", function(event) {
	var e = $(event.srcElement);
	if (e.get("%id") !== null) {
		selectTdEquipage.select(".tpBateau").set("value", e.text());
		selectTdEquipage.select(".rating").set("value", e.get("%id"));
		return true; // .tpBateau will loose focus
	}
});

$.addManche(1);
$.addEquipage(1);
$("#addManche").on("click", function() { $.addManche(1); });
$("#addEquipage").on("click", function() { $.addEquipage(1); });

});
