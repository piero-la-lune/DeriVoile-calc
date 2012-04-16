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

	var version = "@v";

	var ratings = new Array();
	var recents = new Array();
	var selects = new Array();
	var select = 'deriveurs';
	var placeholder = 'min';

	var nbManche = 1;
	var nbEquipage = 0;

	var etapeActuelle;

(function($) {

	$.exporterHTML = function() {
var style = "\
body {\
	font-family: 'OpenSans', 'Trebuchet MS', Tahoma, Helvetica, arial, Sans-Serif;\
	text-align: center;\
}\
h1 { text-align: center; }\
#contenu { padding: 0 40px; }\
a {\
	color: #005FA3;\
	text-decoration: none;\
}\
a:hover {\
	text-decoration: underline;\
}\
table {\
	margin: auto;\
	border: 1px solid #C9D7F1;\
	border-collapse: collapse;\
	color: #005FA3;\
}\
thead td {\
	background-color: #D9E4F8;\
	font-weight: bold;\
}\
td {\
	border: 1px solid #C9D7F1;\
	text-align: center;\
	padding: 5px; \
}";
var clone = $("#etape4").clone();
clone.find("#resultsSmall").remove();
clone.find("#results").show();
		FenPrincipale.exporterHTML('<html><head><meta charset="utf-8" /><style>'+style+'</style></head><body>'+clone.html()+'</body></html>');
	}

	$.enregistrer = function() {
		var donnees = new Object();
		donnees['nomRegate'] = $("#nomRegate").val();
		donnees['nbEquipages'] = nbEquipage;
		donnees['nbManches'] = nbManche;
		donnees['bateaux'] = select;
		donnees['classement'] = $("#typeClassement").val();
		donnees['manchesRetirees'] = $("#manchesRetirees").val();
		donnees['manches'] = new Object();
		for (i=0; i<nbManche; i++) {
			donnees['manches'][i] = {
				h: $("#etape2 table tfoot .h").eq(i).val(),
				min: $("#etape2 table tfoot .min").eq(i).val(),
				s: $("#etape2 table tfoot .s").eq(i).val()
			};
		}
		donnees['equipages'] = new Object();
		$("#etape2 table tbody tr").each(function(i) {
			donnees['equipages'][i] = new Object();
			donnees['equipages'][i]['nom'] = $(this).find(".nom").val();
			donnees['equipages'][i]['bateau'] = $(this).find("select").val();
			donnees['equipages'][i]['rating'] = $(this).find(".rating").val();
			donnees['equipages'][i]['manches'] = new Object();
			for (j=0; j<nbManche; j++) {
				donnees['equipages'][i]['manches'][j] = {
					h: $(this).find(".h").eq(j).val(),
					min: $(this).find(".min").eq(j).val(),
					s: $(this).find(".s").eq(j).val()
				};
			}
		});
		var texte = JSON.stringify(donnees);
		FenPrincipale.enregistrer(texte);
		return texte;
	};

	$.ouvrir = function(texte) {
		var donnees;
		var nb = 0;
		var tr;

			// Vérification et ouverture du fichier
			ouverture1 = function(texte) {
		FenPrincipale.progression(1);
		if (!texte || texte == '') { FenPrincipale.ouvrir('Le fichier est vide.'); return false; }
		FenPrincipale.progression(2);
		donnees = JSON.parse(texte);
		if (!donnees) {
			FenPrincipale.ouvrir('Erreur lors de la lecture du classement : vérifiez que le fichier est valide...');
			return false;
		}
		else if (donnees['erreur'] !== undefined) {
			FenPrincipale.ouvrir(donnees['erreur']);
			return false;
		}
		FenPrincipale.progression(3);
		setTimeout(function() { ouverture2(); }, 0);
			};

			// Remise à zéro
			ouverture2 = function() {
		$("#etape2 table tbody").html(' ');
		$("#etape2 table thead").html('<tr><td>Équipage</td></tr>');
		$("#etape2 table tfoot td:gt(0)").remove();
		$("#showManche option:gt(0)").remove();
			// Récupération des données principales
		$("#nomRegate").val((donnees['nomRegate'] === undefined) ? '' : donnees['nomRegate']);
		$("#typeBateaux").val((donnees['bateaux'] === undefined) ? 'deriveurs' : donnees['bateaux']).change();
		$("#typeClassement").val((donnees['classement'] === undefined) ? 'temps' : donnees['classement']).change();
		$("#manchesRetirees").val((donnees['manchesRetirees'] === undefined) ? '0' : donnees['manchesRetirees']);
		FenPrincipale.progression(4);
		nbManche = 0;
		nb = (donnees['nbManches'] === undefined) ? 0 : donnees['nbManches']; // car nbManche est augmenté dans $.addManche
		nbEquipage = (donnees['nbEquipages'] === undefined) ? 0 : donnees['nbEquipages'];
		setTimeout(function() { ouverture3(0); }, 0);
			};

			// Ajout du bon nombre de manches
			ouverture3 = function(i) {
		$.addManche();
		if (donnees['manches'][i]['h'] !== undefined) { $("#etape2 table tfoot .h").eq(i).val(donnees['manches'][i]['h']); }
		if (donnees['manches'][i]['min'] !== undefined) { $("#etape2 table tfoot .min").eq(i).val(donnees['manches'][i]['min']); }
		if (donnees['manches'][i]['s'] !== undefined) { $("#etape2 table tfoot .s").eq(i).val(donnees['manches'][i]['s']); }
		FenPrincipale.progression(parseInt((i+1)*16/nb)+4);
		if (++i < nb) { setTimeout(function() { ouverture3(i); }, 0); }
		else { setTimeout(function() { ouverture4(); }, 0); }
			};

			// Ajout du bon nombre d'équipages
			ouverture4 = function() {
		var tab = ''; var txt = $.getEquipage();
		for (var i=0; i<nbEquipage; i++) { tab += txt; }
		$("#etape2 table tbody").html(tab);
		tr = $("#etape2 table tbody tr:eq(0)");
		setTimeout(function() { ouverture5(0); }, 0);
			};

			// Entrée des informations sur les équipages
			ouverture5 = function(i) {
		var e = donnees['equipages'][i];
		tr.find(".nom").val((e['nom'] === undefined) ? '' : e['nom']);
		tr.find(".bateau").val((e['bateau'] === undefined) ? '' : e['bateau']);
		tr.find(".rating").val((e['rating'] === undefined) ? '' : e['rating']);
		for (var j=0; j<nbManche; j++) {
			tr.find(".h").eq(j).val((e['manches'][j]['h'] === undefined) ? '' : e['manches'][j]['h']);
			tr.find(".min").eq(j).val((e['manches'][j]['min'] === undefined) ? '' : e['manches'][j]['min']);
			tr.find(".s").eq(j).val((e['manches'][j]['s'] === undefined) ? '' : e['manches'][j]['s']);
		}
		tr = tr.next();
		FenPrincipale.progression(parseInt((i+1)*78/nbEquipage)+20);
		if (++i < nbEquipage) { setTimeout(function() { ouverture5(i); }, 0); }
		else { setTimeout(function() { ouverture6(); }, 0); }
			};

			ouverture6 = function() {
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); } // Si le classement est au scratch
		FenPrincipale.progression(100);
		FenPrincipale.ouvrir("");
		$("#etape0 input").click();
		return true;
			};
		
		setTimeout(function() { ouverture1(texte); }, 0);
	};

	$.getEquipage = function() {
		var ligne = '<tr><td><input type="text" value="" class="nom" placeholder="Nom de l\'équipage" /><a href="#" title="Supprimer cet équipage" class="removeEquipage">w</a><span class="tpBateau">'+$.getSelect()+'<input type="text" value="1,000" class="rating" /></span></td>';
		for (i=0; i<nbManche; i++) { ligne += $.getManche(placeholder); }
		ligne += '</tr>';
		return ligne;
	};

	$.getManche = function(txt) {
		return '<td class="tps"><input type="text" value="" class="h" size="2" placeholder="h" /><br /><input type="text" value="" class="min" size="3" placeholder="'+txt+'" /><br /><input type="text" value="" class="s" size="2" placeholder="s" /></td>';
	};

	$.addEquipage = function() {
		nbEquipage++;
		$("#etape2 table tbody").append($.getEquipage);
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); }
		if ($("#showManche").val() != 'all') { $("#showManche").change(); }
	};

	$.removeEquipage = function(num) {
		nbEquipage--;
		$("#etape2 tbody tr").eq(num).remove();
	};

	$.addManche = function() {
		nbManche++;
		$("#etape2 tbody tr").each(function() {
			$(this).append($.getManche(placeholder));
		});
		$("#etape2 thead tr").append('<td>M'+nbManche+'<a href="#" title="Supprimer cette manche" class="removeManche">w</a></td>');
		$("#etape2 tfoot tr").append($.getManche('min'));
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); }
		$("#showManche").append('<option value="'+nbManche+'">la manche n°'+nbManche+'</option>');
		$(window).resize();
		if ($("#showManche").val() != 'all') { $("#showManche").val(nbManche).change(); }
	};
	
	$.removeManche = function(num) {
		nbManche--;
		$("#etape2 thead td").eq(num).remove();
		$("#etape2 tbody tr").each(function() {
			$(this).find("td").eq(num).remove();
		});
		$("#etape2 tfoot td").eq(num-1).remove();
		var i = 1;
		$("#etape2 thead td:gt(0)").each(function() {
			$(this).html('M'+i+'<a href="#" title="Supprimer cette manche" class="removeManche">w</a>');
			i++;
		});
		if ($("#showManche").val() != 'all') { $("#showManche").val('1').change(); }
		$("#showManche option:last").remove();
	};

	$.gotoEtape = function(etape) {
		if (etapeActuelle.attr('id') == 'etape'+etape) { return false; }
		var a = $(".navEtapes a:eq("+(parseInt(etape)-1)+")");
		$("#etapes").css('height', $("#etapes").height());
		etapeActuelle.fadeOut('fast', function() {
			etapeActuelle = $("#etape"+etape);
			$("#etapes").animate({height: $("#etape"+etape).height()});
			etapeActuelle.fadeIn(function() {
				$("#etapes").css('height', 'auto');
				$.resizeTable();
			});
		});
		$("#etape2 ol").hide();
		$(".navEtapes a").removeClass('selected');
		a.addClass('selected');
		FenPrincipale.setEtape(etape);
	};

	$.getSelect = function() {
		var selection = '<select><option value="default" selected="selected">Type de bateau :</option>';
		if (recents['default'] !== undefined) { rct = recents['default']; } else { rct = ''; }
		selection += '<optgroup label="Récents :">'+rct+'</optgroup>';
		selection += '<optgroup label="Autres :"><option value="autre">Rentrer un coefficient :</option></optgroup>';
		selection += '<optgroup label="Tous :">'+selects[select]+'</optgroup>';
		return selection;
	};

	$.resizeTable = function() {
		if (etapeActuelle.attr('id') == 'etape4') {
			t = etapeActuelle.find("#results");
			if (t.css('display') != 'none') { t.attr('data-width', t.outerWidth()); }
			if (etapeActuelle.innerWidth() < parseInt(t.attr('data-width'))) {
				$("#results").hide();
				$("#resultsSmall").show();
				$("#manches table").addClass('restricted');
			}
			else {
				$("#resultsSmall").hide();
				$("#results").show();
				$("#manches table").removeClass('restricted');
			}
		}
		else if (etapeActuelle.attr('id') == 'etape2') {
			t = etapeActuelle.find("table");
			if ($("#showManche").val() == 'all' && etapeActuelle.innerWidth() < t.outerWidth()) {
				$("#showManche").val(nbManche).change();
			}
		}
	};

})(jQuery)

