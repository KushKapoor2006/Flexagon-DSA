import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

# --- Configuration ---
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_PATH = os.path.normpath(os.path.join(SCRIPT_DIR, "../simulator/outputs/"))
RESULTS_PATH = os.path.join(SCRIPT_DIR, "results/")

MODELS = {
    "vgg16": "VGG-16 (Orig)",
    "distilbert": "DistilBERT (Orig)",
    "vgg16_tuned": "VGG-16 (Tuned)",
    "distilbert_tuned": "DistilBERT (Tuned)"
}
MODES = {
    "base": "baseline",
    "flex": "pipelined_prefetch"
}

def ensure_results_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)
        print(f"Created directory: {path}")

def load_data(model_key, mode_key):
    filename = f"{model_key}_{MODES[mode_key]}_results.csv"
    filepath = os.path.normpath(os.path.join(DATA_PATH, filename))
    if not os.path.exists(filepath):
        print(f"WARNING: File not found: {filepath}")
        return None, None
    data = pd.read_csv(filepath)
    total_row = data[data['Layer'] == 'TOTAL'].iloc[0]
    layer_data = data[data['Layer'] != 'TOTAL'].copy()
    # Convert numeric fields to float where possible
    for col in ['WallClock', 'DRAMBusyPct', 'Compute', 'ConfigStall', 'DrainStall', 'DataStall']:
        if col in total_row.index:
            try:
                total_row[col] = float(total_row[col])
            except:
                total_row[col] = float(pd.to_numeric(total_row[col], errors='coerce').fillna(0))
    return total_row, layer_data

def annotate_bar(ax, rect, text, offset_frac=0.02, color='blue'):
    height = rect.get_height()
    x = rect.get_x() + rect.get_width() / 2.0
    ylim = ax.get_ylim()
    y_offset = (ylim[1] - ylim[0]) * offset_frac
    y = height + y_offset
    ax.text(x, y, text, ha='center', va='bottom', fontsize=10, fontweight='bold', color=color, clip_on=False)

def plot_1_baseline_breakdown(all_totals, save_path):
    print("Plot 1: Baseline Runtime Breakdown...")
    rows = []
    for full_key, total in all_totals.items():
        if full_key.endswith("_base"):
            model_key = full_key.rsplit("_", 1)[0]
            label = MODELS.get(model_key, model_key)
            rows.append({
                'Label': label,
                'Compute': float(total.get('Compute', 0)),
                'ConfigStall': float(total.get('ConfigStall', 0)),
                'DrainStall': float(total.get('DrainStall', 0)),
                'DataStall': float(total.get('DataStall', 0)),
                'WallClock': float(total.get('WallClock', 0))
            })
    if not rows:
        print("No baseline totals found.")
        return
    df = pd.DataFrame(rows).set_index('Label')
    stall_components = ['Compute', 'ConfigStall', 'DrainStall', 'DataStall']
    ax = df[stall_components].plot(kind='bar', stacked=True, figsize=(12, 7), rot=0)
    ax.set_title('Plot 1: Baseline Runtime Breakdown (Compute vs. Stall)', fontsize=16, pad=18)
    ax.set_ylabel('Total Cycles (Log Scale)', fontsize=12)
    ax.set_xlabel('Network', fontsize=12)
    ax.set_yscale('log')
    ax.legend(title='Runtime Component')
    ax.grid(axis='y', linestyle='--', alpha=0.7)
    for i, label in enumerate(df.index):
        total_cycles = df.loc[label, 'WallClock']
        total_stall = df.loc[label, ['ConfigStall', 'DrainStall', 'DataStall']].sum()
        stall_pct = (total_stall / total_cycles * 100) if total_cycles > 0 else 0
        ax.text(i, df.loc[label, stall_components].sum(), f' Stall: {stall_pct:.1f}% ', ha='center', va='bottom', fontweight='bold')
    plt.tight_layout()
    plt.savefig(os.path.join(save_path, "1_baseline_breakdown.png"), dpi=300, bbox_inches='tight')
    plt.close()

