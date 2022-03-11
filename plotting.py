import json
import matplotlib.pyplot as plt

def main():

    """
    functie die de tijd plot over de mogelijke threads die dan getoond word als een plot.
    """

    time = []
    threads = []

    with open('data/data.json') as f:
        data = json.loads(f.read())

    for key, value in data["merge_sort"].items():
        threads.append(key)
        time.append(value)

    fig, ax = plt.subplots()

    ax.bar(threads, time, width=0.2, edgecolor="white", linewidth=1)

    plt.xlabel('Threads')
    plt.ylabel('Tijd (microseconds)')
    plt.show()

main()
