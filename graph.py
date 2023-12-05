import re
import matplotlib.pyplot as plt

def parse_input(input_text):
    thread_times = re.findall(r'Time to solve all sudokus with (\d+) threads: (\d+\.\d+) s', input_text)
    thread_times = [(int(thread), float(time)) for thread, time in thread_times]
    return thread_times

def plot_graph(thread_times, input_name):
    threads, times = zip(*thread_times)
    plt.plot(threads, times, marker='o')
    plt.xlabel('Number of Threads')
    plt.ylabel('Time to Solve all Sudokus (s)')
    plt.title('Threads vs. Time to Solve Sudokus')
    plt.grid(True)
    plt.savefig(input_name+".png")
    plt.clf()

if __name__ == "__main__":
    input_name = "16_16"
    input_file_path = input_name+".txt"
    
    with open(input_file_path, "r") as file:
        input_text = file.read()

    thread_times = parse_input(input_text)
    plot_graph(thread_times, input_name)
