#pragma once

#include "string.h"

void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length);
void fuzzy_date_to_words(int dow, int day, int month, int year, char* words,  const size_t length);
