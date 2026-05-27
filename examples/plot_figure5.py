import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import os

files = [
    ("csv_data/figure5_SI_fine.csv", "SI fine"),
    ("csv_data/figure5_SI_coarse.csv", "SI coarse"),
    ("csv_data/figure5_RSI.csv", "RSI"),
    ("csv_data/figure5_RSI_tail.csv", "RSI tail average"),
]

z_slices = [0.25, 0.50, 0.75]

vmin = 0.0
vmax = 2.0

for file, title in files:
    df = pd.read_csv(file)

    for z0 in z_slices:
        df["zdist"] = (df["z"] - z0).abs()

        # 取最靠近该 z 截面的前 12% cell
        tol = df["zdist"].quantile(0.12)
        sub = df[df["zdist"] <= tol].copy()

        x = sub["x"].to_numpy()
        y = sub["y"].to_numpy()
        val = sub["phi0"].to_numpy()

        triang = tri.Triangulation(x, y)

        plt.figure(figsize=(6, 5))
        plt.tricontourf(
            triang,
            val,
            levels=80,
            vmin=vmin,
            vmax=vmax
        )
        plt.colorbar(label=r"$\phi_0$")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title(f"{title}, z≈{z0}")
        plt.axis("equal")
        plt.tight_layout()

        out = f"Figures/{os.path.basename(file).replace('.csv', '')}_z{z0:.2f}.png"
        plt.savefig(out, dpi=300)
        plt.close()