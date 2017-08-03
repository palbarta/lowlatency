#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>

std::vector<int> a;
std::vector<std::pair<int, double>> results;
int finalNext = 0;

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
seqAccess(int n) {
	const int N = 1000*1000;
	const auto start = std::chrono::steady_clock::now();

	int next = 0;
	for (int i = 0; i < N; i++) {
		// Sequential memory access - prefetching
		//const int idx = i % n;
		// Random memory access - new cache line load at each access
		//const int idx = i;
		next = a[next];
	}
	const auto end = std::chrono::steady_clock::now();
	const auto diff = end - start;
	const std::pair<int, double> result(n, std::chrono::duration <double, std::milli> (diff).count());
	results.push_back(result);
	finalNext += next;
}

int 
main(int argc, char* argv[])
{
	// allocate 400 MB
	const int MB = 100;
	a.resize(1024*1024*MB);

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, a.size() - 1);

	for (int i = 0; i < a.size(); ++i) {
		a[i] = distribution(generator);
	}

	std::cout << "Sequential access" << std::endl;
	for (int n = 1024*1024*50; n > 2; n /= 2) {
		for (int i = 0; i < n; ++i) {
			a[i] = a[i] % n;
		}
		seqAccess(n);
	}

	for (const auto& result : results) {
		auto&& size = prettyPrintByte(4*result.first);
		std::cout << size << " byte chunk read and written 1M times in " 
	              << result.second << " ms" << std::endl;
	}

	std::cout << "Final next" << finalNext << std::endl;

	return 0;
}
