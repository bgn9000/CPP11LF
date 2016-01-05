#include <string>
#include <iostream>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>

#include <boost/lexical_cast.hpp>

//      Circular Printer 
//
//      A program which takes three inputs (command line)
//
//        1. a string
//        2. character count 'N' (integer)
//        3. thread count 'T' - number of threads (integer).
//
//      Each thread then should continually print blocks of 'N' character
//      substrings, wrapping around the string if necessary -- treating it
//      like ring buffer source.
//
//      The threads should produce strictly sequential output and also
//      run strictly in sequence. For the following input:
//
//        # myprinter ABCDEFGHIJ 3 4
//
//      we expect the following output:
//
//        Using 4 threads to print ABCDEFGHIJ alternatively in sets of 3 characters
//        Thread1: ABC
//        Thread2: DEF
//        Thread3: GHI
//        Thread4: JAB
//        Thread1: CDE
//        Thread2: FGH
//        Thread3: IJA
//        ......

int Notice(const char* appName)
{
    std::cout << 
         "Usage : " << appName << " [-h] [-S \"ABCDEFGHIJ\"] [-N 3]  [-T 4] "
         "\n\t -h          : Display the notice."
         "\n\t -S          : whole string to print. (default ABCDEFGHIJ)"
         "\n\t -N          : number of chars to print per block. (default 3)"
         "\n\t -T          : number of concurrent threads. (default 4)"
         << std::endl;
    return 0;
}

size_t printBlock(size_t pos, int threadNum, const std::string& wholeStringToPrint, int nbCharsToPrint)
{
    const auto len = wholeStringToPrint.size();
    std::cout << "Thread\t" << threadNum << ": ";
    while (nbCharsToPrint)
    {
        std::cout << wholeStringToPrint[pos];
        pos = (pos+1) % len;
        --nbCharsToPrint;
	}
    std::cout << std::endl;
    return pos;
}

int main(int argc, char* argv[])
{
    int nbCharsToPrint = 3;
	int nbThreads = 4;
    std::string wholeStringToPrint("ABCDEFGHIJ");

    // parse arguments
    int optchr;
    while ((optchr = getopt(argc, argv, "S:N:T:h")) != EOF)
    {
        switch (optchr)
        {
        case 'S':
            if (strlen(optarg) > 0)
	            wholeStringToPrint=std::string(optarg);
			else 
			{
            	std::cerr << "Bad param (-S) : expecting string" << std::endl;
                Notice(argv[0]);
	            return -1;			
            }
            break;
        case 'N':
            try
            {
	            nbCharsToPrint=boost::lexical_cast<int>(optarg);
                if (nbCharsToPrint <= 0)
                {
                    std::cerr << "Bad param (-N) : not valid block size=[" << optarg << "]" << std::endl;
                    Notice(argv[0]);
    	            return -1;
                }
            }
            catch(const boost::bad_lexical_cast &)
            {
            	std::cerr << "Bad param (-N) : expecting number, got [" << optarg << "]" << std::endl;
                Notice(argv[0]);

	            return -1;			
            }
            break;
        case 'T':
            try
            {
	            nbThreads=boost::lexical_cast<int>(optarg);
                if (nbThreads <= 0)
                {
                    std::cerr << "Bad param (-T) : not valid number of threads=[" << optarg << "]" << std::endl;
                    Notice(argv[0]);
    	            return -1;
                }
            }
            catch(const boost::bad_lexical_cast &)
            {
            	std::cerr << "Bad param (-T) : expecting number, got [" << optarg << "]" << std::endl;
                Notice(argv[0]);
	            return -1;			
            }
            break;
        case 'h':
        default:
            Notice(argv[0]);
            return -1;
        }
    }

    std::cout << 
		"Print [" << wholeStringToPrint <<
		"] into blocks of [" << nbCharsToPrint <<
		"] with [" << nbThreads << "] concurrent threads" << std::endl;


    if (nbThreads == 1)
    {
        std::cout << "Not implemented..." << std::endl;
        return 0;
    }

    std::vector<std::condition_variable> condition_vars(nbThreads);
    std::vector<std::mutex> mutexes(nbThreads);
    std::vector<std::shared_ptr<std::thread>> threads(nbThreads);

    size_t pos = 0;

    for (int threadnum = 1; threadnum < nbThreads-1; ++threadnum)
    {
        threads[threadnum].reset(
            new std::thread(
	            [&condition_vars, &mutexes, &pos, threadnum, &wholeStringToPrint, nbCharsToPrint]() 
		        {
                    while(1)
                    {
                        std::unique_lock<std::mutex> lock1(mutexes[threadnum]);
                        condition_vars[threadnum].wait(lock1);
                        pos = printBlock(pos, threadnum+1, wholeStringToPrint, nbCharsToPrint);
                        std::unique_lock<std::mutex> lock2(mutexes[threadnum+1]);
                        condition_vars[threadnum+1].notify_one();
                    }
		        }
            )
        );
    }

    threads[nbThreads-1].reset(
        new std::thread(
            [&condition_vars, &mutexes, &pos, nbThreads, &wholeStringToPrint, nbCharsToPrint]() 
	        {
                while(1)
                {
                    std::unique_lock<std::mutex> lock1(mutexes[nbThreads-1]);
                    condition_vars[nbThreads-1].wait(lock1);
                    pos = printBlock(pos, nbThreads, wholeStringToPrint, nbCharsToPrint);
                    std::unique_lock<std::mutex> lock2(mutexes[0]);
                    condition_vars[0].notify_one();
                }
	        }
        )
    );

    threads[0].reset(
        new std::thread(
            [&condition_vars, &mutexes, &pos, &wholeStringToPrint, nbCharsToPrint]() 
            {
                pos = printBlock(pos, 1, wholeStringToPrint, nbCharsToPrint);
                {
                    std::unique_lock<std::mutex> lock(mutexes[1]);
                    condition_vars[1].notify_one();
                }
                while(1)
                {
                    std::unique_lock<std::mutex> lock1(mutexes[0]);
                    condition_vars[0].wait(lock1);
                    pos = printBlock(pos, 1, wholeStringToPrint, nbCharsToPrint);
                    std::unique_lock<std::mutex> lock2(mutexes[1]);
                    condition_vars[1].notify_one();
                }
            }
        )
    );
    
    for (int threadnum = 0; threadnum < nbThreads; ++threadnum)
    {
        threads[threadnum]->join();
    }

	return 0;
}
