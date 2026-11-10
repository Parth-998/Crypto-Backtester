# Crypto Backtester
 
A backtesting engine written in C. It loads OHLC (open, high, low, close) price data from a CSV file, runs a buy-the-dip strategy over it, and prints a trade log and performance report.
 
## Build
 
```sh
gcc -Wall -Wextra -o backtester main.c data.c strategy.c
```
 
## Usage
 
```sh
./backtester <pricefile> [options]
```
 
| Option | What it does | Default |
| --- | --- | --- |
| `-c`, `--cash <amount>` | Starting cash in dollars | 1000 |
| `-t`, `--threshold <pct>` | Percent drop that triggers a buy | 3 |
| `-o`, `--out <file>` | Also save the report to a file | — |
| `--clear-reports` | Delete every saved report and exit | — |
| `-h`, `--help` | Show usage | — |
 
Example:
 
```sh
./backtester prices.csv --cash 5000 --threshold 2.5 --out run1.txt
```
 
A sample dataset, `prices.csv`, is included.
 
## How it's organized
 
- `data.c` / `data.h` — reads and validates the CSV into an array of candles
- `strategy.c` / `strategy.h` — runs the buy-the-dip simulation and writes the trade log and report
- `main.c` — parses the command line and ties it together
Memory for the candle data is allocated dynamically, and bad input (a missing file, an invalid number, an unknown flag) exits with an error message.
 
Saved report paths are tracked in `reports_index.txt`, which is how `--clear-reports` knows what to delete.
 
