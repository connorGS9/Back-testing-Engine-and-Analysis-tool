import yfinance as yf

symbol = "BTC-USD"
df = yf.download(symbol, start="2015-01-01", end="2025-01-01")

if df is None or df.empty:
    raise ValueError("No data returned from yfinance. Check your ticker or dates.")

df.columns = df.columns.droplevel(1)          # drop the "Ticker" level -> single clean header
df = df[["Open", "High", "Low", "Close", "Volume"]]   # force conventional OHLCV order
df.to_csv(f"data/{symbol}.csv")