#!/usr/bin/env python3
# compute_metrics_from_log.py
# Usage: python3 compute_metrics_from_log.py <per_cycle_csv>
import sys
import pandas as pd
import numpy as np

def cv(series):
    mean = series.mean()
    std = series.std(ddof=0)
    if mean == 0:
        return np.nan
    return std / mean

fn = sys.argv[1]
df = pd.read_csv("../outputs/per_cycle_log.csv")

# Drop non-numeric columns that might be present (like "POST" rows)
df = df[df['cycle'].apply(lambda x: str(x).isdigit())].copy()
df['cycle'] = df['cycle'].astype(int)

# Option: analyze only cycles after reset deasserted
if 'rst_n' in df.columns:
    df_post_reset = df[df['rst_n'] == 1]
else:
    df_post_reset = df

numeric_cols = df_post_reset.select_dtypes(include=[np.number]).columns.tolist()

metrics = []
for c in numeric_cols:
    s = df_post_reset[c].astype(float)
    metrics.append({
        'column': c,
        'mean': s.mean(),
        'std': s.std(ddof=0),
        'cv': cv(s),
        'non_zero_count': int((s != 0).sum())
    })

meta = {
    'total_cycles_sampled': int(len(df_post_reset)),
    'total_active_compute_cycles': int(df_post_reset['active_compute_cycles'].sum()) if 'active_compute_cycles' in df_post_reset.columns else None,
}

print("=== META ===")
for k,v in meta.items():
    print(f"{k}: {v}")
print("")
print("=== PER-COLUMN METRICS ===")
m_df = pd.DataFrame(metrics)
print(m_df.to_string(index=False))

# Throughput / requests if columns present
if 'requests_issued' in df_post_reset.columns and 'responses_seen' in df_post_reset.columns:
    total_req = int(df_post_reset['requests_issued'].sum())
    total_resp = int(df_post_reset['responses_seen'].sum())
    cycles = len(df_post_reset)
    print("")
    print("Throughput (requests/cycle):", total_req / cycles if cycles>0 else 0)
    print("Responses/cycle:", total_resp / cycles if cycles>0 else 0)
