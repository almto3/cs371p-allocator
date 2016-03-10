


/*
// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

// --------
// includes
// --------

#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

using namespace std;

// -----------
// TestVoting
// -----------

// -----------
// voting_read
// -----------
TEST(VotingFixture, read_1)
{
    istringstream s("1\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n");
    vector<Case> p = voting_read(s);
    ASSERT_EQ( p[0].n, 2);
    ASSERT_EQ( p[0].b, 3);
    ASSERT_EQ( p[0].names[0], "John Doe");
    ASSERT_EQ( p[0].ballots[1][0], 1);
}

TEST(VotingFixture, read_2)
{
    istringstream s("2\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n");
    vector<Case> p = voting_read(s);
    ASSERT_EQ( p[1].n, 3); 
    ASSERT_EQ( p[0].b, 3); 
    ASSERT_EQ( p[1].names[2], "Sirhan Sirhan"); 
    ASSERT_EQ( p[1].ballots[3][2], 3);
}

TEST(VotingFixture, read_3)
{
    istringstream s("1\n\n1\nJohn Doe\n1\n1\n1\n");
    vector<Case> p = voting_read(s);
    ASSERT_EQ( p[0].n, 1); 
    ASSERT_EQ( p[0].b, 3); 
    ASSERT_EQ( p[0].names[0], "John Doe"); 
    ASSERT_EQ( p[0].ballots[0][0], 1);
}

// -----------
// voting_eval
// -----------
TEST(VotingFixture, eval_1) 
{
    istringstream s("1\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n");
    vector<Case> p = voting_read(s);
    vector<int> results = voting_eval( p[0] );
    ASSERT_EQ( results[0], 0 );
    ASSERT_EQ( results[1], 1 );
}

TEST(VotingFixture, eval_2) 
{
    istringstream s("2\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n");
    vector<Case> p = voting_read(s);
    vector<int> results = voting_eval( p[1] );
    ASSERT_EQ( results[0], 0 );
    ASSERT_EQ( results[1], 1 );
    ASSERT_EQ( results[2], 1 );
}

TEST(VotingFixture, eval_3) 
{
    istringstream s("1\n\n1\nJohn Doe\n1\n1\n1\n");
    vector<Case> p = voting_read(s);
    vector<int> results = voting_eval( p[0] );
    ASSERT_EQ( results[0], 0 );
}

// ------------
// voting_solve
// ------------
TEST(VotingFixture, solve_1) 
{
    istringstream r("1\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n");
    ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("John Doe\n", w.str());
}

TEST(VotingFixture, solve_2) 
{
    istringstream r("2\n\n2\nJohn Doe\nJane Doe\n1 2\n1 2\n2 1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n");
    ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("John Doe\n\nJohn Doe\n", w.str());
}

TEST(VotingFixture, solve_3) 
{
    istringstream r("1\n\n1\nJohn Doe\n1\n1\n1\n");
    ostringstream w;
    voting_solve(r, w);
    ASSERT_EQ("John Doe\n", w.str());
}

/*
% g++ -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall Collatz.c++ TestCollatz.c++ -o TestCollatz -lgtest -lgtest_main -lpthread



% valgrind TestCollatz                                         >  TestCollatz.out 2>&1
% gcov -b Collatz.c++     | grep -A 5 "File 'Collatz.c++'"     >> TestCollatz.out
% gcov -b TestCollatz.c++ | grep -A 5 "File 'TestCollatz.c++'" >> TestCollatz.out



% cat TestCollatz.out
==14225== Memcheck, a memory error detector
==14225== Copyright (C) 2002-2011, and GNU GPL'd, by Julian Seward et al.
==14225== Using Valgrind-3.7.0 and LibVEX; rerun with -h for copyright info
==14225== Command: TestCollatz
==14225==
Running main() from gtest_main.cc
[==========] Running 7 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 7 tests from Collatz
[ RUN      ] Collatz.read
[       OK ] Collatz.read (31 ms)
[ RUN      ] Collatz.eval_1
[       OK ] Collatz.eval_1 (5 ms)
[ RUN      ] Collatz.eval_2
[       OK ] Collatz.eval_2 (3 ms)
[ RUN      ] Collatz.eval_3
[       OK ] Collatz.eval_3 (3 ms)
[ RUN      ] Collatz.eval_4
[       OK ] Collatz.eval_4 (3 ms)
[ RUN      ] Collatz.print
[       OK ] Collatz.print (17 ms)
[ RUN      ] Collatz.solve
[       OK ] Collatz.solve (10 ms)
[----------] 7 tests from Collatz (88 ms total)

[----------] Global test environment tear-down
[==========] 7 tests from 1 test case ran. (132 ms total)
[  PASSED  ] 7 tests.
==14225==
==14225== HEAP SUMMARY:
==14225==     in use at exit: 0 bytes in 0 blocks
==14225==   total heap usage: 495 allocs, 495 frees, 70,302 bytes allocated
==14225==
==14225== All heap blocks were freed -- no leaks are possible
==14225==
==14225== For counts of detected and suppressed errors, rerun with: -v
==14225== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 2 from 2)
File 'Collatz.c++'
Lines executed:100.00% of 17
Branches executed:100.00% of 18
Taken at least once:61.11% of 18
Calls executed:89.47% of 19
Creating 'Collatz.c++.gcov'
File 'TestCollatz.c++'
Lines executed:100.00% of 26
Branches executed:57.14% of 224
Taken at least once:28.57% of 224
Calls executed:54.07% of 209
Creating 'TestCollatz.c++.gcov'
*/