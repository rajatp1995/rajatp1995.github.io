#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define FIREBASE_HOST "projectdatabase-d5dcc.firebaseio.com"
#define FIREBASE_AUTH "6IQZdw009bCIaZic2wJkvGwlYNnPoOosYWVXcToW"
#define WIFI_SSID "Smartcart"
#define WIFI_PASSWORD "12345678"

uint16_t RECV_PIN = 14;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  
  irrecv.enableIRIn();  
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(RECV_PIN);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

String fpath;
String fdir;
char delim[] = ",";
String genpath[50][3];
String token2[50];
String token1[50];
int a1=0,b1=0;

void loop() 
{ 
  if (irrecv.decode(&results)) 
  {
  Serial.print("IR value : ");
  int ab = results.value;
  Serial.println(ab);
  a1=ab/7;
  b1=(ab%7)-1;
  Serial.println(a1);
  Serial.println(b1);
  
  int i=0,j=0;
  FirebaseObject pth = Firebase.get("Path");
  fpath = pth.getString("path");
  int n = fpath.length();
  String path[n];
  char path_array[n+1]; 
  strcpy(path_array, fpath.c_str()); 
  char *token = strtok(path_array,delim);

    while (token)
    {
        token1[i] = token;
        token = strtok(NULL,delim);
        i++;
    }

    int k=0;
    for (int h=0;h<(i-1);h=h+2)
    {
      genpath[k][0]=token1[h];
      genpath[k][1]=token1[h+1];
      k++;
    }

  FirebaseObject dir = Firebase.get("Dir");
  fdir = dir.getString("dir");
  int m = fdir.length();
  String directions[m];
  char dir_array[m+1]; 
  strcpy(dir_array, fdir.c_str()); 
  char *tkn = strtok(dir_array,delim);
  
   while (tkn)
    {
        token2[j] = tkn;
        tkn = strtok(NULL,delim);
        j++;
    }

    for (int r=0;r<j;r++)
    {
      genpath[r][2]=token2[r];
    }

    for (int y=0;y<j;y++)
    {
    Serial.print("Step ");
    Serial.print(y+1);
    Serial.println(" : ");
    Serial.print(genpath[y][0]);
    Serial.print("  ");
    Serial.print(genpath[y][1]);
    Serial.print("  ");
    Serial.println(genpath[y][2]);
    }  
    Serial.println("");
    Serial.println("");

    String a = String(a1);
    String b = String(b1);

    for (int d=0;d<j;d++)
    {
      if (a==genpath[d][0] && b==genpath[d][1])
      {
        if(genpath[d][2]=="1")
        {
          Serial.println("Forward");
        }
        if(genpath[d][2]=="2")
        {
          Serial.println("Left");
        }
        if(genpath[d][2]=="3")
        {
          Serial.println("Right");
        }
        if(genpath[d][2]=="0")
        {
          Serial.println("Reverse");
        }
      }
    }

        irrecv.resume();  // Receive the next value
  }
    
   delay(100);
}

