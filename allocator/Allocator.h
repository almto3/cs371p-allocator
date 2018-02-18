// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <stdexcept> // invalid_argument
#include <iostream>

using namespace std;


// ---------
// Allocator
// ---------

template <typename T, std::size_t N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;        // this is correct
        }                                              

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);
        }

    private:
        // ----
        // data
        // ----
        int s1 = 0, s2 = 0;

        char a[N];

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * Basically, this version checks that the sentinels give correct values, doesn't check for the validity of the data, also checks that the first and last sentinels match each other
         */
        bool valid () const {

            int i = 0;  //i is position/value of the sentinel
            int sen1 = 0, sen2 = 0;

            //while loop will read the sentinels until it reaches the end of the heap
            while (i < N){
                
                sen1 = (*this)[i];
                i = i + 4 + abs(sen1);

                sen2 = (*this)[i];
                i += 4;

                if (sen1 != sen2){      //sentinels don't match 
                    return false;
                }
            }
            
            //i has reached the end without any problems 
            if (i == N){
                return true;
            }
            //there's a problem with the heap
            else{
                return false;
            }
        }

        /**
         * O(1) in space
         * O(1) in time
         * Friend Tests for the unit tests
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
        FRIEND_TEST(TestAllocator2, index);
         
        FRIEND_TEST(TestAllocator2, Allocator1);
        FRIEND_TEST(TestAllocator2, Allocator2);
        FRIEND_TEST(TestAllocator2, Allocator3);

        FRIEND_TEST(TestAllocator2, deallocate1);
        FRIEND_TEST(TestAllocator2, deallocate2);
        FRIEND_TEST(TestAllocator2, deallocate3); 

        FRIEND_TEST(TestAllocator2, allocate1);
        FRIEND_TEST(TestAllocator2, allocate2);
        FRIEND_TEST(TestAllocator2, allocate3);

        FRIEND_TEST(TestAllocator2, valid1);
        FRIEND_TEST(TestAllocator2, valid2);
        FRIEND_TEST(TestAllocator2, valid3);
        FRIEND_TEST(TestAllocator2, valid4);

        int& operator [] (int i) {
            return *reinterpret_cast<int*>(&a[i]);
        }

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        Allocator () {//initializes sentinels 
            
            if ( N < ( sizeof(T) + (2 * sizeof(int)) ) )
                throw std::bad_alloc ();

            int sen = N - 8;
            
            (*this)[0] = sen;
            (*this)[N-4] = sen;
    
            assert(valid());
        }

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * throw a bad_alloc exception, if n is invalid
         */
        pointer allocate (size_type n) {//finds first fit 
            
            if ((n * sizeof(T)) > (N - 8))
                throw std::bad_alloc ();
            
            int i = 0;          //i is position of the first sentinel
            int sen = 0;        //value of sentinel
            bool ex = false;    //do we need to allocate more space than the user wants?
            
            while (i < N){
                sen = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]); //legacy code
                
                if (sen <= 0 ){      //check if it's occupied or free
                    i = i + 8 + abs( sen );
                    continue;
                }

                else if (sen == (n * sizeof(T) ) ){     //wooohooo, it's a perfect fit
                    break;
                }

                else if (sen < (n * sizeof(T) ) ){  //check if block is free, but not big enough
                    i = i + 8 + abs( sen );
                    continue;       
                }
                else if ( (sen - (n * sizeof(T) ) ) < ( sizeof(T) + 8 ) ){  //check if it's free, but we need to give user more space                    
                    ex = true;
                    break;
                }
                else{       //it fits!
                    break;
                }
            }

            // by this point, i is either the end of the heap or the address to a valid block        
            if (i == N)
                throw std::bad_alloc ();

            if(sen == (n * sizeof(T)) ){        //perfect fit, just change pos to neg
                (*this)[i] = -(*this)[i];
                (*this)[i + sen + 4] = (*this)[i]; 
            }

            else {      //not a perfect fit
                if(ex){
                
                    (*this)[i] = - (n * sizeof(T) ) - (sen - (n * sizeof(T) ) );
                    (*this)[i + 4 + - (*this)[i] ] = - (n * sizeof(T) ) - (sen - (n * sizeof(T) ) );
                }
                else{

                    (*this)[i] = - (n * sizeof(T) );                         //change 1st sen
                    (*this)[i + 4 + (n * sizeof(T)) ] = - (n * sizeof(T) ) ;  //change 2nd sen

                    int x = sen - 8 - (n * sizeof(T) );

                    (*this)[i + 8 + (n * sizeof(T)) ] = x;  //change 3rd sen
                    
                    //vv, code works, but not sure why we're skipping 3 ints instead of tw, maybe i just had too much coffee?
                    (*this)[i + 12 + (n * sizeof(T)) + x] = x;  //change 4th sen
                }
            }
            assert(valid());
            //we're returning the address to the data, not the sentinel!
            pointer pi = (pointer)(*this)[i+4];     //casting to a pointer type
            return pi;
           }             

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());
        }                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid
         * deallocate takes an occupied block and frees it, will throw an exception in case the given pointer does not point to the first sentinel of an occupied block
         */
        void deallocate (pointer p, size_type) {
            
            bool senbfr = false, senafr = false;        //true is the sentinels are free
            int sen = 0;
           
            if( (*p < 4) || ( *p > (N-sizeof(int)-sizeof(T)) ) ){
                throw std::invalid_argument( "received neg value of p " );
            }
                
            p = p - sizeof(int);
            sen = -(*this)[*p];         //sen will always be positive after this point

            if (sen < 0){
                throw std::invalid_argument( "received pos value of sen, which means it's already free ");
            }
            if (*p > 4){        // not at beginning 
                //read the sentinel before it and set senbfr
                int senb = (*this)[*p - sizeof(int) ];
                
                if (senb > 0)
                    senbfr = true;
            }
            if ( (N - *p - sen - (2 * sizeof(int))) > 0 ){       //not at the end
                //read the sentinel after it and set senafr
                int sena = (*this)[*p + sen + (2 * sizeof(int))];
                if (sena > 0)
                    senafr = true;
            }

            if(senbfr && senafr){
                
                int sena = (*this)[ (*p) + (2* sizeof(int)) + sen];         //value of sen after
                int senb = (*this)[ (*p) - sizeof(int)];         //value of sen before
                int sen_new = sen + senb + sena + 16;           //value of the new sentinels (combined block)

                (*this)[ (*p) - (2 * sizeof(int)) - senb ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (2 * sizeof(int)) + sen + sena + 8 ] = sen_new;    //changing the 6th/last sen
            }
            else if (senbfr && !senafr){    //coalesce with prev chunk

                int senb = (*this)[(*p) - sizeof(int)];         //value of sen before
                int sen_new = sen + senb + 8;                   //value of the new sentinels (combined block)

                (*this)[ (*p) - (2 * sizeof(int)) - senb ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (2 * sizeof(int)) + sen ] = sen_new;    //changing the 4th/last sen
            }
            else if (!senbfr && senafr){    //coalesce with next chunk
                
                int sena = (*this)[ (*p) + (2* sizeof(int)) + sen];         //value of sen after
                int sen_new = abs(sen) + sena + 8;           //value of the new sentinels (combined block)

                (*this)[ (*p) ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (sizeof(int)) + sen_new ] = sen_new;    //changing the 4th/last sen
            }
            else{
                (*this)[*p] = -(*this)[*p];
                (*this)[*p + sen + 4] = -(*this)[*p];
            }
            assert(valid());
        }

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());
        }

        /**
         * O(1) in space
         * O(1) in time
         * interprets bytes to ints from the heap
         */
        const int& operator [] (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);
        }
};

    //takes an it and return the byte representation of it (small endian)
    vector<unsigned char> intToBytes(int z){
         vector<unsigned char> v (4);
         for (int i = 0; i < 4; i++)
             v[3 - i] = (z >> (i * 8));
         return v;
    }


#endif // Allocator_h