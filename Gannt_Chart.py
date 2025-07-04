import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# Step 1: Manually feed your process execution intervals from C++ output
process_intervals = {
# feed your ouput here .
}

# Step 2: Plotting the Gantt chart
fig, gnt = plt.subplots(figsize=(10, 5))
gnt.set_title('Gantt Chart for CPU Scheduling RR')
gnt.set_xlabel('Time')
gnt.set_ylabel('Process')

# Set Y-axis ticks and labels
process_ids = list(process_intervals.keys())
gnt.set_yticks([10 * i for i in range(1, len(process_ids) + 1)])
gnt.set_yticklabels([f'P{pid}' for pid in process_ids])
gnt.grid(True)

# Assign different colors
colors = ['skyblue', 'salmon', 'palegreen', 'plum', 'khaki', 'orange']

# Step 3: Draw bars for each process execution interval
for i, pid in enumerate(process_ids):
    intervals = process_intervals[pid]
    for interval in intervals:
        start, end = interval
        gnt.broken_barh([(start, end - start)], (10 * (i + 1) - 2, 4),
                        facecolors=colors[i % len(colors)])
        # Label inside the bar
        gnt.text((start + end) / 2, 10 * (i + 1), f'P{pid}', 
                 ha='center', va='center', fontsize=9, color='black')

plt.tight_layout()
plt.show()