def plot_2_normalized_speedup(all_totals, save_path):
    print("Plot 2: Normalized Runtime (Speedup)...")
    groups = [
        (["vgg16", "distilbert"], "Original"),
        (["vgg16_tuned", "distilbert_tuned"], "Tuned")
    ]

    for keys, name in groups:
        labels = []
        norms = []
        speedups = []
        for k in keys:
            base = all_totals.get(f"{k}_base")
            flex = all_totals.get(f"{k}_flex")
            if base is None or flex is None:
                continue
            base_time = float(base.get('WallClock', 0))
            flex_time = float(flex.get('WallClock', 0))
            if base_time == 0 or flex_time == 0:
                continue
            normalized = flex_time / base_time
            speedup = base_time / flex_time
            labels.append(MODELS[k])
            norms.append(normalized)
            speedups.append(speedup)

        if len(labels) == 0:
            continue

        fig, ax = plt.subplots(figsize=(10,7))
        bars = ax.bar(np.arange(len(labels)), norms, color='navy', width=0.6)
        ax.set_xticks(np.arange(len(labels)))
        ax.set_xticklabels(labels, rotation=0)
        ax.set_ylim(0, max(1.1, max(norms) * 1.1))
        ax.axhline(y=1.0, color='black', linestyle='--', linewidth=1)
        ax.set_title(f'Plot 2: Normalized Runtime ({name})', fontsize=16, pad=18)
        ax.set_ylabel('Normalized Runtime (Lower is Better)', fontsize=12)
        ax.set_xlabel('Network', fontsize=12)
        ax.grid(axis='y', linestyle='--', alpha=0.7)
        for i, bar in enumerate(bars):
            text = f'{speedups[i]:.2f}x'
            annotate_bar(ax, bar, text, offset_frac=0.02)
        plt.tight_layout()
        fname = f"2_normalized_speedup_{name.replace(' ','_')}.png"
        plt.savefig(os.path.join(save_path, fname), dpi=300, bbox_inches='tight')
        plt.close()
        print(f"{name} speedups:")
        for i, lab in enumerate(labels):
            print(f"  {lab}: {speedups[i]:.3f}x")

def plot_3_dram_busy(all_totals, save_path):
    print("Plot 3: DRAM Busy Percentage...")
    rows = []
    for full_key, total in all_totals.items():
        model_key = full_key.rsplit("_", 1)[0]
        mode = full_key.rsplit("_", 1)[1]
        label = MODELS.get(model_key, model_key) + f" ({'BASE' if mode=='base' else 'FLEX'})"
        rows.append({
            'Label': label,
            'DRAMBusyPct': float(total.get('DRAMBusyPct', 0))
        })
    if not rows:
        return
    df = pd.DataFrame(rows).set_index('Label')
    ax = df.plot(kind='bar', figsize=(12,7), legend=False, colormap='coolwarm')
    ax.set_title('Plot 3: DRAM Busy Percentage (End-to-End)', fontsize=16, pad=18)
    ax.set_ylabel('DRAM Busy (%)', fontsize=12)
    ax.set_xlabel('Model (Mode)', fontsize=12)
    ax.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(os.path.join(save_path, "3_dram_busy_pct.png"), dpi=300, bbox_inches='tight')
    plt.close()

