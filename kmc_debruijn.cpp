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

enum DNA_MAP {C, A, T, G};  // A=1, C=0, T=2, G=3

/**
 * Converts a string of "ATCG" to a uint64_t
 * where each character is represented by using only two bits
 */
uint64_t str_to_int(string str)
{
	uint64_t strint = 0;
	for (auto it = str.begin(); it != str.end(); it++) {
		uint8_t curr = 0;
		switch (*it) {
			case 'A': { curr = DNA_MAP::A; break; }
			case 'T': { curr = DNA_MAP::T; break; }
			case 'C': { curr = DNA_MAP::C; break; }
			case 'G': { curr = DNA_MAP::G; break; }
		}
		strint = strint | curr;
		strint = strint << 2;
	}
	return strint >> 2;
}

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
	CKMCFile kmer_database_list;
	CKMCFile kmer_database_rand;
	vector<CKmerAPI> kmers;
	vector<uint32_t> path_lengths;
	CKmerAPI kmer(28);
	uint32_t counter;
	struct timeval start, end;
	struct timezone tzp;

	if (!kmer_database_list.OpenForListing(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	cout << "Reading kmers from the database list" << endl;
	while (kmer_database_list.ReadNextKmer(kmer, counter)) {
		kmers.push_back(kmer);
	}
	kmer_database_list.Close();

	if (!kmer_database_rand.OpenForRA(filename)) {
		cout << "Can not open the database file" << endl;
		return EXIT_FAILURE;
	}

	cout << "Calc the longest path in the de bruijn graph" << endl;
	uint32_t max_size = 0;
	gettimeofday(&start, &tzp);
	for (CKmerAPI kmer : kmers) {
		if (!kmer_database_rand.CheckKmer(kmer, counter)) {
			cout << "Can not find the kmer: " << kmer.to_string() << endl;
			continue;
		}
		else {
			set<uint64_t> path;
			/* push the current node in the path list */
			string str = kmer.to_string();
			path.insert(str_to_int(str));
			while (true) {
				int next_kmerA = 0, next_kmerC = 0, next_kmerT = 0, next_kmerG = 0;
				CKmerAPI kmerA(28), kmerC(28), kmerT(28), kmerG(28);
				/* Construct next possible nodes from this node */
				str = str.substr(1, str.length());

				kmerA.from_string(str + 'A');
				CKmerAPI RkmerA(kmerA);
				RkmerA.reverse();
				if (RkmerA < kmerA)
					kmerA = RkmerA;
				
				kmerC.from_string(str + 'C');
				CKmerAPI RkmerC(kmerC);
				RkmerC.reverse();
				if (RkmerC < kmerC)
					kmerC = RkmerC;
				
				kmerT.from_string(str + 'T');
				CKmerAPI RkmerT(kmerT);
				RkmerT.reverse();
				if (RkmerT < kmerT)
					kmerT = RkmerT;
				
				kmerG.from_string(str + 'G');
				CKmerAPI RkmerG(kmerG);
				RkmerG.reverse();
				if (RkmerG < kmerG)
					kmerG = RkmerG;

				/* Check for the next edge from this node */
				/* If found then push node in the path list */
				if (kmer_database_rand.CheckKmer(kmerA, counter)) {
					str = kmerA.to_string();
					if (!path.insert(str_to_int(kmerA.to_string())).second) {
						/*Found a loop*/
						break;
					}
					next_kmerA = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerC, counter)) {
					str = kmerC.to_string();
					if (!path.insert(str_to_int(kmerC.to_string())).second) {
						/*Found a loop*/
						break;
					}
					next_kmerC = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerT, counter)) {
					str = kmerT.to_string();
					if (!path.insert(str_to_int(kmerT.to_string())).second) {
						/*Found a loop*/
						break;
					}
					next_kmerT = 1;
				}
				if (kmer_database_rand.CheckKmer(kmerG, counter)) {
					str = kmerG.to_string();
					if (!path.insert(str_to_int(kmerG.to_string())).second) {
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
			path_lengths.push_back(path.size());
			if (path.size() > max_size) {
				max_size = path.size();
			}
		}
	}
	gettimeofday(&end, &tzp);
	print_time_elapsed("", &start, &end);

	cout << "Max path size: " << max_size << endl;

	ofstream res;
	res.open("debruijn_paths");
	for (uint32_t len : path_lengths) {
		res <<  len << "\n" ;
	}
	res.close();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

