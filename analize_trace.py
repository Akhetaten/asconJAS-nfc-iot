#!/usr/bin/env python3

import struct
import csv
import gzip
import os
from tabulate import tabulate

# === CONFIGURATION ===
#TRACE_FILE = "nefertiti_executions.trace"  
TRACE_FILE = "akhenaton_executions.trace" 
#CSV_FILE = "nefertiti_trace_summary.csv"
CSV_FILE = "akhenaton_trace_summary.csv"
CSV_FIELDS = ["PC", "Opcode", "Count"]

# === FUNCTIONS ===

def parse_trace(trace_file):
    """
    Parse a Renode execution trace file (.trace)
    Returns total_cycles and a dict of opcode counts.
    """
    total_cycles = 0
    opcode_counts = {}

    # handle gzip or plain binary
    if trace_file.endswith(".gz"):
        with gzip.open(trace_file, "rb") as f:
            data = f.read()
    else:
        with open(trace_file, "rb") as f:
            data = f.read()

    record_size = 8  # 4 bytes PC + 4 bytes Opcode
    num_records = len(data) // record_size  # only full records

    for i in range(num_records):
        offset = i * record_size
        pc, opcode = struct.unpack("<II", data[offset:offset+record_size])
        total_cycles += 1
        opcode_counts[opcode] = opcode_counts.get(opcode, 0) + 1

    return total_cycles, opcode_counts

def write_csv(opcode_counts, csv_file):
    """
    Write opcode counts to a CSV file
    """
    with open(csv_file, "w", newline="") as csvf:
        writer = csv.writer(csvf)
        writer.writerow(CSV_FIELDS)
        for opcode, count in sorted(opcode_counts.items()):
            writer.writerow([hex(opcode), count, count])  # PC left blank or add if available

def print_summary(total_cycles, opcode_counts):
    """
    Print a nice table using tabulate
    """
    table = []
    for opcode, count in sorted(opcode_counts.items()):
        table.append([hex(opcode), count, round(count/total_cycles*100, 2)])
    headers = ["Opcode", "Count", "% of total cycles"]
    print("\n=== TRACE SUMMARY ===")
    print(f"Total cycles: {total_cycles}")
    print(tabulate(table, headers=headers, tablefmt="grid"))

# === MAIN ===
def main():
    print(f"Parsing trace file: {TRACE_FILE}")
    total_cycles, opcode_counts = parse_trace(TRACE_FILE)

    print_summary(total_cycles, opcode_counts)
    write_csv(opcode_counts, CSV_FILE)
    print(f"\nCSV saved to: {os.path.abspath(CSV_FILE)}")

if __name__ == "__main__":
    main()
