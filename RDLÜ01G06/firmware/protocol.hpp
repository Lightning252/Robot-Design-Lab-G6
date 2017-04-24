/*
Empfängt die Daten von der seriellen Konsole und prüft, ob bestimmte Strings in den Daten vorhanden ist und gibt dann dementsprechend etwas auf der Konsole aus.
*/
void listenUSART();

/*
Prüft, ob in den übergebenen Buffer die Strings 'LED AN' oder 'LED AUS' vorhanden sind und gibt dies dan auf der Konsole aus.Der Parameter buffer enthält Daten, die vom Pc an die serielle Konsole geschrieben hat. buffer_length ist die geschriebene Länge.
@param 
*/
void parseBuffer(unsigned char buffer[], unsigned int buffer_length);
