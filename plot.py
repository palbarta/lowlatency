import matplotlib.pyplot as plt

f = open('/home/brazil/Workspace/lowlatency/testmore_out.txt', 'r')
data = []
steps = []
memory_access_latencies = []
while True:
	if not f.readline():
		break
	step = int(f.readline().split()[2])
	f.readline()
	numOfL3Misses = int(f.readline().split()[5].replace(',', ''))
	timeDelta = float(f.readline().split()[10])
	entry = {'step' : step, 'numOfL3Misses' : numOfL3Misses, 'timeDelta': timeDelta}
	data.append(entry)
	
	if (numOfL3Misses < 800000):
		continue

	memory_access_latency = timeDelta * 1000000.0 / numOfL3Misses
	
	steps.append(step)
	memory_access_latencies.append(memory_access_latency)



#print data

plt.plot(steps, memory_access_latencies)
plt.ylabel('steps')
plt.ylabel('memory access latency (ns)')
plt.show()
