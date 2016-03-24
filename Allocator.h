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
#include <iostream>


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
            
            cout << "Valid () \n";
            while (i < N){
                cout <<"A: sen1 = " << sen1 << ", sen2 = " << sen2;
                cout << "\ni = " << i << ", N = " << N;

                sen1 = (*this)[i];
                cout <<"\nB: sen1 = " << sen1 << " at i = " << i;

                i = i + 4 + abs(sen1);
                sen2 = (*this)[i];
                cout <<"\tsen2 = " << sen2 << " at i = " << i;

                i += 4;

                cout << "\ni = " << i << ", N = " << N << "\n\n";

                if (sen1 != sen2){

                    cout <<"valid() returning false because \nsen1 = " << sen1 << ", sen2 = " << sen2;
                    cout << "\ni = " << i << ", N = " << N << endl;
                    return false;
                }
            }
            


            if (i == N){
                cout <<"valid() returning true with i = " << i << ", N = " << N << "\n\n"<<endl;
                return true;
            }
            else{
                cout <<"valid() returning false because i = " << i << ", N = " << N << endl;
                return false;
            }
        }

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
        FRIEND_TEST(TestAllocator2, index);
        FRIEND_TEST(TestAllocator2, index);
        FRIEND_TEST(TestAllocator11, index);

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
            cout << "Allocator() with N = " << N << "\n";
            try{

              if ( N < ( sizeof(T) + (2 * sizeof(int)) ) )
                throw std::bad_alloc ();
            int sen = N - 8;
            
           
            /*

            s1 = ( sizeof(T) + (2 * sizeof(int)) );
            s2 = s1;
            
            for (int d = 0; d<4; d++)//s1
                a[d] = (s1 >> (8*d)) & 0xff;

            

            for (int d = 4; d<0; d--)//s2
                a[N - d] = (s2 >> (8*d)) & 0xff;
            */
            

            (*this)[0] = sen;
            (*this)[N-4] = sen;

            cout << "Printing s1: " << sen << endl;
            cout << "Printing i: " << (*this)[0] << endl;
            cout << "Printing j: " << (*this)[N - 4] << endl;

            }
            catch (bad_alloc& ia) {
                std::cerr << "Invalid argument: " << N << " " << ia.what() << '\n';

            }
            catch( ... ){
                //what to do here ?!
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
            cout << "allocate() with n = " << n << "\n";

            try{
                if ((n * sizeof(T)) > (N - 8))
                    throw std::bad_alloc ();
            }
            catch (bad_alloc& ia) {
                std::cerr << "EXCEPTION 1 allocate () with N = " << N << " n = " << n << " s = " << sizeof(T) << "\n" << (n * sizeof(T)) << " " << (N - 8) << "\n" << ia.what() << '\n';

            }
            catch( ... ){
                //what to do here ?!
            }

            
            int i = 0;  //i is position of the first sentinel
            int sen = 0; //value of sentinel
            
            while (i < N){
                sen = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]);
                cout << "while: i = " << i << " sen = " << sen;
                if (sen <= 0 ){      //check if it's occupied  //what about zero?!
                    cout << " block1\n";
                    i = i + 8 + abs( sen );
                    continue;       //test this ?!
                }

                else if (sen == (n * sizeof(T) ) ){     //perfect fit
                    cout << " block2\n";
                    break;
                }
                
                else if (sen < (n * sizeof(T) ) ){  //check if it's free, but not enough space
                    cout << " block3\n";
                    i = i + 8 + abs( sen );
                    continue;       
                }
                else if ( (sen - (n * sizeof(T) ) ) < ( sizeof(T) + 8 ) ){  //check if it's free, but not enough space v2
                    cout << " block4\n";
                    i = i + 8 + abs( sen );
                    continue;     
                }
                else{       //it fits!
                    cout << " block5\n";
                    break;
                }
            }
            // i is either the end of the heap or the address to a valid block
           
            //try{
                if (i == N)
                    throw std::bad_alloc ();
            //}
            /*
            catch (bad_alloc& ia) {
                std::cerr << "EXCEPTION 2 allocate () with N = " << N << " i = " << i << " n = " << n << " s = " << sizeof(T) << "\n" << (n * sizeof(T)) << " " << (N - 8) << "\n" << ia.what() << '\n';

            }
            catch( ... ){
                //what to do here ?!
            }
            */
            // i is a valid block
            sen = (a[i+3] << 24) | (a[i+2] << 16) | (a[i+1] << 8) | (a[i]); //is this correct? 

            if(sen == (n * sizeof(T)) ){        //perfect fit, change pos to neg
                cout << "not a perfect fit\n"; 
                (*this)[i] = -(*this)[i];
                (*this)[i + sen + 4] = -(*this)[i];
            }
            else {      //not a perfect fit
                cout << "not a perfect fit\n"; 
                (*this)[i] = - (n * sizeof(T) );                         //change 1st sen
                (*this)[i + 4 + (n * sizeof(T)) ] = - (n * sizeof(T) ) ;  //change 2nd sen

                cout << "sen 1 = " <<  (n * sizeof(T) ) << " at i = " << i << "\n";
                cout << "sen 2 = " <<  (n * sizeof(T) ) << " at i = " << i + 4 + (n * sizeof(T)) << "\n";

                int x = sen - 8 - (n * sizeof(T) );

                cout << "x = " << x << "\n"; 

                (*this)[i + 8 + (n * sizeof(T)) ] = x;  //change 3rd sen
                
                //vv, changed 12 to 8, not sure if mistake or not ?!
                (*this)[i + 12 + (n * sizeof(T)) + x] = x;  //change 4th sen

                

                cout << "sen 3 = " << x << " at i = " << i + 8 + (n * sizeof(T)) << "\n";
                cout << "sen 4 = " << x << " at i = " << i + 12 + (n * sizeof(T)) + x << "\n";
            }

            assert(valid());
                                            //we're returning the address to the data, not the sentinel!
            pointer pi = (pointer)&(*this)[i+4];     //totally gussed this one, maybeeee won't work
            return pi;

           // return nullptr;                      // return a pointer containg the address of i
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

            cout << "deallocate() with p = " << *p << " and (*this)[*p] = " << (*this)[*p]<< "\n";
            
            bool senbfr = false, senafr = false;        //true is the sentinels are free
            int sen = 0;
            //try{
                p = p - sizeof(int);
                sen = -(*this)[*p];         //sen will always be positive after this point
                cout << "deallocate() with p = " << *p << " and (*this)[*p] = " << (*this)[*p]<< "\n";

                if( (*p < 4) && ( *p > (N-sizeof(int)-sizeof(T)) ) )
                    throw std::invalid_argument( "received neg value of p " );
                if (sen < 0)
                    throw std::invalid_argument( "received pos value of sen, which means it's already free ");

           // }
            /*
            catch (const std::invalid_argument& ia) {
                sen = -sen;
                std::cerr << "Invalid argument: " <<  ia.what() << " sen = "<< sen <<'\n';
            }
            catch( ... ){
                //what to do here ?!
            }*/
            cout << "*p = " << *p << " sen = " << sen;
            if (*p > 4){        // not at beginning 
                //read the sentinel before it and set senbfr
                int senb = (*this)[*p - sizeof(int) ];
                
                if (senb > 0)
                    senbfr = true;
                cout << " senb = " << senb << " senbfr = " << senbfr;
            }

            if ( (N - *p - sen - (2 * sizeof(int))) > 0 ){       //not at the end
                //read the sentinel after it and set senafr
                int sena = (*this)[*p + sen + (2 * sizeof(int))];
                if (sena > 0)
                    senafr = true;
                cout << " sena = " << sena << " senafr = " << senafr << "\n";
                
            }

            if(senbfr && senafr){
                cout << "Block 1"  << "\n";
                int sena = (*this)[ (*p) + (2* sizeof(int)) + sen];         //value of sen after
                int senb = (*this)[ (*p) - sizeof(int)];         //value of sen before
                int sen_new = sen + senb + sena + 16;           //value of the new sentinels (combined block)

                (*this)[ (*p) - (2 * sizeof(int)) - senb ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (2 * sizeof(int)) + sen + sena + 8 ] = sen_new;    //changing the 6th/last sen
            }
            else if (senbfr && !senafr){    //coalesce with prev chunk

                int senb = (*this)[(*p) - sizeof(int)];         //value of sen before
                int sen_new = sen + senb + 8;                   //value of the new sentinels (combined block)

                cout << "Block 2"  << "\n";
                (*this)[ (*p) - (2 * sizeof(int)) - senb ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (2 * sizeof(int)) + sen ] = sen_new;    //changing the 4th/last sen

            }
            else if (!senbfr && senafr){    //coalesce with next chunk
                cout << "Block 3"  << "\n";
                int sena = (*this)[ (*p) + (2* sizeof(int)) + sen];         //value of sen after

                int sen_new = abs(sen) + sena + 8;                   //value of the new sentinels (combined block)

                (*this)[ (*p) ] = sen_new;   //changing the 1st sen
                (*this)[ (*p) + (sizeof(int)) + sen_new ] = sen_new;    //changing the 4th/last sen

                cout << "1st sen =  " << (*this)[ (*p) ] << " at " << (*p);
                cout << " 4th sen =  " << (*this)[ (*p) + (2 * sizeof(int)) + sen_new ] << " at " << (*p) + (2 * sizeof(int)) + sen_new;
            }
            else{
                 cout << "Block 4"  << "\n";
                (*this)[*p] = -(*this)[*p];
                (*this)[*p + sen + 4] = -(*this)[*p];
            }

            cout << "deallocate() ends";
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
