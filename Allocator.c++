// --------
// includes
// --------

#include <iostream> // istream, ostream
#include <sstream>  // getline
#include <string>   // string
#include <cassert>  // assert
#include <cstring>  // stoi
#include <vector>   // string

#include "Allocator.h"

using namespace std;

int num_cases;

// ------------
// voting_read
// ------------
vector<Case> voting_read (istream& r)
{
  int cur_candidate, num_candidates, cur_voter, voter_candidate, cur_election;
  string s, b;
  stringstream ballot_line;

  // Get the first line, the number of cases, and create an array of cases of that size
  getline( r, s);
  num_cases = stoi( s, nullptr);

  vector<Case> elections( num_cases );
  
  getline( r, s); // Skipping blank line

  // ----------------------------------------------------------------------------
  // For each case, set the names of candidates, create a vector for each ballot,
  // and add the ballot to the ballot vector.
  // ----------------------------------------------------------------------------
  for ( cur_election = 0; cur_election < num_cases; cur_election++ )
  {
    getline( r, s);
    num_candidates = stoi( s, nullptr);
    elections[ cur_election ].n = num_candidates;
    elections[ cur_election ].names.resize( num_candidates );

    for ( cur_candidate = 0; cur_candidate < num_candidates; cur_candidate++ )
    {
      getline( r, s);
      elections[ cur_election ].names[cur_candidate] = s;
    }

    cur_voter = 0;

    // -----------------------------------------------------------------------------
    // If there are still lines with content on it, then they must be valid ballots.
    // If there are no more lines or only a blank line, then it has either reached
    // the end of the ballots or the end of the file.
    // -----------------------------------------------------------------------------
    while ( getline( r, s) && !s.empty() )
    {
      ballot_line<<s;
      voter_candidate = 0;
      vector<int> r;

      // Split the line by spaces and add each element to the vector
      while( getline( ballot_line, b, ' '))
      {
        r.push_back(stoi( b, nullptr));
        voter_candidate++;
      }
      elections[ cur_election ].ballots.push_back(r);
      ballot_line.str("");
      ballot_line.clear();
      cur_voter++;
    }

    // Set the number of ballots
    elections[ cur_election ].b = cur_voter;
  }

  return elections;

}

// ------------
// voting_eval
// ------------
vector<int> voting_eval (Case& c)
{
  assert ( c.n > 0 && c.n < 21 );
  assert ( c.b > 0 && c.b < 1001 );

  vector<int> vote_count(c.n);
  vector<int> out_of_the_race(c.n);
  vector< vector<int> > voter_list[c.n];
  int i, max, min;

  while ( true )
  {
    // -----------------------------------------------------------------------------------------
    // For every ballot in the ballot array, find the first candidate that is still in the race,
    // and remove all of the other candidates. Then push the ballot onto the voter list of the
    // candidate that they voted for, and delete it from the ballot list.
    // -----------------------------------------------------------------------------------------
    while(c.ballots.size())
    {
      while ( out_of_the_race[ c.ballots.at(0).at(0) - 1 ] ) c.ballots.at(0).erase(c.ballots.at(0).begin());
      voter_list[ c.ballots.at(0).at(0) - 1 ].push_back(c.ballots.at(0) );
      c.ballots.erase( c.ballots.begin() );
    }

    // Find the maximum and minimum vote counts
    max = 0;
    min = 10000;
    for ( i = 0; i < c.n; ++i)
    {
      vote_count[ i ] = voter_list[ i ].size();
      if ( vote_count[ i ] > max ) max = vote_count[ i ];
      if ( !out_of_the_race[ i ] && vote_count[ i ] < min ) min = vote_count[ i ];
    }

    // ---------------------------------------------------------------------------------------
    // If a candidate has at least half of the maximum number of ballots, they are the winner.
    // Set all other candidates as out of the race, and return that vector.
    // ---------------------------------------------------------------------------------------
    if( max * 2 > c.b)
    {
      for ( i = 0; i < c.n; ++i )
      {
        if ( vote_count[ i ] < max ) out_of_the_race[ i ] = 1;
      }

      return out_of_the_race;
    }

    // ---------------------------------------------------------------------------------------
    // If the minimum vote count and the maximum cote count are the same, then every candidate
    // tied, so set all valid candidates to sill in the race and return that vector.
    // ---------------------------------------------------------------------------------------
    if( min == max )
    {
      for ( i = 0; i < c.n; ++i )
      {
        if ( vote_count[ i ] != max ) out_of_the_race[ i ] = 1;
      }
      return out_of_the_race;
    }

    // -----------------------------------------------------------------------------------------
    // If a candidate has the minimum number of votes, they need to be eliminated.
    // Note that they are out of the race, and then add their ballots back onto the ballot list.
    // -----------------------------------------------------------------------------------------
    for ( i = 0; i < c.n; ++i )
    {
      if ( vote_count[ i ] == min )
      {
        out_of_the_race[ i ] = 1;
        c.ballots.insert( c.ballots.end(), voter_list[ i ].begin(), voter_list[ i ].end() );
      }
    }

  }

}

// -------------
// voting_solve
// -------------
void voting_solve (istream& r, ostream& w)
{
  vector<Case> Cases= voting_read( r );

  vector<int> results;

  // ----------------------------------------------------------
  // For each candidate, if they are in the race, they have won
  // Print each of these candidates out.
  // ----------------------------------------------------------
  for ( int i = 0; i < num_cases; ++i)
  {
    results = voting_eval( Cases[i] );
    for ( int j = 0; j < Cases[i].n; ++j)
    {
      if( !results[j])
        w << Cases[i].names[j] << endl;
    }
    if(i < num_cases-1) w << endl;
  }

}
