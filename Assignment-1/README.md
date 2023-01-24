#### CS 695 - Assignment 1

###### How to start the server using submitted tar file?

- Extract the tar file (`190050096-cs695-a1.tar.gz`)

- Copy all files in the `190050096-cs695-a1` directory to `libhttp-1.8/examples/cflask`

- Change the current working directory to `libhttp-1.8/examples/cflask` and run the below command

  ```bash
  make clean # not required
  make # I have submitted Makefile along with other files
  
  ./cflask <port_num> <num_threads> # ./cflask 8090 6 (for eg)
  # something like below will be printed on screen
  Hey there! I'm a cflask server.
  Press enter to exit.
  
  # Now, you can test it using browser or postman
  # http://localhost:8090/
  # http://localhost:8090/square?num=5
  # http://localhost:8090/arithmetic/fibonacci?num=10
  ```

###### How to generate load test data and graph from the submitted file?

```shell
# I have submitted a python file (script.py)
# All the *.data and *.jpg can be easily generated using this file
# matplotlib required for running below script

python3 script.py # make sure you have done make clean and make before you run this and you are in correct directory

# This will generate 6 plots and 9 data files
# First load test performed on url1 = http://localhost:8080/arithmetic/prime?num=523
# loadtest1t0.data --- all the data generated keeping threadpool size = 1
# loadtest1t1.data --- all the data generated keeping threadpool size = 2
# loadtest1t2.data --- all the data generated keeping threadpool size = 3
# ---------------------------------------------------------------------------

# First load test performed on url2 = http://localhost:8080/arithmetic/fibonacci?num=20
# loadtest2t0.data --- all the data generated keeping threadpool size = 1
# loadtest2t1.data --- all the data generated keeping threadpool size = 2
# loadtest2t2.data --- all the data generated keeping threadpool size = 3
# ---------------------------------------------------------------------------

# First load test performed on url3 = http://localhost:8080/helloworld?str=Raja
# loadtest3t0.data --- all the data generated keeping threadpool size = 1
# loadtest3t1.data --- all the data generated keeping threadpool size = 2
# loadtest3t2.data --- all the data generated keeping threadpool size = 3
# ---------------------------------------------------------------------------

# Note: loadtest{*}_response_time.jpg -- Avg. response time vs number of concurrent requests
# loadtest{*}_throughput.jpg -- Throughput vs number of concurrent requests
```



###### Plot Explanation or Load Testing

**Aim/Motivation**

- Load testing is used to determine how well a system will perform under a heavy user load. This information is used to identify bottlenecks and other performance issues that may need to be addressed before the system is deployed in production.

**Setup and Execution Details, Metrics**

- I have write a script using python subprocess to setup the experiment. It will start the server, collect the data generated from the apache benchmark and plot the graphs.
- `./cflask 8080 <threadpool_size>` was used in the script to start the server.
- `ab -n 100000 -c <num_client> <url>` was used to run the benchmark. 
  - **-n** is the number of requests to perform for the benchmarking session 
  - **-c concurrency**
    Number of multiple requests to perform at a time. Default is one request at a time.
- Two metrics from the stats are most important.
  - **Requests per second:**   <> [#/sec] (mean). --- used in plotting throughput graph
    **Time per request:**       <> [ms] (mean) -- used in plotting avg. response time graph
- Execution details are given in the above [How to generate load test data and graph from the submitted file?](#how-to-generate-load-test-data-and-graph-from-the-submitted-file) section

**Hypothesis/Expectation**

- For each throughput vs concurrent requests graph: throughput increases and then flattens at a certain load value
- For avg. response time: Lower avg. response time for large threadpool size and avg. response time for more concurrent requests should be more.

**Observations from the Data/Plots and explanation of behavior & inferences**

- I am running this experiment on my local machine(**Both server and apache benchmark are sharing resources**). -- explanation of behaviors in next section. Note: libhttp uses threading and Apache Bench only uses one OS thread irrespective of the concurrency level.
  Note: Apache benchmark concurrency(c) number of requests open at all times
- url1 - http://localhost:8080/arithmetic/prime?num=523 load testing 1
  - **Both throughput and avg. response time have some peaks and valleys.** Why? Both server and apache benchmark are sharing limited resources. 
  - **Throughput graph**: As we can see in the plot that throughput first increases and then flattens (there is some fluctuation although). As expected, the throughput for threadpoolsize=1 is lower than both threadpoolsize=2 and 3, because with a threadpoolsize of 1, only one task can be executed at a time, while with a threadpoolsize of 2 or 3, multiple tasks can be executed simultaneously, increasing the throughput.
    - There is some flipping seen between threadpoolsize 2 and 3, with a higher threadpoolsize, there may be increased overhead and context switching, which can also impact performance. It ultimately depends on the specific workload and the resources available. It also worth noting that **ab** has limitation
  - **Average response time**: As seen from graph, as expected increase in concurrency leads to increases in avg. response time. threadpoolsize =1 has more avg. response time than other twos as expected.
    - Again there is some competition happening between 2 and 3, possible reason is because of limitation of **ab**, same machine and context switching.
- url2 - http://localhost:8080/arithmetic/fibonacci?num=20 load testing 2
  - **Both throughput and avg. response time have some peaks and valleys.** Why? Both server and apache benchmark are sharing limited resources. 
  - **Throughput graph**: Same as url 1
  - **Average response time**: same as url 1
- url3 - http://localhost:8080/helloworld?str=Raja load testing 3
  - **Both throughput and avg. response time have some peaks and valleys.** Why? Both server and apache benchmark are sharing limited resources. 
  - **Throughput graph**: same as url 1
  - **Average response time**: same as url 1

**Note**: **Comparison between url 1 , url 2 and url 3 (different function)**: 

- Avg. response time of url2(i.e fibonacci) goes higher(upto 7ms) than url 1(i.e prime), because fibonacci require more computation. For helloworld, it is lower than fibonacci and prime overall with some deviation.
- For Throughput, there is no clear difference visible in the plots of all three url because of ab limitation, all these functions require lesser computation time and server-client both running on same machine(Sharing resources). 

**Note**: I performed load testing on my virtualbox Linux machine which have limited resources. Since, server and **ab** both running on same machine, ab results are also not much accurate and libhttp has also some limitation. So, you can see some deviation in plots in compare with ideal graph.

**Note2:** I also run server on remote cse machine and apache benchmark on my machine but graph formed was zigzag only (graph not attached).