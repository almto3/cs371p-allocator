#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <iostream> // istream, ostream
#include <string>   // string
#include <vector>   // vector

using namespace std;

// ------------
// struct Case
// ------------
/**
 * @param n the number of candidates
 * @param b the number of ballots
 * @param names the names of candidates
 * @param ballots the ballots of candidates
 */

struct Case {
	int n;
  int b;
	vector<string> names;
	vector< vector<int> > ballots;
};

// ------------
// voting_read
// ------------
/**
 * read a std input
 * @param r an istream
 * @returns a vector of struct Case
 */
vector<Case> voting_read (istream& r);		

// ------------
// voting_eval
// ------------
/**
 * @param c the instance of the struct Case
 * @returns the winner id in the array of candidates
 */
vector<int> voting_eval (Case& c);

/**
 * @param r an istream
 * @param w an ostream
 */
void voting_solve (istream& r, ostream& w);

#endif // Voting_h
