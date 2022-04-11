function connectToBle() {
  // Connect to a device by passing the service UUID
  blueTooth.connect(0xFFE0, gotCharacteristics);
}


// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) { 
    console.log('error: ', error);
  }
  console.log('characteristics: ', characteristics);
  blueToothCharacteristic = characteristics[0];

  blueTooth.startNotifications(blueToothCharacteristic, gotValue, 'string');
  
  isConnected = blueTooth.isConnected();
  // Add a event handler when the device is disconnected
  blueTooth.onDisconnected(onDisconnected);
}


// A function that will be called once got values
function gotValue(value) {
  if(value.substring(0,1) == "T"){
    console.log('Temperature Value: ', value);
    tempValue = value;
  } else if(value.substring(0,1) == "H"){
    console.log('Humidity Value: ', value);
    humiValue = value;
  } else if(value.substring(0,1) == "P"){
    console.log('Pressure Value: ', value);
    presValue = value;
  }else if(value.substring(0,1) == "F"){
    console.log('Fourth Value: ', value);
    fourValue = value;
  }
}

function onDisconnected() {
  console.log('Device got disconnected.');
  isConnected = false;
}


function sendData(command) {
  const inputValue = command;
  if (!("TextEncoder" in window)) {
    console.log("Sorry, this browser does not support TextEncoder...");
  }
  var enc = new TextEncoder(); // always utf-8
  blueToothCharacteristic.writeValue(enc.encode(inputValue));
}
