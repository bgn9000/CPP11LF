#include <string>
#include <iostream>
#include <future>
#include <memory>

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
    std::cout << "Thread" << threadNum << ":\t";
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

    std::vector<std::shared_ptr<std::promise<size_t>>> promises(nbThreads);
    std::vector<std::shared_ptr<std::future<size_t>>> futures(nbThreads);
    std::vector<std::shared_ptr<std::thread>> threads(nbThreads);
    for (int threadnum = 0; threadnum < nbThreads; ++threadnum)
    {
        promises[threadnum].reset(new std::promise<size_t>());
        futures[threadnum].reset(new std::future<size_t>());
        *futures[threadnum] = promises[threadnum]->get_future();
    }

    threads[0].reset(
        new std::thread(
            [&promises, &futures, nbThreads, &wholeStringToPrint, nbCharsToPrint]() 
            {
                size_t pos = 0;
                size_t newpos = printBlock(pos, 1, wholeStringToPrint, nbCharsToPrint);
                promises[0]->set_value(newpos);
                while(1)
                {
                    pos = futures[nbThreads-1]->get();
                    futures[nbThreads-1].reset(new std::future<size_t>());
                    promises[nbThreads-1].reset(new std::promise<size_t>());
                    *futures[nbThreads-1] = promises[nbThreads-1]->get_future();
                    newpos = printBlock(pos, 1, wholeStringToPrint, nbCharsToPrint);
                    promises[0]->set_value(newpos);
                }
            }
        )
    );

    for (int threadnum = 1; threadnum < nbThreads; ++threadnum)
    {
        threads[threadnum].reset(
            new std::thread(
	            [&promises, &futures, threadnum, &wholeStringToPrint, nbCharsToPrint]() 
		        {
                    size_t pos = 0;
                    size_t newpos = 0;
                    while(1)
                    {
                        pos = futures[threadnum-1]->get();
                        futures[threadnum-1].reset(new std::future<size_t>());
                        promises[threadnum-1].reset(new std::promise<size_t>());
                        *futures[threadnum-1] = promises[threadnum-1]->get_future();
                        newpos = printBlock(pos, threadnum+1, wholeStringToPrint, nbCharsToPrint);
                        promises[threadnum]->set_value(newpos);
                    }
		        }
            )
        );
    }
    
    for (int threadnum = 0; threadnum < nbThreads; ++threadnum)
    {
        threads[threadnum]->join();
    }
	return 0;
}
