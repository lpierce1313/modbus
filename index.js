//Required Modules
var express = require('express');
var app = express();
var port = process.env.PORT || 3000;
var bodyParser = require('body-parser');
var urlencodedParser = bodyParser.urlencoded({extended: false});
var addon = require('./cpp/modbus/build/Release/addon');

//Modbus Data
var modbusData =
  {
    comPort: "",
    parity: "1",
    speed: "9600",
    code: "3",
    unitAdd: "",
    startReg: "",
    numReg: "",
    format: "i",
    result: []
  };

//Sets styling engine to jade
app.use(express.static('public'));
app.set('view engine', 'ejs');

//Not sure!?
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

//App pages for web
app.get('/', function (req, res) {
  // res.send('Hello world!');
  res.render('index')
});

app.get('/help', function (req, res) {
  // res.send('Hello world!');
  res.render('help')
});

app.get('/about', function (req, res) {
  // res.send('Hello world!');
  res.render('about')
});

app.get('/modbus', function (req, res) {
  // res.send('Hello world!');
  res.render('modbus', {data: modbusData})
});

app.post('/modbus', urlencodedParser, function (req, res) {
  // res.send('Hello world!');
  //console.log(req.body);
  modbusData = req.body;
  // console.log(modbusData);
  //console.log(req.body);
  //If nothing was input since these are optional and have default vals
  if(!modbusData.code) { modbusData.code = "3"; }
  if(!modbusData.speed) { modbusData.speed = "9600"; }
  if(!modbusData.parity) { modbusData.parity = "1"; }

  if(!modbusData.format) { modbusData.format = "i"; }
  if(!modbusData.sign) { modbusData.sign = "s"; }
  if(!modbusData.bit) { modbusData.bit = "16"; }

  switch (modbusData.format){
    case 'a':
    modbusData.dataType = "Ascii";
    break;
    case 'i':
    modbusData.dataType = "Integer";
    break;
    case 'b':
    modbusData.dataType = "Byte";
    break;
    case 'h':
    modbusData.dataType = "Hex";
    break;
  }
  // console.log(modbusData);

  //Untouched
  if(modbusData.sign == "s" && modbusData.bit == 16){
    switch(modbusData.format){
      case 'a':
        modbusData.format = 'a';
        break;
      case 'i':
        modbusData.format = 'i';
        break;
      case 'h':
        modbusData.format = 'h';
        break;
      case 'b':
        modbusData.format = 'b';
        break;
    }
  }
  //B,L,I to caps, only change h when 32 bit
  else if(modbusData.sign == "u" && modbusData.bit == 16){
    switch(modbusData.format){
      case 'a':
        modbusData.format = 'a';
        break;
      case 'i':
        modbusData.format = 'I';
        break;
      case 'h':
        modbusData.format = 'h';
        break;
      case 'b':
        modbusData.format = 'B';
        break;
      }
  }
  else if(modbusData.sign == "s" && modbusData.bit == 32){
    switch(modbusData.format){
      case 'a':
        modbusData.format = 'a';
        break;
      case 'i':
        modbusData.format = 'l';
        break;
      case 'h':
        modbusData.format = 'H';
        break;
      case 'b':
        modbusData.format = 'b';
        break;
    }
  }
  else if(modbusData.sign == "u" && modbusData.bit == 32){
    switch(modbusData.format){
      case 'a':
        modbusData.format = 'a';
        break;
      case 'i':
        modbusData.format = 'L';
        break;
      case 'h':
        modbusData.format = 'H';
        break;
      case 'b':
        modbusData.format = 'B';
        break;
     }
  }

  //Save to an array
  modbusData.result = addon.modbus(modbusData.comPort,modbusData.parity,
    modbusData.speed,modbusData.code,modbusData.unitAdd,modbusData.startReg,
    modbusData.numReg, modbusData.format);

    console.log(modbusData);

  //Render the page to modbus success once the submit button is pressed
  res.render('modbus-success', {data: modbusData});
});

//Load server on localhost 3000
app.listen(port, function() {
    console.log('Our app is running on http://localhost:' + port);
});
