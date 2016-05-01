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

#include "kmer_counter/libs/zlib.h"
#include "kmer_counter/libs/bzlib.h"

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
	string filename = string(argv[1]);
	int num_kmers = atoi(argv[2]);
	CKMCFile kmer_database_list;
	CKMCFile kmer_database_rand;
	vector<CKmerAPI> kmers;
	CKmerAPI kmer(28);
	uint32_t counter;
	struct timeval start, end;
	struct timezone tzp;

	if (!kmer_database_list.OpenForListing(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	cout << "Reading kmers from the database list" << endl;
	int i = 0;
	while (kmer_database_list.ReadNextKmer(kmer, counter)) {
		i++;
		if (i == num_kmers)
			break;
		kmers.push_back(kmer);
	}
	cout << "Total kmers: " << i << endl;
	kmer_database_list.Close();

	if (!kmer_database_rand.OpenForRA(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	cout << "Calc the longest path in the de bruijn graph" << endl;
	uint32_t max_size = 0;
	for (CKmerAPI kmer : kmers) {
		if (!kmer_database_rand.CheckKmer(kmer, counter)) {
			cout << "Can not find the kmer: " << kmer.to_string() << endl;
			continue;
		}
		else {
			set<string> path;
			/* push the current node in the path list */
			string str = kmer.to_string();
			path.insert(str);
			while (true) {
				int next_kmerA = 0, next_kmerC = 0, next_kmerT = 0, next_kmerG = 0;
				CKmerAPI kmerA(28), kmerC(28), kmerT(28), kmerG(28);
				/* Construct next possible nodes from this node */
				str = str.substr(1, str.length());

				kmerA.from_string(str + 'A');
				kmerC.from_string(str + 'C');
				kmerT.from_string(str + 'T');
				kmerG.from_string(str + 'G');

				/* Check for the next edge from this node */
				/* If found then push node in the path list */
				if (kmer_database_rand.CheckKmer(kmerA, counter)) {
					str = kmerA.to_string();
					if (!path.insert(kmerA.to_string()).second) {
						/*Found a loop*/
						break;
					}
					next_kmerA = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerC, counter)) {
					str = kmerC.to_string();
					if (!path.insert(kmerC.to_string()).second) {
						/*Found a loop*/
						break;
					}
					next_kmerC = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerT, counter)) {
					str = kmerT.to_string();
					if (!path.insert(kmerT.to_string()).second) {
						/*Found a loop*/
						break;
					}
					next_kmerT = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerG, counter)) {
					str = kmerG.to_string();
					if (!path.insert(kmerG.to_string()).second) {
						/*Found a loop*/
						break;
					}
					next_kmerG = 1;
				}

				/* Check if there is only one edge from this node */
				if (next_kmerA + next_kmerC + next_kmerT + next_kmerG == 1) {
					continue;
				}
				else {	/* Abort this path if there are multiple edges */
					break;
				}
			}
			if (path.size() > max_size) {
				cout << "Path size: " << path.size() << endl;
				max_size = path.size();
			}
		}
	}

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

