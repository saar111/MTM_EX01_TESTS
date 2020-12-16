#include "date.h"
#include <stdlib.h>

#define DATE_1_FIRST -1
#define DATE_2_FIRST 1
#define DATES_ARE_EQUAL 0

#define NUM_1_IS_BIGGER -1
#define NUM_2_IS_BIGGER 1
#define EQUAL 0

#define LAST_DAY_OF_MONTH 30
#define LAST_MONTH_OF_YEAR 12

struct Date_t {
    int day;
    int month;
    int year;
};

// HELPER FUNCTIONS //

// Checking if the date is legal, returns true or false
static bool checkIfDateLegal(int day, int month) {
    bool day_legal = (day <= 30 && day >= 1);
    bool month_legal = (month <= 12 && month >= 1);

    if ((day_legal == true) && (month_legal == true)) {
        return true;
    } else {
        return false;
    }
}

// Compares two integers
// if the first is bigger returns -1, if the second is bigger returns 1, else 0
static int compareNumbers(int num1, int num2) {
    if (num1 > num2) {
        return NUM_1_IS_BIGGER;
    }
    if (num1 < num2) {
        return NUM_2_IS_BIGGER;
    }

    return EQUAL;

}

// date.h FUNCTIONS //

Date dateCreate(int day, int month, int year) {

    if (checkIfDateLegal(day, month) == false) {
        return NULL;
    }

    Date date = malloc(sizeof(*date));
    if (date == NULL) {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;

    return date;

}

void dateDestroy(Date date) {
    if (date == NULL) {
        return;
    }
    free(date);
}

Date dateCopy(Date date) {
    if (date == NULL) {
        return NULL;
    }

    Date new_date = dateCreate(date->day, date->month, date->year);
    return new_date;

}

bool dateGet(Date date, int *day, int *month, int *year) {
    if (date == NULL || day == NULL || month == NULL || year == NULL) {
        return false;
    }

    *day = date->day;
    *month = date->month;
    *year = date->year;

    if (day == NULL || month == NULL || year == NULL) {
        return false;
    }
    return true;
}


int dateCompare(Date date1, Date date2) {
    if (date1 == NULL || date2 == NULL) {
        return 0;
    }

    int day1 = date1->day;
    int month1 = date1->month;
    int year1 = date1->year;

    int day2 = date2->day;
    int month2 = date2->month;
    int year2 = date2->year;

    int day_result = compareNumbers(day1, day2);
    int month_result = compareNumbers(month1, month2);
    int year_result = compareNumbers(year1, year2);

    if (year_result == NUM_1_IS_BIGGER) {
        return DATE_2_FIRST;
    }
    if (year_result == NUM_2_IS_BIGGER) {
        return DATE_1_FIRST;
    }

    if (year_result == EQUAL) {

        if (month_result == NUM_1_IS_BIGGER) {
            return DATE_2_FIRST;
        }
        if (month_result == NUM_2_IS_BIGGER) {
            return DATE_1_FIRST;
        }
        if (day_result == NUM_1_IS_BIGGER) {
            return DATE_2_FIRST;
        }
        if (day_result == NUM_2_IS_BIGGER) {
            return DATE_1_FIRST;
        }
    }
    return DATES_ARE_EQUAL;

}

void dateTick(Date date) {
    if (date == NULL) {
        return;
    }

    int day = date->day;
    int month = date->month;
    int year = date->year;

    if (day == LAST_DAY_OF_MONTH) {
        if (month == LAST_MONTH_OF_YEAR) {
            date->day = 1;
            date->month = 1;
            date->year = year + 1;
        } else {
            date->day = 1;
            date->month = month + 1;
        }
    } else {
        date->day = day + 1;
    }
}
