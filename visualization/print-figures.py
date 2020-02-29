# %%
import pandas as pd
import os

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.ticker import StrMethodFormatter

mpl.rc('axes', labelsize=14)
mpl.rc('xtick', labelsize=12)
mpl.rc('ytick', labelsize=12)

# Where to save the figures
PROJECT_ROOT_DIR = ".."
IMAGES_PATH = os.path.join(PROJECT_ROOT_DIR, "assets")
os.makedirs(IMAGES_PATH, exist_ok=True)

def save_fig(fig_id, tight_layout=True, fig_extension="png", resolution=300):
    path = os.path.join(IMAGES_PATH, fig_id + "." + fig_extension)
    print("Saving figure", fig_id)
    if tight_layout:
        plt.tight_layout()
    plt.savefig(path, format=fig_extension, dpi=resolution)


DATA_FILE = "data.csv"
def load_data(file_name: str=DATA_FILE) -> pd.DataFrame:
    return pd.read_csv(file_name)

def clean_data(data: pd.DataFrame) -> pd.DataFrame:
    data["didwater"] = (data["didwater"] == True).astype(int)
    data["time"] = pd.to_datetime(data["time"])
    return data


def plot_humidity_time(data: pd.DataFrame):
    data = data.copy()
    plt.plot_date("time", "humiditypercent", data=data, fmt="-")
    data = data[data["didwater"] == 1]
    plt.plot_date("time", "humiditypercent", data=data, fmt="ro")

    plt.gca().yaxis.set_major_formatter(StrMethodFormatter('{x:,.0f}%'))
    plt.setp(plt.gca().xaxis.get_majorticklabels(), "rotation", 90)
    plt.legend(("humidity", "watered"), loc="upper center")
    plt.ylabel("Humidity", fontsize=14)
    plt.title("Humidity in time", fontsize=14)

    save_fig("humidity_time")
    plt.show()


def plot_did_water(data: pd.DataFrame):
    data = data.copy()
    sizes = data["didwater"].value_counts()
    plt.pie(sizes, explode=(0.05, 0), labels=["No", "Yes"], 
        autopct="%1.1f%%", shadow=True, startangle=30)

    plt.axis("equal")
    plt.title("Did the chip water the plant", fontsize=14)

    save_fig("did_water")
    plt.show()


def plot_when_water(data: pd.DataFrame):
    data = data.copy()
    data = data[data["didwater"] == 1]
    plt.hist(data["humiditypercent"])

    plt.ylabel("Counts", fontsize=14)
    plt.xlabel("Humidity", fontsize=14)
    plt.gca().xaxis.set_major_formatter(StrMethodFormatter('{x:,.0f}%'))
    plt.title("At which percent of humidity the chip waters the plant", fontsize=14)

    save_fig("when_water")
    plt.show()


def main():
    data = load_data()
    data = clean_data(data)

    plot_humidity_time(data)
    plot_did_water(data)
    plot_when_water(data)


if __name__ == "__main__":
    main()
# %%
