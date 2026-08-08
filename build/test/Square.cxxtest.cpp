/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

bool suite_TestSquare_init = false;
#include "/home/ubuntu/cs3520-mini-project-samuel/test/Square.cxxtest.hpp"

static TestSquare suite_TestSquare;

static CxxTest::List Tests_TestSquare = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSquare( "test/Square.cxxtest.hpp", 4, "TestSquare", suite_TestSquare, Tests_TestSquare );

static class TestDescription_suite_TestSquare_testDefaultConstructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testDefaultConstructor() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 7, "testDefaultConstructor" ) {}
 void runTest() { suite_TestSquare.testDefaultConstructor(); }
} testDescription_suite_TestSquare_testDefaultConstructor;

static class TestDescription_suite_TestSquare_testConstructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testConstructor() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 13, "testConstructor" ) {}
 void runTest() { suite_TestSquare.testConstructor(); }
} testDescription_suite_TestSquare_testConstructor;

static class TestDescription_suite_TestSquare_testAdd : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testAdd() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 18, "testAdd" ) {}
 void runTest() { suite_TestSquare.testAdd(); }
} testDescription_suite_TestSquare_testAdd;

static class TestDescription_suite_TestSquare_testEquals : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testEquals() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 22, "testEquals" ) {}
 void runTest() { suite_TestSquare.testEquals(); }
} testDescription_suite_TestSquare_testEquals;

static class TestDescription_suite_TestSquare_testNotEquals : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testNotEquals() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 27, "testNotEquals" ) {}
 void runTest() { suite_TestSquare.testNotEquals(); }
} testDescription_suite_TestSquare_testNotEquals;

static class TestDescription_suite_TestSquare_testLessThan : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSquare_testLessThan() : CxxTest::RealTestDescription( Tests_TestSquare, suiteDescription_TestSquare, 32, "testLessThan" ) {}
 void runTest() { suite_TestSquare.testLessThan(); }
} testDescription_suite_TestSquare_testLessThan;

