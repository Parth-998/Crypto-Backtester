#include "strategy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_report(FILE *out, double starting_cash, double final_value,
                         double profit, Trade *trades, int trade_count) {
    fprintf(out, "Starting cash: $%.2f\n", starting_cash);
    fprintf(out, "Final value:   $%.2f\n", final_value);
    fprintf(out, "Profit/loss:   $%.2f\n", profit);
    fprintf(out, "Trades made:   %d\n\n", trade_count);
    for (int i = 0; i < trade_count; i++) {
        fprintf(out, "%s  %s  price=$%.2f  coins=%.4f\n",
                trades[i].date,
                trades[i].type == BUY ? "BUY " : "SELL",
                trades[i].price,
                trades[i].coins);
    }
}

void run_strategy(Candle *candles, int count, double cash,
                  double threshold, const char *outfile) {
    Trade *trades = malloc(count * sizeof(Trade));
    if (trades == NULL) {
        printf("Out of memory.\n");
        return;
    }

    double starting_cash = cash;
    double coins_held = 0.0;
    double buy_price = 0.0;
    int holding = 0;
    int trade_count = 0;

    for (int i = 1; i < count; i++) {
        double prev = candles[i - 1].close;
        double today = candles[i].close;
        double pct_change = (today - prev) / prev * 100.0;

        if (!holding && pct_change <= -threshold) {
            coins_held = cash / today;
            cash = 0.0;
            buy_price = today;
            holding = 1;

            strcpy(trades[trade_count].date, candles[i].date);
            trades[trade_count].type = BUY;
            trades[trade_count].price = today;
            trades[trade_count].coins = coins_held;
            trade_count++;
        }
        else if (holding && today > buy_price) {
            cash = coins_held * today;

            strcpy(trades[trade_count].date, candles[i].date);
            trades[trade_count].type = SELL;
            trades[trade_count].price = today;
            trades[trade_count].coins = coins_held;
            trade_count++;

            coins_held = 0.0;
            holding = 0;
        }
    }

    double final_value = cash + coins_held * candles[count - 1].close;
    double profit = final_value - starting_cash;

    write_report(stdout, starting_cash, final_value, profit, trades, trade_count);

    if (outfile != NULL) {
        FILE *fp = fopen(outfile, "w");
        if (fp != NULL) {
            write_report(fp, starting_cash, final_value, profit, trades, trade_count);
            fclose(fp);

            FILE *idx = fopen("reports_index.txt", "a");
            if (idx != NULL) {
                fprintf(idx, "%s\n", outfile);
                fclose(idx);
            }
        }
        else {
            printf("Warning: could not write to %s\n", outfile);
        }
    }

    free(trades);
}
