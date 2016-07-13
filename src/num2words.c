#include "num2words.h"
#include "string.h"
//#include "stdio.h"

static const char* const UNITES[] = {
  "zero",
  "une",
  "deux",
  "trois",
  "quatre",
  "cinq",
  "six",
  "sept",
  "huit",
  "neuf"
};

static const char* const DIZAINE[] ={
  "",
  "onze",
  "douze",
  "treize",
  "quatorze",
  "quinze",
  "seize",
  "dix-sept",
  "dix-huit",
  "dix-neuf"
};

static const char* const DIZAINES[] = {
  "",
  "dix",
  "vingt",
  "trente",
  "quarante",
  "cinquante",
  "soixante",
  "soixante-dix",
  "quatre-vingt",
  "quatre-vingt-dix"
};
static const char* const JOUR[]= {
  "Dimanche",
  "Lundi",
  "Mardi",
  "Mercredi",
  "Jeudi",
  "Vendredi",
  "Samedi",  
};
static const char* const MOIS[]= {
  "janvier",
  "février",
  "mars",
  "avril",
  "mai",
  "juin",
  "juillet",
  "août",
  "septembre",
  "octobre",
  "novembre",
  "décembre",
};
static const char* STR_ESPACE = " ";
static const char* STR_HEURE = "heure";
static const char* STR_HEURES = "heures";
static const char* STR_MIDI = "midi";
static const char* STR_MINUIT = "minuit";
static const char* STR_QUART = "quart";
static const char* STR_MOINS = "moins";
static const char* STR_LE = "le";
static const char* STR_DEMIE = "demie";
static const char* STR_ET = "et";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;
  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, DIZAINE[ones_val]);
      return strlen(DIZAINE[ones_val]);
    }
    strcat(words, DIZAINES[tens_val]);
    len += strlen(DIZAINES[tens_val]);
    if (ones_val > 0) {
      strcat(words, STR_ESPACE);
      len += 1;
    }
  }
  if (ones_val > 0 || num == 0) {
    strcat(words, UNITES[ones_val]);
    len += strlen(UNITES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) 
{
  size_t written = strlen(str);
  if (written > length ){
    written = length;
  }
  strncat(buffer, str, written);
  return written;
}

//static size_t append_digit(char* buffer, const size_t length, int num) 
//{
//  if (num == 1) 
//    snprintf(buffer, length, "1er");
//  else   
//    snprintf(buffer, length, "%d", num);
//  return written;
//}
static size_t append_hour( char * words, const size_t length, int fuzzy_hours)
{
  size_t remaining=length;
  if (fuzzy_hours == 0 || fuzzy_hours == 24) {
    remaining -= append_string(words, remaining, STR_MINUIT);
  } else if (fuzzy_hours == 12) {
    remaining -= append_string(words, remaining, STR_MIDI);
  } else {
    fuzzy_hours = fuzzy_hours % 12;
    remaining -= append_number(words, fuzzy_hours);
    remaining -= append_string(words, remaining,STR_ESPACE);
    if (fuzzy_hours==1 ){
      remaining -= append_string(words, remaining,STR_HEURE);
    } else {
      remaining -= append_string(words, remaining,STR_HEURES);
    }     
  }
  return length - remaining;
}

void fuzzy_time_to_words(int hours, int minutes, char* words,  const size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }
  size_t remaining = length;
  memset(words, 0, length);

  if (fuzzy_minutes ==0) {
    remaining -= append_hour(words, remaining, fuzzy_hours);
  }
  else {
    if (fuzzy_minutes == 15) {
      remaining -= append_hour(words, remaining, fuzzy_hours);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_ET);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_QUART);            
    } else if (fuzzy_minutes == 45) {
      fuzzy_hours = (fuzzy_hours + 1) % 24;
      remaining -= append_hour(words, remaining, fuzzy_hours);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_MOINS);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_LE);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_QUART);      
    } else if (fuzzy_minutes == 30) {
      remaining -= append_hour(words, remaining, fuzzy_hours);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_ET);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_DEMIE);      
    } else if (fuzzy_minutes < 30) {
      remaining -= append_hour(words, remaining, fuzzy_hours);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_number(words, fuzzy_minutes);      
    } else {
      fuzzy_hours = (fuzzy_hours + 1) % 24;
      remaining -= append_hour(words, remaining, fuzzy_hours);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_string(words, remaining, STR_MOINS);
      remaining -= append_string(words, remaining, STR_ESPACE);
      remaining -= append_number(words, (60 - fuzzy_minutes));    
    }
  } 
}


void fuzzy_date_to_words(int dow, int day, int month, int year, char* words,  const size_t length) 
{
  size_t remaining=length;
  memset(words, '\0',length);
  remaining -= append_string(words, remaining, JOUR[dow]);
  remaining -= append_string(words, remaining, STR_ESPACE);
  if (day ==1)
    remaining -= append_string(words, remaining, "premier");
  else   
    remaining -= append_number(words, day);
  remaining -= append_string(words, remaining, STR_ESPACE);  
  remaining -= append_string(words, remaining, MOIS[month]);  
  //remaining -= append_digits(words, remaining, day);  
  //remaining -= append_string(words, remaining, STR_ESPACE);
  //remaining -= append_string(words, remaining, MOIS[month]);  
}
  
