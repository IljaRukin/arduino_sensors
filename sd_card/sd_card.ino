#include <SD.h>
#include <SPI.h>

int pinCS = 53; //Pin 10 (Slave Pin)
File outFile; //instance for writing
File inFile; //instance for reading
String filename = "text.txt"; //file name
char content[] = "content to write";

void setup() {
  //setup once
  pinMode(pinCS, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect
  }
  Serial.println("start");

  /*open communication*/
  if (SD.begin()){
    //initialized
    Serial.println("initialization success");
  }
  else {
    //failed
    Serial.println("initialization failed");
    return; //close setup
  }


  /*write*/
  outFile = SD.open(filename, FILE_WRITE);//write open
  if (outFile) {
    //opened for write
    outFile.println(content);//writing
    Serial.println("write successful");
    outFile.close();//close
  }
  else {
    //failed
    Serial.println("file open failed");
  }



  /*read*/
  inFile = SD.open(filename, FILE_READ);//read open
  if (inFile) {
    //opened for read
    while (inFile.available()) {
      Serial.write(inFile.read());//get content by columns
    }
    inFile.close();//close
  }
  else {
      Serial.println("read file failed");
  }
}

void loop(){
}
