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
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument

using namespace std;
// ---------
// Allocator
// ---------


//allocator<double, 100> x;
//double* p = x.allocate(3); // sometimes gives you more than what you are asking for
//…
//x.deallocate(p); // frees a block and maybe coalesces it

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
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

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
         * Basically, this version checks that the sentinels give correct values, doesn't check for the validity of the data, also checks that the first and last sentinels match
         */
        bool valid () const {

            int i = 0;  //i is position/value of the sentinel
            int sen1 = 0, sen2 = 0;
            

            while (i < N){
                sen1 = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]);
                i = i + 4 + abs( (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]));
                sen2 = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]);
                i += 4;

                if (sen1 != sen2)
                    return false;
            }

            if (i == N)
                return true;
            else
                return false;
        }

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
        FRIEND_TEST(TestAllocator2, index);
        int& operator [] (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        Allocator () {//sets sentinels 
            //(*this)[0] = 0; // replace!
            try{

              if ( N < ( sizeof(T) + (2 * sizeof(int)) ) )
                throw std::bad_alloc ();
            s1 = - ( sizeof(T) + (2 * sizeof(int)) );
            s2 = s1;
            
            for (int d = 0; d<4; d++)//s1
                a[d] = (s1 >> (8*d)) & 0xff;

            for (int d = 4; d<0; d--)//s2
                a[N - d] = (s2 >> (8*d)) & 0xff;


            }
            catch (bad_alloc&){
                throw;
            }
            catch( ... ){

            }

            // initialize the variables !!
                
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

            if ((n * sizeof(T)) < (N - 8))
                return null;        //or throw an exception?

            
            
            int i = 0;  //i is position of the first sentinel
            int sen = 0; //value of sentinel
            

            while (i < N){
                sen = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]);

                if (sen < 0 ){      //check if it's occupied  //what about zero?!
                    i = i + 8 + abs( sen );
                    continue;       //test this ?!
                }

                else if (sen == (n * sizeof(T) ) ){     //perfect fit
                    break;
                }
                else if (sen < (n * sizeof(T) ) ){  //check if it's free, but not enough space
                    i = i + 8 + abs( sen );
                    continue;       
                }
                else if ( (sen - (n * sizeof(T) ) ) < ( sizeof(T) + 8 ) ){  //check if it's free, but not enough space v2
                    i = i + 8 + abs( sen );
                    continue;     
                }
                else{       //it fits!
                    break;
                }
            }
            // i is either the end of the heap or a the address to a valid block
            if (i == N)
                throw std::bad_alloc ();

            // i is a valid block
            sen = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]); //is this correct? 

            if(sen == (n * sizeof(T)) ){        //perfect fit, change pos to neg
                (*this)[i] = -(*this)[i];
                (*this)[i + sen + 4] = -(*this)[i];
            }
            else {      //not a perfect fit
                (*this)[i] = - (n * sizeof(T) );                         //change 1st sen
                (*this)[i + 4 + (n * sizeof(T)) ] = - (n * sizeof(T) ) ;  //change 2nd sen

                int x = sen - 8 - (n * sizeof(T) );

                (*this)[i + 8 + (n * sizeof(T)) ] = x;  //change 3rd sen
                (*this)[i + 8 + (n * sizeof(T)) + x] = x;  //change 4th sen
            }
            

            assert(valid());
            return nullptr;}             // replace!

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid
         * <your documentation>
         */
        void deallocate (pointer p, size_type) {
            // <your code>
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        const int& operator [] (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

vector<unsigned char> intToBytes(int paramInt)
{
     vector<unsigned char> arrayOfByte(4);
     for (int i = 0; i < 4; i++)
         arrayOfByte[3 - i] = (paramInt >> (i * 8));
     return arrayOfByte;
}

#endif // Allocator_h
