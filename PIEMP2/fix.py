import pandas as pd
import plotly.graph_objects as go
import numpy as np

df = pd.read_csv('data v2.csv', header =None)
df.iloc[0::2] = df.iloc[0::2].apply(lambda row: row[::-1], axis=1) # flips every other row it prints the opposite way
print("Min y:", df.min().min(), "Max y:", df.max().max())
df = df.clip(lower=0, upper=837)
df = 10 ** (((df) / -552) + (1240/552))



fig = go.Figure(data=go.Heatmap(z=df.values, colorscale='hot'))
fig.update_layout(title='Heatmap', autosize=True)
fig.show()
fig.write_html("heatmap.html")
print("Heatmap saved as heatmap.html. Open this file in your browser.")