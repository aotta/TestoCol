//  file: pebble-js-app.js
//  auth: Matthew Clark, SetPebble

// change this token for your project
var setPebbleToken = 'A9EU';
var parametri;
console.log('start pebble-js');

Pebble.addEventListener('ready', function(e) {
console.log('ready');
//	var key = e.payload.action;
	
//  if (typeof(key) != 'undefined') {
    console.log('leggo i parametri');
		var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try {
				console.log('setting:' + settings);
				Pebble.sendAppMessage(JSON.parse(settings));
				console.log('inviato: '+JSON.parse(settings));
      } catch (e) {
      }
    }
    var request = new XMLHttpRequest();
    request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try {
            Pebble.sendAppMessage(JSON.parse(request.responseText));
						console.log('send:'+ JSON.parse(request.responseText));
          } catch (e) {
          }
    }
    request.send(null);
  //}
});

Pebble.addEventListener('appmessage', function(e) {
	console.log('appmessage');
	var key = e.payload.action;
	
  if (typeof(key) != 'undefined') {
    console.log('leggo i parametri');
		var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try {
				console.log('setting:' + settings);
				Pebble.sendAppMessage(JSON.parse(settings));
				console.log('inviato: '+JSON.parse(settings));
      } catch (e) {
      }
    }
    var request = new XMLHttpRequest();
    request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try {
            Pebble.sendAppMessage(JSON.parse(request.responseText));
						console.log('send:'+ JSON.parse(request.responseText));
          } catch (e) {
          }
    }
    request.send(null);
  }
});
Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('http://x.SetPebble.com/' + setPebbleToken + '/' + Pebble.getAccountToken());
	console.log('URL aperto');
});
Pebble.addEventListener('webviewclosed', function(e) {
  console.log('webclosed event');
	if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
		parametri=decodeURIComponent(e.response);
		parametri = JSON.parse(parametri);
		console.log('parametri: '+parametri);
		try {
      Pebble.sendAppMessage(parametri);
      localStorage.setItem(setPebbleToken, parametri);
			console.log('setItem:'+ setPebbleToken + parametri);
    } catch(e) {
    }
  }
});
