function drawScreen() {
  textSize(18);
  if (isConnected) {
    background(0, 255, 0);
    text('Connected :)', 80, 15);
  } else {
    background(255, 0, 0);
    textAlign(LEFT, TOP);
    text('Disconnected :/', 80, 15);
  }

  text(tempValue, 30, 40);
  text(humiValue, 30, 60);
  text(presValue, 30, 80);
  text(fourValue, 30, 100);
  
  //if(oldColorPickerValue != ledColorPicker.value() && millis()-millisecondTimerStart>50 && isConnected){
  //  oldColorPickerValue = ledColorPicker.value();
  //  sendData("LED Color" + ledColorPicker.value()+ "\n");
  //  millisecondTimerStart = millis();
  //}
  
}
