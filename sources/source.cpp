// Copyright 2020 <telsamar>
#include <thread>
#include <cstdlib>
#include <vector>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup.hpp>


#include "picosha2.h"

using std::thread;
using std::atoi;
using std::vector;
using std::cout;
using std::string;
using std::size_t;
using std::to_string;
using std::rand;
#define IDEAL ("0000");

static const int SIZE_ROTATION(5*1024*1024);
static const int ZERO(0);
void init_logs() {
    auto log = boost::log::add_file_log(
            boost::log::keywords::file_name = "../logs/logFile_%N.log" ,
            boost::log::keywords::rotation_size = SIZE_ROTATION,
            boost::log::keywords::time_based_rotation =
                    boost::log::sinks::file::rotation_at_time_point{ ZERO, ZERO, ZERO },
            boost::log::keywords::format = "[%TimeStamp%]: %Message%");

    auto log2 = boost::log::add_console_log(
            cout,
            boost::log::keywords::format = "[%TimeStamp%]: %Message%");

    log->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::trace);
    log2->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info);
}

void logs() {
    srand(*(new int));
    static unsigned int K(0);

    while (true) {
        int R = rand_r(&K)%255;
        string s = to_string(R);
        string Crypted = picosha2::hash256_hex_string(s);
        if (Crypted.substr(Crypted.size() - IDEAL.size()) != IDEAL)
            BOOST_LOG_TRIVIAL(trace) << "Non-positive result: " << Crypted << ", which result of " << s;
        else
            BOOST_LOG_TRIVIAL(info) << "Positive result: " << Crypted << ", which result of " << s;
    }
}
int main(int argc, char* argv[]) {
     size_t M = 0;
     std::cin >> M;
    if (argc >1)
        M = atoi(argv[1]);
    if (M == 0 || M >= 12)

     M = thread::hardware_concurrency();


	boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
	init_logs();
	boost::log::add_common_attributes();
	BOOST_LOG_TRIVIAL(trace) << "Threads amount: " << M;
	vector<thread*> threads;
	threads.resize(M);
	for (size_t i = 0; i < M; i++)
		threads[i] = new thread(logs);
	for (auto th : threads)
		th->join();
	return 0;
}