$(document).ready(function() {

	etapeActuelle = $("#etape0");

	$("#ratingsDeriveurs tbody tr").each(function() {
		var code = $(this).children().eq(0).html();
		selects['deriveurs'] += '<option value="'+code+'">'+code+' ('+$(this).children().eq(1).html()+')</option>';
		ratings[code] = $(this).children().eq(4).html();
	});
	$("#ratingsMulticoques tbody tr").each(function() {
		var code = $(this).children().eq(0).html();
		selects['multicoques'] += '<option value="'+code+'">'+code+'</option>';
		ratings[code] = 1/$(this).children().eq(1).html();
	});
	$.addEquipage();


/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ NAVIGATION -------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(".navEtapes a:eq(0)").click(function() { $.gotoEtape('1'); return false; });
$(".navEtapes a:eq(1)").click(function() { $.gotoEtape('2'); return false; });
$(".navEtapes a:eq(2)").click(function() { $.gotoEtape('3'); return false; });

$("#etape0 input").click(function() {
	$(".navEtapes").slideDown();
	$(".btCalculer").slideDown();
	$.gotoEtape('1'); return false;
});



/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ FORMULAIRE -------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$("#formulaire select").live('change', function() { FenPrincipale.modif(); });
$("#formulaire input").live('keyup', function() { FenPrincipale.modif(); });

$("#addEquipage").click(function() {
	$.addEquipage();
	return false;
});

$("#addManche").click(function() {
	$.addManche();
	return false;
});

$(".removeEquipage").live('click', function() {
	if (nbEquipage < 2) { return false; }
	$.removeEquipage($(this).parent().parent().index());
	return false;
});

$(".removeManche").live('click', function() {
	if (nbManche < 2) { return false; }
	$.removeManche($(this).parent().index());
	return false;
});

$("#calculer").click(function() {
	FenPrincipale.calculer();
});

$("tbody .tps input").live('keyup', function() {
	if ($(this).val().length >= 2 && !isNaN($(this).val())) {
		if ($(this).index() < 2) {
			$(this).next().focus();
		}
		else if ($(this).parents("tr").index() < nbEquipage-1) {
			$(this).parents("tr").next().find("td").eq($(this).parent().index()).find("input").eq(0).focus();
		}
		else {
			$(this).parents("tbody").find("tr").eq(0).find("td").eq($(this).parent().index()+1).find("input").eq(0).focus();
		}
	}
});

$("#etape2 table select").live('change', function() {
	if (recents[$(this).val()] === undefined && $(this).val() != 'default' && $(this).val() != 'autre') {
		var txt = '<option value="'+$(this).val()+'">'+$(this).find(':selected').text()+'</option>';
		$("#etape2 select").each(function() { $(this).find("optgroup:eq(0)").prepend(txt); });
		$(this).find("optgroup:eq(0) option:eq(0)").attr("selected","selected");
		recents[$(this).val()] = true;
		recents['default'] += txt;
	}
	if ($(this).val() == 'autre') {
		$(this).parent().find(".rating").show();
	}
	else {
		$(this).parent().find(".rating").hide();
	}
});

$("#etape2 a:eq(0)").click(function() {
	var val = $("#typeClassement").val();
	if (val == 'temps') {
		$("#etape2 ol:eq(0)").toggle();
	}
	else {
		$("#etape2 ol:eq(1)").toggle();
	}
	return false
});
$("#typeClassement").change(function() {
	var val = $(this).val();
	if (val == 'temps') {
		$("#pTypeBateau").show();
		$("#etape2 table tfoot").show();
		$("#etape2 table .tpBateau").show();
		$("#etape2 table .tps").find("*").show();
		placeholder = 'min';
		$("#etape2 table .tps").find(".min").attr('placeholder', placeholder);
	}
	else {
		$("#pTypeBateau").hide();
		$("#etape2 table tfoot").hide();
		$("#etape2 table .tpBateau").hide();
		$("#etape2 table .tps").find("*:not(.min)").hide();
		placeholder = 'n°';
		$("#etape2 table .tps").find(".min").attr('placeholder', placeholder);
	}
	$("#showManche").val('all').change();
});
$("#typeBateaux").change(function() {
	select = $(this).val();
	recents = new Array();
	$("#etape2 table select").remove();
	$("#etape2 table tbody tr").each(function() {
		$(this).find(".tpBateau").prepend($.getSelect());
		$(this).find(".rating").hide();
	});
});
$("#showManche").change(function() {
	if ($(this).val() == 'all') {
		$("#etape2 table .tps").show();
		$("#etape2 table thead td:gt(0)").show();
	}
	else {
		var nb = parseInt($(this).val());
		$("#etape2 table tr .tps").hide();
		$("#etape2 table thead td:gt(0)").hide();
		$("#etape2 table tbody tr").each(function() { $(this).find(".tps").eq(nb-1).show(); });
		$("#etape2 table thead td").eq(nb).show();
		$("#etape2 table tfoot .tps").eq(nb-1).show()
	}
});



/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------- POPUP ---------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(".question").click(function() {
	$(this).next(".reponse").toggle();
});



/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------- TABLES --------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(window).bind("orientationchange resize", function(){
	$.resizeTable();
});



});