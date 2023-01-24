import subprocess
import matplotlib.pyplot as plt
import os
import time

# list of c values to test
c_values = [i for i in range(1, 23, 1)]

url = ["http://localhost:8080/arithmetic/prime?num=523", "http://localhost:8080/arithmetic/fibonacci?num=20", "http://localhost:8080/helloworld?str=Raja"]

# file to output results
output_file = ["loadtest1", "loadtest2", "loadtest3"]

# load testing graph names
throughput_graph = ["loadtest1_throughput", "loadtest2_throughput", "loadtest3_throughput"]
response_time_graph = ["loadtest1_response_time", "loadtest2_response_time", "loadtest3_response_time"]

# threadpool sizes
threadpool_size = [1, 2, 3]

print("Starting tests")

# three test
for i in range(3):
    # dictionary to store results
    throughput_results = [{}, {}, {}]
    response_time_results = [{}, {}, {}]
    print(f"Starting test {i + 1} for {url[i]}")

    for j in range(3):
        file_path = output_file[i] + "t" + str(j) + ".data"
        if os.path.exists(file_path):
            os.remove(file_path)
        # start server
        cmd = "./cflask 8080 " + str(threadpool_size[j])
        # start the server process
        server_process = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        time.sleep(3)
        # run Apache Benchmark for each c value
        for c in c_values:
            # run ab command
            # server_process = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            ab_output = subprocess.run(["ab", "-c", str(c), "-n", "100000", url[i]], capture_output=True)

            throughput = None
            response_time = None

            with open(output_file[i] + "t" + str(j) + ".data", "a") as f:
                f.write(f'--------------------c = {c}--------------------\n')
                f.write(ab_output.stdout.decode())
                f.write('\n')
            # parse output for throughput and response time
            for line in ab_output.stdout.decode().split("\n"):
                if "Requests per second:" in line:
                    throughput = float(line.split()[3])
                    throughput_results[j][c] = throughput
                elif "Time per request:" and "(mean)" in line:
                    response_time = float(line.split()[3])
                    response_time_results[j][c] = response_time

        # send the "enter" command to the server
        server_process.stdin.write(b"\n")
        server_process.stdin.flush()

        # retrieve the output from the server
        output, error = server_process.communicate()
             
    print(f"Test {i + 1} completed")
    print(f"Now generating graphs for test {i + 1} for {url[i]}")
    # create graph of throughput results
    label = ["threadpool size = " + str(threadpool_size[0]), "threadpool size = " + str(threadpool_size[1]), "threadpool size = " + str(threadpool_size[2])]
    fig = plt.figure()
    plt.plot(list(throughput_results[0].keys()), list(throughput_results[0].values()), label=label[0], alpha=0.5)
    plt.plot(list(throughput_results[1].keys()), list(throughput_results[1].values()), label=label[1], alpha=0.6)
    plt.plot(list(throughput_results[2].keys()), list(throughput_results[2].values()), label=label[2], alpha=0.7)
    plt.xlabel("Concurrent Requests")
    plt.ylabel("Throughput (requests/second)")
    plt.title("Throughput vs Concurrent Requests(n=100000)\n" + " - " + url[i], fontsize=10)
    plt.legend()
    plt.savefig(throughput_graph[i] + ".jpg")
    plt.close(fig)

    # create graph of response time results
    fig = plt.figure()
    for k in range(3):
        plt.plot(list(response_time_results[k].keys()), list(response_time_results[k].values()), label=label[k], alpha=0.5 + 0.1 * k)
    plt.xlabel("Concurrent Requests")
    plt.ylabel("Avg Response Time (ms)")
    plt.title("Avg Response Time vs Concurrent Requests(n=100000)\n" + " - " + url[i], fontsize=10)
    plt.legend(loc='upper right', fontsize='small')
    plt.savefig(response_time_graph[i] + ".jpg")
    plt.close(fig)

    print(f"Graphs generated for test {i + 1} for {url[i]}")

print("All tests completed")
