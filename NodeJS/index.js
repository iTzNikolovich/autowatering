const express = require('express');

const app = express();

const bodyParser = require('body-parser');
app.use(bodyParser.json());

app.use(express.static('public'));

//CONFIG DB
const { KVBase } = require('./KVBase');
let db = new KVBase();

app.get('/', (req, res) => {
	res.send('Connesso!')
});

app.get('/dati/:mac', async (req, res) => {
	const mac = req.params.mac;
	if (mac != '') {
		var data = await db.get(mac);
		console.log('Richiesta GET ricevuta.');
		res.send(data)
	} else {
		res.send('Mac non specificato')
	}
});

//ricerca del vaso con indirizzo mac
app.get('/vasi/:mac', async (req, res) => {
	const mac = req.params.mac;
	// il MAC di Arduino
	if (mac != '') {
		var data = await db.get(mac);
		console.log('READ DATA: ', data);
		res.send('lettura fatta')
	}
	res.status(404).send('Vaso non trovato');
});

app.post('/post', async (req, res) => {
	// RICEVE DATI DA RICHIESTA
	const mac = req.body.mac;
	const temp = req.body.temp;
	const hum = req.body.hum;
	//CREARE OGGETTO DATA NEL DB
	date = Date.now();
	var data = { mac: mac, date: date, temp: temp, hum: hum };
	console.log('Dati ricevuti: ', data);
	await db.set(data.mac, data);
	res.send('Dati ricevuti con successo')
});

app.listen(3000, () => {
	console.log('server started');
});

