#include "data.h"
#include "strategy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(void) {
    printf("Usage: backtest <pricefile> [options]\n\n");
    printf("Run a buy-the-dip backtest on historical price data.\n\n");
    printf("Options:\n");
    printf("  -c, --cash <amount>     starting cash in dollars (default 1000)\n");
    printf("  -t, --threshold <pct>   %% drop that triggers a buy (default 3)\n");
    printf("  -o, --out <file>        also write the report to a file\n");
    printf("      --clear-reports     delete saved report files\n");
    printf("  -h, --help              show this help menu\n");
}

void clear_reports(void) {
    FILE *idx = fopen("reports_index.txt", "r");
    if (idx == NULL) {
        printf("No reports to clear.\n");
        return;
    }

    char name[256];
    int removed = 0;
    while (!feof(idx)) {
        if (fscanf(idx, "%s", name) == 1) {
            if (remove(name) == 0) {
                removed++;
            }
        }
    }
    fclose(idx);

    remove("reports_index.txt");
    printf("Removed %d report file(s).\n", removed);
}

int main(int argc, char *argv[]) {
    char *filename = NULL;
    double cash = 1000.0;
    double threshold = 3.0;
    char *outfile = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        }
        else if (strcmp(argv[i], "--clear-reports") == 0) {
            clear_reports();
            return 0;
        }
        else if (strcmp(argv[i], "--cash") == 0 || strcmp(argv[i], "-c") == 0) {
            i++;
            if (i >= argc) {
                printf("Missing value for --cash\n");
                return 1;
            }
            if (sscanf(argv[i], "%lf", &cash) != 1 || cash < 0) {
                printf("Invalid starting cash: %s\n", argv[i]);
                return 1;
            }
        }
        else if (strcmp(argv[i], "--threshold") == 0 || strcmp(argv[i], "-t") == 0) {
            i++;
            if (i >= argc) {
                printf("Missing value for --threshold\n");
                return 1;
            }
            if (sscanf(argv[i], "%lf", &threshold) != 1 || threshold < 0) {
                printf("Invalid threshold: %s\n", argv[i]);
                return 1;
            }
        }
        else if (strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "-o") == 0) {
            i++;
            if (i >= argc) {
                printf("Missing value for --out\n");
                return 1;
            }
            outfile = argv[i];
        }
        else if (argv[i][0] == '-') {
            printf("Unknown option: %s\n", argv[i]);
            return 1;
        }
        else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        printf("No price file given\n");
        return 1;
    }

    int count;
    Candle *candles = load_candles(filename, &count);
    if (candles == NULL) {
        printf("Could not open file: %s\n", filename);
        return 1;
    }

    run_strategy(candles, count, cash, threshold, outfile);

    free(candles);
    return 0;
}
