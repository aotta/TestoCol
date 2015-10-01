#include "num2words.h"
#include "string.h"
#include "pebble.h"

static const char* const UNITA[] = {
  "ZERO",
  "UNO",
  "DUE",
  "TRE",
  "QUATTRO",
  "CINQUE",
  "SEI",
  "SETTE",
  "OTTO",
  "NOVE"
};

static const char* const TEENS[] ={
  "",
  "UNDICI",
  "DODICI",
  "TREDICI",
  "QUATTORDICI",
  "QUINDICI",
  "SEDICI",
  "DICIASSETTE",
  "DICIOTTO",
  "DICIANNOVE"
};

static const char* const DECINE[] = {
  "",
  "DIECI",
  "VENTI",
  "TRENTA",
  "QUARANTA",
  "CINQUANTA",
  "SESSANTA",
  "SETTANTA",
  "OTTANTA",
  "NOVANTA"
};

static const char* const DECIN[] = {
  "",
  "DIECI",
  "VENT",
  "TRENT",
  "QUARANT",
  "CINQUANT",
  "SESSANT",
  "SETTANT",
  "OTTANT",
  "NOVANT"
};

static const char* STR_PAST = " E ";
static const char* STR_UNA = "UNA";
static const char* STR_MIDNIGHT="MEZZANOTTE";
static const char* STR_NOON="MEZZOGIORNO";


static size_t append_number(char* words, int num) {
  int decine_val = num / 10 % 10;
  int unita_val = num % 10;

  size_t len = 0;

  if (decine_val > 0) {
    if (decine_val == 1 && num != 10) {
      strcat(words, TEENS[unita_val]);
      return strlen(TEENS[unita_val]);
    }
    if (unita_val !=1 && unita_val !=8) {
			strcat(words, DECINE[decine_val]);
    	len += strlen(DECINE[decine_val]);    	
		} else {
			APP_LOG(APP_LOG_LEVEL_INFO, "minuti= %i",unita_val);
			strcat(words, DECIN[decine_val]);
    	len += strlen(DECIN[decine_val]);    	
		}
  }

  if (unita_val > 0 || num == 0) {
		strcat(words, UNITA[unita_val]);
    len += strlen(UNITA[unita_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void time_to_words(int hours, int minutes, char* words, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = minutes; //((minutes + 2) / 5) * 5;

  size_t remaining = length;
  memset(words, 0, length);
  
	//APP_LOG(APP_LOG_LEVEL_INFO, "hours= %i",fuzzy_hours);

	if (fuzzy_hours == 0) {
			remaining -= append_string(words, remaining, STR_MIDNIGHT);
			} else if (fuzzy_hours == 12) {
				remaining -= append_string(words, remaining, STR_NOON);
			  } else if (fuzzy_hours == 1 || fuzzy_hours ==13) {
					remaining -= append_string(words, remaining, STR_UNA);
					} else {
					remaining -= append_number(words, fuzzy_hours % 12);  
			  	}	 
	
	if (fuzzy_minutes != 0) {
				  	remaining -= append_string(words, remaining, STR_PAST); //e
				  	remaining -= append_number(words, fuzzy_minutes);
			 		} 
		}

