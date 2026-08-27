#!/usr/bin/env python3
import re, csv, struct, numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# === Configuration ===
CLOCK_FREQ = 168_000_000  # STM32F4 MCU clock Hz
POWER_MW = 50             # Estimated MCU power draw (mW) — adjustable

BASE = Path("/home/akhenaton/iot-nfc-sim/loopascon")
TRACE_FILES = {
    "akhenaton": BASE / "akhenaton_executions.trace",
    "nefertiti": BASE / "nefertiti_executions.trace"
}
MEM_DUMPS = {
    "akhenaton": BASE / "akhenaton_mem.bin",
    "nefertiti": BASE / "nefertiti_mem.bin"
}
LOG_FILE = BASE / "nodes.log"
OUT_DIR = BASE / "results"
OUT_DIR.mkdir(exist_ok=True)

def parse_cycles(trace_path):
    cycles = 0
    with open(trace_path) as f:
        for line in f:
            if "PC=" in line:
                cycles += 1  # each opcode → 1 cycle approx OR replace with real MCU model later
    return cycles

def calc_timing_energy(cycles):
    time_s = cycles / CLOCK_FREQ
    time_ns = time_s * 1e9
    power_w = POWER_MW / 1000
    energy_j = power_w * time_s
    return time_ns, energy_j * 1e6  # return µJ

def parse_latency_throughput():
    timestamps = []
    with open(LOG_FILE) as f:
        for line in f:
            match = re.search(r"timestamp: (\d+)", line)
            if match:
                timestamps.append(int(match.group(1)))
    if len(timestamps) < 2:
        return None, None
    delays = np.diff(timestamps)
    avg_latency = np.mean(delays)
    throughput = 1e9 / avg_latency  # msgs per second
    return avg_latency, throughput

def parse_ram_usage(mem_file):
    data = Path(mem_file).read_bytes()
    used = sum(b != 0 for b in data)
    return used

def main():
    results = []
    for node, trace in TRACE_FILES.items():
        cycles = parse_cycles(trace)
        time_ns, energy_uj = calc_timing_energy(cycles)
        ram_bytes = parse_ram_usage(MEM_DUMPS[node])

        results.append({
            "Node": node,
            "Cycles": cycles,
            "Time_ns": time_ns,
            "Energy_µJ": energy_uj,
            "RAM_Used_Bytes": ram_bytes
        })

    latency, throughput = parse_latency_throughput()
    for r in results:
        r["Latency_ns"] = latency
        r["Throughput_msgs_per_s"] = throughput

    csv_path = OUT_DIR / "metrics.csv"
    with open(csv_path, "w") as f:
        w = csv.DictWriter(f, results[0].keys())
        w.writeheader()
        w.writerows(results)

    # Plot
    nodes = [r["Node"] for r in results]
    times = [r["Time_ns"] for r in results]
    energies = [r["Energy_µJ"] for r in results]

    plt.figure()
    plt.bar(nodes, times)
    plt.ylabel("Time (ns)")
    plt.title("Encryption/Decryption Time")
    plt.savefig(OUT_DIR / "time.png")

    plt.figure()
    plt.bar(nodes, energies)
    plt.ylabel("Energy (µJ)")
    plt.title("Energy per Message")
    plt.savefig(OUT_DIR / "energy.png")

    print(f"Results saved in {OUT_DIR}")

if __name__ == "__main__":
    main()

