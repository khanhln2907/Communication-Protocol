

#include "Datenlogger.h"


/*
In der init()-Funktion wird ein Check durchgefuehrt, ob die SD-Karte gelesen werden kann.
Aktuell gibt die Funktion nur eine Serielle-Ausgabe, ob der Schreib- und Lesevorgang erfolgreich war.
Evtl. ist es sinnvoll eine bool-Abfrage zu machen, um weitere Aktionen taetigen zu koennen.
Funktion zum Speichern der Daten im File "Data.txt" bei jedem Neustart des Systems wird die Datei ueberschrieben!!!
*/
void Datalog::init(int chipSelect)
{
    if (SD.begin(chipSelect))                                               // Check if the microSD card exists and is usable
    {
        Serial.println("microSD is ready");

        if (SD.exists("Data.txt"))                                          // Check, ob die Datei "Data.txt" vorhanden ist
        {
            Serial.println("Old Data.txt exists and will be removed.");
            Serial.println("Removing example.txt...");
            SD.remove("Data.txt");                                          // Falls vorhanden - DELETE, alte Daten werden ueberschrieben!
        }
        else
        {
            Serial.println("Data.txt doesn't exist and must be created.");  // Warnung, das die Datei noch erstellt werden muss
        }
        // return true;                                                     // if not - stop sketch

        this->dataFile = SD.open("Data.txt", FILE_WRITE);    // Das File "Data.txt" wird geoeffnet / erstellt und Daten werden in diesem gespeichert
        if (dataFile)                                       // If the file is ready, write to it:
        {
            S_Ueberschrift = String("Hour ") + String("Minute ") + String("Second ") + String("Latitude ") + String("Longitude ") + String("Altitude ");
            dataFile.println(S_Ueberschrift);
            Serial.println("Das File wurde mit der ersten Zeile erstellt");
            //dataFile.close();
        }
    }

    else
    {
        Serial.println("Card failed, or not present");
        // return false;                                                    // if not - stop sketch
    }
}

// Methode nur zum Oeffnen des Files und zum Schreiben in dieses
void Datalog::openFile()
{
    this->dataFile = SD.open("Data.txt", FILE_WRITE);    // Das File "Data.txt" wird geoeffnet / erstellt und Daten werden in diesem gespeichert
}


// Methode zum Speichern der Daten
void Datalog::storedata(byte hour, byte minute, byte second, float latitude, float longitude, float altitude)
//void Datalog::storedata(float hour, float minute, float second, float latitude, float longitude, float altitude)
{
    /*
    struct dataset lokalesDataset;
    lokalesDataset.hour = hour;
    lokalesDataset.minute = minute;
    lokalesDataset.second = second;
    lokalesDataset.latitude = latitude;
    lokalesDataset.longitude = longitude;
    lokalesDataset.altitude = altitude;
    */

    // File dataFile = SD.open("Data.txt", FILE_WRITE);    // Das File "Data.txt" wird geoeffnet / erstellt und Daten werden in diesem gespeichert
    if (this->dataFile)                                       // If the file is ready, write to it:
    {
        // String dataString = "";
        // dataFile.print(data, DEC);
        //String dataString = String(hour) + " " + String(minute) + " " + String(second) + " " + String(latitude) + " " + String(longitude) + " " + String(altitude);

        //dataString = String(data);
        //dataFile.println(String(data));

        //this->dataFile.write((const uint8_t*)&lokalesDataset, sizeof(lokalesDataset));
        //this->dataFile.write(lokalesDataset);
        //dataFile.println(dataString);
        char sz[8];
        sprintf(sz, "%02d:%02d:%02d ", hour, minute, second);
        dataFile.print("$");
        dataFile.print(sz);
        //dataFile.print(String(hour)+ ":" + String(minute)+ ":" + String(second));
        dataFile.print(",");
        dataFile.print(latitude, DEC);
        dataFile.print(",");
        dataFile.print(longitude, DEC);
        dataFile.print(",");
        dataFile.print(altitude);
        dataFile.println(" ");



        //Serial.println("Etwas wird geloggt");
    }

    else                                                // If the file is not ready, show error
    {
        Serial.println("Error opening Data.txt");
    }
    // Serial.println("fertig");
}

// Methode zum Schliessen des Files
void Datalog::closeFile()
{
    this->dataFile.close();
}