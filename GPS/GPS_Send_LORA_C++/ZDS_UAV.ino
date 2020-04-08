
#include <SD_t3.h>
#include <SD.h>
#include "Datenlogger.h"
#include "TinyGPS_mod.h"


TinyGPS gps;
//Datalog* datensatz;
Datalog datensatz;
// 
// ------------------------------------
#define gpsPort Serial5
#define LoRaPort Serial3
#define Probe_serial Serial2

int chipSelect = BUILTIN_SDCARD;        // 



static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS& gps);
static void print_str(const char* str, int len);

/*
typedef union {
    char charbinary_c;
    byte binary_c[1];
} binaryByte_c;

typedef union {
    byte bytebinary_h;
    byte binary_h[1];
} binaryByte_h;

typedef union {
    byte bytebinary_m;
    byte binary_m[1];
} binaryByte_m;

typedef union {
    byte bytebinary_s;
    byte binary_s[1];
} binaryByte_s;

typedef union {
    float floatingPoint_lat;
    byte binary_lat[4];
} binaryFloat_lat;

typedef union {
    float floatingPoint_lon;
    byte binary_lon[4];
} binaryFloat_lon;

typedef union {
    float floatingPoint_alt;
    byte binary_alt[4];
} binaryFloat_alt;


/*
binaryByte_c start_char;
binaryFloat_lat u_flat;
binaryFloat_lon u_flon;
binaryFloat_alt u_alt;
binaryByte_h u_hour;
binaryByte_m u_minute;
binaryByte_s u_second;


typedef struct dataset
{
    byte hour;
    byte minute;
    byte second;
    byte latitude;
    byte longitude;
    byte altitude;
};*/

typedef struct dataset
{
    char start_char;
    byte hour;
    byte minute;
    byte second;
    float latitude;
    float longitude;
    float altitude;
};

dataset serial_dataset;
byte testHour = 0;
byte testMinute = 0;
byte testSecond = 0;
float testLat = 0.0;
float testLong = 0.0;
float testAlt = 0.0;
unsigned char i = 0;


void setup()
{
    Serial.begin(115200);
    gpsPort.begin(9600);
    LoRaPort.begin(9600);
    //Probe_serial.begin(115200);

    datensatz.init(chipSelect);

   

    //start_char.charbinary_c = '$';

    //Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
    //Serial.println("by Mikal Hart");
    //Serial.println();
    //Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
    //Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
    //Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
}

void loop()
{
    float flat, flon, alt;
    unsigned long age, chars = 0;
    char sz[8];
    //unsigned short sentences = 0, failed = 0;
    //static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
    int year;
    byte month, day, hour, minute, second, hundredths;
    //float month, day, hour, minute, second, hundredths;
    //unsigned long age;

    //print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
    //print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
    gps.f_get_position(&flat, &flon, &age);
    //print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
    //print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    //print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
    //print_date(gps);
    //print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
    //print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
    //print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
    //print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
    //print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
    //print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
    //print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

    //gps.stats(&chars, &sentences, &failed);
    //print_int(chars, 0xFFFFFFFF, 6);
    //print_int(sentences, 0xFFFFFFFF, 10);
    //print_int(failed, 0xFFFFFFFF, 9);
    Serial.println();
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
   
    Serial.println('$');
    sprintf(sz, "%02d:%02d:%02d", hour, minute, second);
    Serial.println(sz);
    //Serial.println(String(hour) + ":" + String(minute) + ":" + String(second));

    
    Serial.println(flat,DEC);
    Serial.println(flon,DEC);
    Serial.println(gps.f_altitude());
    alt = gps.f_altitude();
    //Serial.println( );
    
    /*
    u_hour.bytebinary_h = hour;
    u_minute.bytebinary_m = minute;
    u_second.bytebinary_s = second;
    u_flat.floatingPoint_lat = flat;
    u_flon.floatingPoint_lon = flon;
    u_alt.floatingPoint_alt = alt;
    */

    testSecond += 1;
    if(testSecond == 60){
      testSecond = 0;
      testMinute += 1;
    }
    if(testMinute == 60){
      testMinute = 0;
      testHour += 1;
    }
    if(testHour == 24) testHour = 0;

    if(testSecond % 5 == 0) testLat = (float) 19.236237;
    if (testSecond % 10 == 0) testLong = (float) 912.134341;
    if (testSecond % 15 == 0)  testAlt = (float) 23.3544;
    if(testSecond % 20 == 0) { // Reset
       testLat = 0.0;
       testLong = 0.0;
       testAlt = 0.0;
    }
    
    
      


    serial_dataset.start_char = '$';
    //serial_dataset.hour = hour;
    //serial_dataset.minute = minute;
    //serial_dataset.second = second;
    serial_dataset.hour = testHour;
    serial_dataset.minute = testMinute;
    serial_dataset.second = testSecond;
    serial_dataset.latitude = testLat;
    serial_dataset.longitude = testLong;
    serial_dataset.altitude = testAlt;

    /*serial_dataset.hour = 0;
    serial_dataset.minute = 0;
    serial_dataset.second = 0;
    serial_dataset.latitude = 0;
    serial_dataset.longitude = 0;
    serial_dataset.altitude = 0;*/


    //Probe_serial.write((byte*)&serial_dataset, sizeof(serial_dataset));     // Diese Funktion sendet den gesamten String byte fuer byte an den seriellen Port
    //Probe_serial.println("Hallo");

    LoRaPort.write((byte*)&serial_dataset, sizeof(serial_dataset));     // Diese Funktion sendet den gesamten String byte fuer byte an den seriellen Port
    //LoRaPort.write((byte*)&serial_dataset);     // Diese Funktion sendet den gesamten String byte fuer byte an den seriellen Port

    /*


    datensatz.openFile();
    datensatz.storedata(hour, minute, second, flat, flon, alt);
    datensatz.closeFile();
    */


    Serial.println();
    smartdelay(1000);
}













static void smartdelay(unsigned long ms)
{
    unsigned long start = millis();
    do {
        while (gpsPort.available())
            gps.encode(gpsPort.read());
    } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
    if (val == invalid) {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    }
    else {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
    smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
    char sz[32];
    if (val == invalid) {
        strcpy(sz, "*******");
    }
    else {
        sprintf(sz, "%ld", val);
    }
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i) {
        sz[i] = ' ';
    }
    if (len > 0) {
        sz[len - 1] = ' ';
    }
    Serial.print(sz);
    smartdelay(0);
}

static void print_date(TinyGPS& gps)
{
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned long age;
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    if (age == TinyGPS::GPS_INVALID_AGE) {
        Serial.print("********** ******** ");
    }
    else {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
            month, day, year, hour, minute, second);
        Serial.print(sz);
    }
    print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
    smartdelay(0);
}

static void print_str(const char* str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i) {
        Serial.print(i < slen ? str[i] : ' ');
    }
    smartdelay(0);
}
