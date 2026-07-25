import yfinance as yf

symbol = "AAPL"
df = yf.download(symbol, start="2015-01-01", end="2025-01-01")
df.columns = df.columns.droplevel(1)          # drop the "Ticker" level -> single clean header
df = df[["Open", "High", "Low", "Close", "Volume"]]   # force conventional OHLCV order
df.to_csv(f"data/{symbol}.csv")