import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib as mpl

# Load CSV
fn = "PIEMP3/MP3/plot.csv"
df = pd.read_csv(fn, header=None, dtype=str)
# 5 columns: 4 digital channel for the sensors + Motor output
df = df.iloc[:, :5]
df.columns = ["L", "ML", "MR", "R", "MotorOutput"]

# Convert channels to ints (safe)
for c in ["L", "ML", "MR", "R"]:
    df[c] = pd.to_numeric(df[c].str.strip(), errors="coerce").fillna(0).astype(int)

# Event mapping to colors
events = df["MotorOutput"].astype(str).str.strip()
unique = sorted(events.unique())
cmap = plt.get_cmap("tab10")
event_colors = {ev: cmap(i % 10) for i, ev in enumerate(unique)}

# Create an RGB image where columns 0-3 are binary channels (black/white)
# and column 4 is the MotorOutput color.
n_rows = len(df)
# RGB image: rows x 5 columns x 3 channels
img = np.zeros((n_rows, 5, 3), dtype=float)

# fill channels: white for 1, black for 0
channels = df[["L", "ML", "MR", "R"]].values
img[:, :4, :] = np.where(channels[..., None] == 1, 1.0, 0.0)  # white or black

# fill motor output column with mapped RGB (drop alpha)
for i, ev in enumerate(events.values):
    rgba = mpl.colors.to_rgba(event_colors[ev])
    img[i, 4, :] = rgba[:3]

# Plot single combined image so MotorOutput is in same panel as channels
fig, ax_hm = plt.subplots(figsize=(10, 6), constrained_layout=True)

ax_hm.imshow(img, aspect="auto", origin="upper", interpolation="nearest")
ax_hm.set_ylabel("time (row index)")
ax_hm.set_xlabel("channels")
ax_hm.set_xticks(np.arange(5))
ax_hm.set_xticklabels(["L", "ML", "MR", "R", "MotorOutput"])

# legend for events (use patches with event_colors)
patches = [mpatches.Patch(color=event_colors[e], label=e) for e in unique]
ax_hm.legend(handles=patches, title="MotorOutput", bbox_to_anchor=(1.02, 1), loc="upper left")

out_png = "plot_visualize.png"
fig.suptitle("Binary channels with MotorOutput timeline")
plt.savefig(out_png, dpi=150, bbox_inches="tight")
print(f"Saved {out_png}")
plt.show()