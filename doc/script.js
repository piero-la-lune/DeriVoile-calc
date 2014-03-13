function load(url) {
	window.scrollTo(0, 0);
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'qrc:/doc/'+url, false);
	xhr.send(null);
	document.querySelector("article").innerHTML = xhr.responseText;
	var as = document.querySelectorAll("a");
	for (var i=0; i<as.length; i++) {
		if (as[i].href == "javascript:load('"+url+"');") {
			as[i].className = 'selected';
		}
		else {
			as[i].className = '';
		}
	}
}