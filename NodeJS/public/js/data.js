function sendData() {
	var xhr = new XMLHttpRequest();
	let element = document.getElementById("element").value;
	let url = "https://autowatering.leomirco.repl.co/dati/";
	url = url.concat(element);
	xhr.open('GET', url, true);
	xhr.onreadystatechange = function() {
		if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
			var data = JSON.parse(this.responseText);
			var t = data.temp;
			var h = data.hum;
			var ts = new Date(data.date);
			document.getElementById("temp").innerHTML = t + "°C";
			document.getElementById("hum").innerHTML = h + "%";
			document.getElementById("time").style.visibility = "visible";
			document.getElementById("time").innerHTML = "Ultima richiesta: " + ts.toUTCString();
		}
	};
	xhr.send();
}
