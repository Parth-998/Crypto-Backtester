#ifndef DATA_H
#define DATA_H

typedef struct {
    char date[16];
    double open;
    double high;
    double low;
    double close;
    double volume;
} Candle;

Candle *load_candles(const char *filename, int *count);

#endif
