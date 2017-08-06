#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdlib>

std::vector<unsigned int> a;
std::vector<std::pair<unsigned int, double>> results;
unsigned int finalNext = 0;

std::string prettyPrintByte(int n) {
	std::string metric = "byte";
	if (n > 1024) {
		n /= 1024;
		metric = "KB";
	}

	if (n > 1024) {
		n /= 1024;
		metric = "MB";
	}
	std::stringstream ss;
	ss << n << " " << metric;
	return ss.str();
}

void 
seqAccess(unsigned int step, unsigned int workSize) {
	if (workSize < a.size()) {
		for (unsigned int i = 0; i < workSize; ++i) {
			a[i] = (i + step);
		}
	}
	else {
		for (unsigned int i = 0; i < workSize; ++i) {
			a[i] = (i + step) % workSize;
		}	
	}

	const int N = 1000*1000;
	const auto start = std::chrono::steady_clock::now();

	unsigned int next = 0;
	for (int i = 0; i < N; ++i) {
		// Sequential memory access - prefetching
		//const int idx = i % n;
		// Random memory access - new cache line load at each access
		//const int idx = i;
		next = a[next];
	}
	const auto end = std::chrono::steady_clock::now();
	const auto diff = end - start;
	const std::pair<int, double> result(workSize, std::chrono::duration <double, std::milli> (diff).count());
	results.push_back(result);
	finalNext += next;
}

int 
main(int argc, char* argv[])
{
	const unsigned int step = (argc > 1) ? std::atoi(argv[1]) : 17;
	const unsigned int workSize = argc > 2 ? std::atoll(argv[2]) : -1;
	std::cout << "Usage: executable <step = " << step 
			  << "> <workSize = " << workSize << ">" << std::endl;

	// allocate 4 GB
	const unsigned int allocatedMemSize = 1024u *1024u * 1024u;
	a.resize(allocatedMemSize);

	std::cout << "Random access simulation" << std::endl;
	if (workSize < 0) {
		int workSize = 12;
		for (; workSize < allocatedMemSize; workSize *= 2) {
			seqAccess(step, workSize);
		}

		if (workSize != allocatedMemSize) {
			seqAccess(step, allocatedMemSize);
		}
	}
	else {
		seqAccess(step, workSize);
	}

	for (const auto& result : results) {
		auto&& size = prettyPrintByte(4*result.first);
		std::cout << size << " byte chunk read and written 1M times in " 
	              << result.second << " ms" << std::endl;
	}

	std::cout << "Final next " << finalNext << std::endl;

	return 0;
}
