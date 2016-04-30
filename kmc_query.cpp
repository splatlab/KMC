/*
 * =====================================================================================
 *
 *       Filename:  kmer_query.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/27/2016 08:48:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prashant Pandey (), ppandey@cs.stonybrook.edu
 *   Organization:  Stony Brook University
 *
 * =====================================================================================
 */
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <set>
#include <bitset>
#include <cassert>
#include <fstream>

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>

#include	"kmc_api/kmc_file.h"

using namespace std;

/* Print elapsed time using the start and end timeval */
void print_time_elapsed(string desc, struct timeval* start, struct timeval* end) 
{
	struct timeval elapsed;
	if (start->tv_usec > end->tv_usec) {
		end->tv_usec += 1000000;
		end->tv_sec--;
	}
	elapsed.tv_usec = end->tv_usec - start->tv_usec;
	elapsed.tv_sec = end->tv_sec - start->tv_sec;
	float time_elapsed = (elapsed.tv_sec * 1000000 + elapsed.tv_usec)/1000000.f;
	cout << desc << "Total Time Elapsed: " << to_string(time_elapsed) << " seconds" << endl;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	int num_query = atoi(argv[2]);
	CKMCFile kmer_database_list;
	CKMCFile kmer_database_rand;
	vector<CKmerAPI*> kmer_objects;
	string filename;
	CKmerAPI kmer(28);
	uint32_t counter;
	struct timeval start, end;
	struct timezone tzp;

	filename = string(argv[1]);

	if (!kmer_database_list.OpenForListing(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	cout << "Reading from the fastq file and inserting in a vector list" << endl;
	while (kmer_database_list.ReadNextKmer(kmer, counter)) {
		kmer_objects.push_back(new CKmerAPI(kmer));
	}
	kmer_database_list.Close();

	if (!kmer_database_rand.OpenForRA(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	cout << "Querying kmers in the QF" << endl;
	gettimeofday(&start, &tzp);
	for (int i = 0; i < num_query; i++) {
		if (!kmer_database_rand.CheckKmer(*kmer_objects[rand()%kmer_objects.size()], counter)) {
			cout << "Can not find the kmer: " << kmer_objects[rand()%kmer_objects.size()]->to_string() << endl;
			abort();
		}
	}
	gettimeofday(&end, &tzp);
	print_time_elapsed("", &start, &end);
	kmer_database_rand.Close();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