def plot_4_per_layer_stalls(all_data_layers, save_path):
    print("Plot 4: Per-Layer Stall Breakdown (Tuned)...")
    stall_cols = ['ConfigStall', 'DrainStall', 'DataStall']
    key_base = "vgg16_tuned_base"
    key_flex = "vgg16_tuned_flex"
    if key_base in all_data_layers and key_flex in all_data_layers:
        vgg_base = all_data_layers[key_base].set_index('Layer')
        vgg_flex = all_data_layers[key_flex].set_index('Layer')
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18,7), sharey=True)
        vgg_base[stall_cols].plot(kind='bar', stacked=True, ax=ax1, colormap='autumn', legend=False)
        ax1.set_title('VGG-16 (Tuned): Per-Layer Stalls (BASELINE)', fontsize=14)
        ax1.set_ylabel('Stall Cycles')
        ax1.set_xlabel('Layer')
        ax1.grid(axis='y', linestyle='--', alpha=0.7)
        vgg_flex[stall_cols].plot(kind='bar', stacked=True, ax=ax2, colormap='summer', legend=False)
        ax2.set_title('VGG-16 (Tuned): Per-Layer Stalls (FLEX-PIPE)', fontsize=14)
        ax2.set_xlabel('Layer')
        ax2.grid(axis='y', linestyle='--', alpha=0.7)
        plt.suptitle('Plot 4a: Per-Layer Stall Breakdown (VGG-16 Tuned)', fontsize=18)
        plt.tight_layout(rect=[0, 0.03, 1, 0.95])
        plt.savefig(os.path.join(save_path, "4a_vgg16_tuned_per_layer_stalls.png"), dpi=300, bbox_inches='tight')
        plt.close()

    key_base = "distilbert_tuned_base"
    key_flex = "distilbert_tuned_flex"
    if key_base in all_data_layers and key_flex in all_data_layers:
        db_base = all_data_layers[key_base].set_index('Layer')
        db_flex = all_data_layers[key_flex].set_index('Layer')
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18,7), sharey=True)
        db_base[stall_cols].plot(kind='bar', stacked=True, ax=ax1, colormap='autumn', legend=False)
        ax1.set_title('DistilBERT (Tuned): Per-Layer Stalls (BASELINE)', fontsize=14)
        ax1.set_ylabel('Stall Cycles')
        ax1.set_xlabel('Layer')
        ax1.grid(axis='y', linestyle='--', alpha=0.7)
        db_flex[stall_cols].plot(kind='bar', stacked=True, ax=ax2, colormap='summer', legend=False)
        ax2.set_title('DistilBERT (Tuned): Per-Layer Stalls (FLEX-PIPE)', fontsize=14)
        ax2.set_xlabel('Layer')
        ax2.grid(axis='y', linestyle='--', alpha=0.7)
        plt.suptitle('Plot 4b: Per-Layer Stall Breakdown (DistilBERT Tuned)', fontsize=18)
        plt.tight_layout(rect=[0, 0.03, 1, 0.95])
        plt.savefig(os.path.join(save_path, "4b_distilbert_tuned_per_layer_stalls.png"), dpi=300, bbox_inches='tight')
        plt.close()

def main():
    ensure_results_dir(RESULTS_PATH)

    try:
        import pandas
        import matplotlib
        import numpy
    except ImportError:
        print("Install: pip3 install pandas matplotlib numpy")
        sys.exit(1)

    print("Loading simulation data...")
    all_data_totals = {}
    all_data_layers = {}
    all_files_loaded = True

    for model_key in MODELS.keys():
        for mode_key in MODES.keys():
            full_key = f"{model_key}_{mode_key}"
            total_row, layer_df = load_data(model_key, mode_key)
            if total_row is None:
                all_files_loaded = False
            else:
                all_data_totals[full_key] = total_row.to_dict()
                all_data_layers[full_key] = layer_df

    if not all_files_loaded:
        print("\nData loading failed. Run simulator for all models (see simulator/Makefile run_all).")
        sys.exit(1)

    print("\nAll data loaded successfully. Generating plots...")
    plot_1_baseline_breakdown(all_data_totals, RESULTS_PATH)
    plot_2_normalized_speedup(all_data_totals, RESULTS_PATH)
    plot_3_dram_busy(all_data_totals, RESULTS_PATH)
    plot_4_per_layer_stalls(all_data_layers, RESULTS_PATH)

    print(f"\nAll plots saved to '{RESULTS_PATH}' directory.")

if __name__ == "__main__":
    main()
