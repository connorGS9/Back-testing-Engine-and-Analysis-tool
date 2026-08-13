from pathlib import Path
import pandas as pd
import plotly.graph_objects as go

# folder containing this script
HERE = Path(__file__).resolve().parent
# output is one level up from analysis/, then into output/
OUT = HERE.parent / "output"

bh = pd.read_csv(OUT / "AAPL_buyhold.csv")
mac = pd.read_csv(OUT / "AAPL_crossover_50_200.csv")


fig = go.Figure()
fig.add_trace(go.Scatter(x=bh["Date"], y=bh["Equity"], name="Buy & Hold"))
fig.add_trace(go.Scatter(x=mac["Date"], y=mac["Equity"], name="MA Crossover 50/200"))
fig.update_layout(title="AAPL: Buy & Hold vs MA Crossover",
                  xaxis_title="Date", yaxis_title="Equity ($)")
fig.show()