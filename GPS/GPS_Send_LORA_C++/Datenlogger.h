/*
 Name:		Datalog.h
 Created:	11.02.2020 22:25:01
 Author:	metsk
 Editor:	http://www.visualmicro.com
*/

//#ifndef _Datenlogger_h
//#define _Datenlogger_h

#include <SD_t3.h>
#include <SD.h>


// Funktionierend jedoch nicht inline
class Datalog
{
private:
	int chipSelect;
	String S_Ueberschrift;
	File dataFile;			// Dies ist ein Objekt der Klasse File (der Bibliothek SD)

public:
	void init(int);			// In dieser Methode wird ein Check durchgefuehrt, ob die SD-Karte gelesen werden kann.
	void openFile();		// Diese Methode oeffnet das File
	void storedata(byte, byte, byte, float, float, float);
	//void storedata(float, float, float, float, float, float);
	void closeFile();		// Diese Methode schliesst das File

//	File dataFile;			// Dies ist ein Objekt der Klasse File (der Bibliothek SD)

	//void storedata(String);	// Diese Methode uebernimmt den Parameter im Format float und speichert diesen auf der SD-Karte

	struct dataset
	{
		float hour;
		float minute;
		float second;
		float latitude;
		float longitude;
		float altitude;
	};
};


