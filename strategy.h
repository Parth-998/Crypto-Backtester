#ifndef STRATEGY_H
#define STRATEGY_H

#include "data.h"

typedef enum { BUY, SELL } TradeType;

typedef struct {
    char date[16];
    TradeType type;
    double price;
    double coins;
} Trade;

void run_strategy(Candle *candles, int count, double cash, double threshold, const char *outfile);

#endif
