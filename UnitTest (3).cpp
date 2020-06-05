#include "pch.h"
#include "CppUnitTest.h"
#include "Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(NewVertex_contains)
		{
			Graph test;
			test.NewVertex("1");
			test.NewVertex("2");
			test.NewVertex("3");
			test.NewVertex("4");
			Assert::AreEqual(false, test.contains("6"));
			Assert::AreEqual(true, test.contains("3"));
			test.NewVertex("6");
			Assert::AreEqual(true, test.contains("6"));
			try
			{
				test.NewVertex("3");
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual(ex.what(), "Attempt to add a new dot with the same name");
			}
		}

		TEST_METHOD(newEdge_FF)
		{
			Graph test;
			test.NewVertex("1");
			test.NewVertex("2");
			test.NewVertex("3");
			test.NewVertex("4");
			float compare = 0;
			Assert::AreEqual(compare, test.FF(0, 1));
			test.newEdge("1", "2", 5);
			compare = 5;
			Assert::AreEqual(compare, test.FF(0, 1));
			try
			{
				test.FF(10, 1);
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual(ex.what(), "Out of range");
			}
			test.newEdge("1", "4", 10);
			test.newEdge("4", "2", 20);
			test.newEdge("2", "3", 90);
			test.newEdge("3", "4", 100);
			compare = 15;
			Assert::AreEqual(compare, test.FF(0, 1));
			compare = 20;
			Assert::AreEqual(compare, test.FF(2, 1));
			try
			{
				test.FF("5", "1");
			}
			catch (const std::exception& ex)
			{
				Assert::AreEqual(ex.what(), "Couldn't find such dot");
			}
		}

		TEST_METHOD(fileDoesntOpenTest)
		{
			Graph test;
			try
			{
				test.fileRead("input.txt");
			}
			catch (invalid_argument error)
			{
				Assert::AreEqual("error file doesnt open!", error.what());
			}
		}

	};
}
