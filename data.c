#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Candle *load_candles(const char *filename, int *count) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) { return NULL; }
    char header[256];
    fgets(header, sizeof(header), fp);
    int rows = 0;
    Candle tmp;
    while (!feof(fp)) {
        if (fscanf(fp, " %15[^,],%lf,%lf,%lf,%lf,%lf", tmp.date, &tmp.open, &tmp.high, &tmp.low, &tmp.close, &tmp.volume) == 6) {
            rows++;
        }
    }
    if (rows == 0) {
        fclose(fp);
        *count = 0;
        return NULL;
    }
    Candle *candles = malloc(rows * sizeof(Candle));
    if (candles == NULL) {
        fclose(fp);
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);
    fgets(header, sizeof(header), fp);
    int i = 0;
    while (i < rows && fscanf(fp, " %15[^,],%lf,%lf,%lf,%lf,%lf", candles[i].date, &candles[i].open, &candles[i].high, &candles[i].low, &candles[i].close, &candles[i].volume) == 6) {
        i++;
    }
    fclose(fp);
    *count = rows;
    return candles;
}
