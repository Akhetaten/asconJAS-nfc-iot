# analyze_memdump.py
import sys

if len(sys.argv) < 2:
    print("Usage: python3 analyze_memdump.py <dump.bin>")
    sys.exit(1)

dump_file = sys.argv[1]

with open(dump_file, "rb") as f:
    data = f.read()

total_bytes = len(data)
used_bytes = sum(1 for b in data if b != 0x00)
usage_percent = (used_bytes / total_bytes) * 100

print(f"File: {dump_file}")
print(f"Total memory dumped: {total_bytes} bytes")
print(f"Non-zero (used) bytes: {used_bytes}")
print(f"RAM Usage: {usage_percent:.2f}%")
