#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CSVParser.h"

#define READ_BUF_SIZE 8192

typedef struct {
    int headerParsed;
    int colContinent;
    int colNewCases;
    int colNewDeaths;
    double totalCases;
    double totalDeaths;
    int rowCount;
} UserData;

static int is_empty_field(const char* value) {
    return value == NULL || value[0] == '\0';
}

static double parse_number(const char* value) {
    if (is_empty_field(value)) {
        return 0.0;
    }
    return atof(value);
}

static int find_column_index(char** cols, int ncols, const char* name) {
    for (int i = 0; i < ncols; i++) {
        if (strcmp(cols[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static void callback(char** cols, int ncols, void* userData) {
    UserData* data = (UserData*) userData;

    if (!data->headerParsed) {
        data->colContinent = find_column_index(cols, ncols, "continent");
        data->colNewCases = find_column_index(cols, ncols, "new_cases");
        data->colNewDeaths = find_column_index(cols, ncols, "new_deaths");
        data->headerParsed = 1;
        return;
    }

    data->rowCount++;

    if (data->colContinent < 0 || data->colNewCases < 0 || data->colNewDeaths < 0) {
        return;
    }

    if (data->colContinent >= ncols || data->colNewCases >= ncols || data->colNewDeaths >= ncols) {
        return;
    }

    if (strcmp(cols[data->colContinent], "South America") != 0) {
        return;
    }

    data->totalCases += parse_number(cols[data->colNewCases]);
    data->totalDeaths += parse_number(cols[data->colNewDeaths]);
}

int main(void) {
    FILE* f;
    CSVParser csv;
    char* buf;
    int qt;
    UserData data = {0, -1, -1, -1, 0.0, 0.0, 0};

    buf = (char*) malloc(READ_BUF_SIZE);
    if (buf == NULL) {
        return 1;
    }

    CSVParser_init(&csv);

    f = fopen("./owid-covid-data.csv", "rb");
    if (f == NULL) {
        free(buf);
        return 1;
    }

    qt = fread(buf, 1, READ_BUF_SIZE, f);
    while (qt > 0) {
        CSVParser_processLines(&csv, buf, qt, callback, &data);
        qt = fread(buf, 1, READ_BUF_SIZE, f);
    }

    fclose(f);
    CSVParser_processLines(&csv, "\n", 1, callback, &data);

    printf("America do Sul - Total de casos: %.0f, Total de mortes: %.0f\n", data.totalCases, data.totalDeaths);

    free(buf);
    return 0;
}
