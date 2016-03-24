// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"
#include <iostream>
#include <new>  
#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
}
TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);

}



TEST(TestAllocator2, Allocator1) {// bad alloc Allocator, from not initializing w/ enough space 
   int verifier = 0; 
     try{
        Allocator<char,6 > x;     

     }
     catch(std::bad_alloc ia){
        //assert(0);
       
        verifier++;
     }
     ASSERT_EQ(verifier,1);

}

TEST(TestAllocator2, Allocator2 ) {// setting Sentinels
     Allocator<double, 50 > x;
    ASSERT_EQ(x[0],42);

}

TEST(TestAllocator2, Allocator3 ){
     Allocator<char, 127> x;
   // int y = x[0];
    ASSERT_EQ(x[0],119);
}



TEST(TestAllocator2, allocate1){// no fit , throwing bad alloc when heap full
      
       int verifier = 0;
        Allocator<int, 100> x;
         int* p = x.allocate(23); // 92 bytes
        try {
       int* q =  x.allocate(1);  
     }
     catch(std::bad_alloc ia){
        verifier++;
     }
     ASSERT_EQ(verifier,1);

}

TEST(TestAllocator2, allocate2){// bad alloc, value in allocate too large 
   int verifier = 0;
    Allocator<int, 100> x;
     try {
    int * p = x.allocate(25);
    }
     catch(std::bad_alloc ia){
        verifier++;
     }
     ASSERT_EQ(verifier,1);

}

TEST(TestAllocator2, allocate3) {// i guess first fit 
    Allocator<int, 100> x;
    int a = x[0]; //wtf is this
    ASSERT_EQ(a, 92);
    int *p = x.allocate(3);
   
   ASSERT_EQ(x[0],-12);

}




TEST(TestAllocator2, valid1) {

    Allocator<int, 100> x;
     x.allocate(10);
    ASSERT_EQ(x.valid(), true);
    
}


TEST(TestAllocator2, valid2) {

    Allocator<int, 100> x;
     int* p = x.allocate(10);
       ASSERT_EQ(x.valid(), true);
       x.allocate(5);
        ASSERT_EQ(x.valid(), true);
    
}


TEST(TestAllocator2, valid3) {

    Allocator<int, 100> x;
      x.allocate(2);
      ASSERT_EQ(x.valid(), true);
      x.allocate(3);
    ASSERT_EQ(x.valid(), true);
    x.allocate(1);
    ASSERT_EQ(x.valid(), true);

} 



TEST(TestAllocator2, deallocate1){//deallocate , coallesce after 
     Allocator<int, 100> x;
    int* p = x.allocate(4);   
    int* q = x.allocate(5);

        if (p != nullptr) {
        x.deallocate(p,  4);
        ASSERT_EQ(x[0],16);
         x.deallocate(q, 5);
        ASSERT_EQ(x[0],92); 
     }
}

TEST(TestAllocator2, deallocate2){//deallocate, coallesce before 
     Allocator<int, 100> x;
    int* p = x.allocate(4);   
    int* q = x.allocate(5);

        if (q != nullptr) {
        x.deallocate(q, 4);
        ASSERT_EQ(x[24],68);
        x.deallocate(p, 5);
        ASSERT_EQ(x[0],92); 
     }
}

TEST(TestAllocator2, deallocate3) { //catching an exception 
   int verifier = 0; 
   int z = -10;
   int* zp = &z;

     Allocator<int, 100> x;
     if (zp != nullptr){

     try{
        x.deallocate(zp,1);

     }
     catch(std::invalid_argument& e){
        //assert(0);
       
        verifier++;
     }
     ASSERT_EQ(verifier,1);
 }


}



// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;  
    const value_type     v = 2;  
    const pointer        b = x.allocate(s); 
    if (b != nullptr) {
        pointer e = b + s;// 
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

