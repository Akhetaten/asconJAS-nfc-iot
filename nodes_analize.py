import pandas as pd
# paths
INPUT_FILE = "nodes_parsed.csv"
SUMMARY_FILE = "nodes_metrics_summary.csv"
# Read the parsed log CSV
df = pd.read_csv(INPUT_FILE)
# timestamp is parsed as datetime
df['timestamp'] = pd.to_datetime(df['timestamp'], format="%H:%M:%S.%f")
# Compute intervals between consecutive events
df['time_diff'] = df['timestamp'].diff().dt.total_seconds()  # difference in seconds
# metrics  Count of events per component
component_counts = df['component'].value_counts().rename("event_count")
# Count of errors/warnings per component
error_counts = df[df['level'].str.upper() == "ERROR"]['component'].value_counts().rename("error_count")
warning_counts = df[df['level'].str.upper() == "WARNING"]['component'].value_counts().rename("warning_count")
# Timing statistics per component
timing_stats = df.groupby('component')['time_diff'].agg(['mean', 'min', 'max', 'std']).fillna(0)
# Combine metrics into a summary DataFrame
summary_df = pd.concat([component_counts, error_counts, warning_counts, timing_stats], axis=1).fillna(0)
# Save summary to CSV
summary_df.to_csv(SUMMARY_FILE)
print(f"Metrics summary saved to {SUMMARY_FILE}\n")
# print top 10 components by event count
print("Top 10 components by event count:")
print(summary_df.sort_values(by='event_count', ascending=False).head(10))
# Overall statistics
print("\nOverall statistics:")
print(f"Total events: {len(df)}")
print(f"Total unique components: {df['component'].nunique()}")
print(f"Total errors: {df[df['level'].str.upper() == 'ERROR'].shape[0]}")
print(f"Total warnings: {df[df['level'].str.upper() == 'WARNING'].shape[0]}")
