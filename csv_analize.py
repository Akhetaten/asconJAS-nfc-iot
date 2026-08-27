#!/usr/bin/env python3

import pandas as pd
from tabulate import tabulate
import matplotlib.pyplot as plt

#CSV_FILE = "akhenaton_trace_summary.csv"
CSV_FILE = "nefertiti_trace_summary.csv"

# === LOAD DATA ===
df = pd.read_csv(CSV_FILE)
df['PC'] = df['PC'].apply(lambda x: int(x, 16))  # convert hex PC to int

# === SUMMARY ===
total_instructions = df['Count'].sum()
unique_opcodes = df['Opcode'].nunique()
print("\n=== TRACE SUMMARY ===")
summary_table = [
    ["Total instructions", total_instructions],
    ["Unique opcodes", unique_opcodes],
    ["Total unique PCs", len(df)],
]
print(tabulate(summary_table, headers=["Metric", "Value"], tablefmt="grid"))

# === OPCODE FREQUENCY ===
opcode_freq = df.groupby('Opcode')['Count'].sum().reset_index().sort_values(by='Count', ascending=False)
print("\n=== OPCODE FREQUENCY ===")
print(tabulate(opcode_freq, headers=["Opcode", "Count"], tablefmt="grid"))

# === OPTIONAL: PLOT OPCODE DISTRIBUTION ===
plt.figure(figsize=(12,6))
plt.bar(opcode_freq['Opcode'].astype(str), opcode_freq['Count'])
plt.xlabel("Opcode")
plt.ylabel("Count")
plt.title("Opcode Execution Frequency")
plt.xticks(rotation=90)
plt.tight_layout()
plt.show()
