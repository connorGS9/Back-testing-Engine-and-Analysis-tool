# Backtesting Engine, Planned Improvements & Design Roadmap

A running list of deliberate future work. Each item records *what*, *why deferred*,
and *what would trigger building it*.

---

## Runtime / execution

### Batch & config-driven runner  (deferred)
- **What:** Drive the engine from a config file (or a batch list) that specifies
  many ticker/strategy/parameter combinations to run in one invocation, each
  writing its own named output CSV.
- **Why deferred:** The command-line-argument runner (current stage) already
  unblocks interactive exploration ("BAH on AAPL, then crossover on TSLA") without
  a recompile. Config-driven batching is strictly more machinery for a need we
  don't have yet.
- **Trigger to build:** When manually typing runs becomes the bottleneck. That is,
  when generating the many output files the analysis dashboard wants, or when
  running a parameter sweep (see below), makes a batch loop clearly worth it.

### Strategy factory  (deferred)
- **What:** Extract the "string name + params to unique_ptr<Strategy>" construction
  out of main into a dedicated `makeStrategy(...)` factory function.
- **Why deferred:** With only two strategies, an inline if/else in main is clearer
  than an abstraction. The factory earns its place when the roster grows.
- **Trigger to build:** Around 5+ strategies, when the if/else in main gets unwieldy.

---

## Data / assets

### In-engine date-range filtering  (deferred)
- **What:** Let the engine backtest a sub-range (e.g. only 2020 to 2022) of a loaded
  series, rather than always the whole file.
- **Why deferred:** Date ranges can be selected at *ingestion* today (yfinance
  start/end), needing zero engine changes. In-engine filtering requires dates to
  be *comparable*, and they are currently stored as strings.
- **Trigger to build:** When you want to test the *same* downloaded data across
  many periods without re-downloading. That is when dates graduate from strings to
  a sortable/comparable representation.

### Multi-symbol / time-aligned feed  (deferred, interfaces already multi-ready)
- **What:** Let a single strategy see multiple assets at once, walking by *date*
  rather than by index, coping with mismatched trading calendars (US vs KRX
  holidays, crypto weekends, differing IPO dates).
- **Why deferred:** The genuinely hard part (time alignment) is not needed for
  single-asset backtests, which cover the whole current strategy roadmap. The
  Strategy/Portfolio interfaces were already shaped to accept it without a rewrite.
- **Trigger to build:** First strategy that needs cross-asset signals (pairs
  trading, portfolio allocation). This is also when dates must become real dates.

### Currency normalization  (deferred)
- **What:** Handle cross-currency comparison (e.g. Samsung in KRW vs a USD asset).
- **Why deferred:** Single-asset returns are currency-agnostic (a % gain is the
  same in any unit), so it does not bite until comparing across currencies.
- **Trigger to build:** Multi-asset, multi-currency portfolios.

### Per-asset calendar constant  (small, soon)
- **What:** The 252 trading-days-per-year constant is equity-specific; crypto is
  about 365. Make it a per-asset parameter.
- **Why deferred:** Only matters once non-equity assets are tested.
- **Trigger to build:** First crypto/FX backtest.

---

## Realism (the "trustworthy numbers" pillar)

### Transaction costs & slippage  (deferred, important)
- **What:** Model a cost per trade and price slippage when rebalancing.
- **Why deferred:** Current sim rebalances to target daily with zero friction, a
  *known simplification*, fine for relative comparison, not for realism.
- **Trigger to build:** Before claiming any strategy's absolute performance is
  realistic; especially important for high-turnover strategies where costs dominate.
- **Note:** This is a genuine correctness/realism upgrade a senior would probe.

### Time-varying risk-free rate  (deferred)
- **What:** Use the actual risk-free rate per date rather than a fixed 4% constant.
- **Why deferred:** A fixed constant is standard for a project like this; the
  simplification is documented and understood.
- **Trigger to build:** If Sharpe/Sortino precision across different eras matters.

---

## Performance (the "defend it with numbers" pillar)

### Parameter sweep + parallelization  (planned milestone)
- **What:** Run a strategy across a grid of parameters, parallelized across cores,
  benchmarked naive-vs-optimized with real numbers.
- **Why deferred:** It is a dedicated later milestone; needs the strategies and
  metrics to exist first (they now do).
- **Key design notes for when built:**
  - Load & parse data ONCE, run all combos against the in-memory series (never
    re-parse per run, the single biggest bottleneck).
  - Price data is read-only across threads, so it is shareable with no locks; give
    each thread its own portfolio/results buffer, merge at the end.
  - Persist *summary metrics per combo* (one tidy row each), NOT full equity
    curves for every combo. Regenerate full curves on demand for interesting ones.
  - Consider incremental (rolling) indicator updates: O(1) per bar vs O(window).
  - Consider virtual to template (static dispatch) as the last-mile optimization,
    with a before/after benchmark.

### CSV to Parquet for storage  (deferred)
- **What:** Columnar, typed, compressed file format; loads far faster than CSV.
- **Why deferred:** CSV is human-readable while debugging; Parquet is a v2 speed/
  scale upgrade.
- **Trigger to build:** When data volume or load time becomes a real cost.

---

## Interface / API polish

### Strategy self-naming  (small, in progress)
- **What:** `virtual std::string name() const` on Strategy so each reports its own
  label (including params for crossover, e.g. "crossover_50_200") for
  self-describing output filenames.
- **Status:** Being added now.

### Position sizing beyond 0/1  (natural extension)
- **What:** The normalized target already supports fractional (0 to 1), leverage
  (>1), and short (<0). Volatility-targeting will be the first strategy to use
  fractional sizing rather than binary in/out.
- **Trigger:** Volatility-targeting strategy.

---

## Explicitly OUT of scope (deliberate boundaries)

- **Options backtesting:** a fundamentally different engine (options chains,
  Greeks, expiration/assignment, paid data). Documented as out-of-scope, not an
  oversight. Would be a separate project built on top of this one.
- **Intraday / tick-level data:** this is a daily-bar engine by design.
- **Live / real-money trading, brokerage execution:** research tool, not an
  execution system.
