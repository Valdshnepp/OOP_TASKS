#include "pch.h"
#include "../circularBuffer/circularBuffer.h"

#include <crtdbg.h>
struct CrtCheckMemory
{
	_CrtMemState state1;
	_CrtMemState state2;
	_CrtMemState state3;
	CrtCheckMemory()
	{
		_CrtMemCheckpoint(&state1);
	}
	~CrtCheckMemory()
	{
		_CrtMemCheckpoint(&state2);
		// using google test you can just do this.
		EXPECT_EQ(0, _CrtMemDifference(&state3, &state1, &state2));
		// else just do this to dump the leaked blocks to stdout.
		if (_CrtMemDifference(&state3, &state1, &state2))
			_CrtMemDumpStatistics(&state3);
	}
};


TEST(bufConstructor, defaultConstr) {
	CrtCheckMemory check;

	CircularBuffer buf;

	EXPECT_TRUE(buf.size()==0);
	EXPECT_TRUE(buf.reserve()==0);
	EXPECT_TRUE(buf.capacity()==0);
	EXPECT_TRUE(buf.is_linearized()==true);
	EXPECT_TRUE(buf.full()==true);
	EXPECT_TRUE(buf.empty()==true);

	EXPECT_ANY_THROW(buf.front());
	EXPECT_ANY_THROW(buf.back());
	EXPECT_ANY_THROW(buf.insert(1));
	EXPECT_ANY_THROW(buf.at(0));
}

TEST(bufConstructor, capacityConstructor) {
	CrtCheckMemory check;

	CircularBuffer buf(5);

	EXPECT_TRUE(buf.size() == 0);
	EXPECT_TRUE(buf.reserve() == 5);
	EXPECT_TRUE(buf.capacity() == 5);
	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.full() == false);
	EXPECT_TRUE(buf.empty() == true);

	EXPECT_ANY_THROW(buf.front());
	EXPECT_ANY_THROW(buf.back());
	EXPECT_ANY_THROW(buf.insert(1));

	buf.push_back('a');

	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf.reserve() == 4);
	EXPECT_TRUE(buf.capacity() == 5);
	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.full() == false);
	EXPECT_TRUE(buf.empty() == false);

	EXPECT_TRUE(buf.front() == 'a');
	EXPECT_TRUE(buf.back() == 'a');
}

TEST(bufConstructor, capacityConstructorWithDefaultValues) {
	CrtCheckMemory check;

	CircularBuffer buf(5,'a');

	EXPECT_TRUE(buf.size() == 5);
	EXPECT_TRUE(buf.reserve() == 0);
	EXPECT_TRUE(buf.capacity() == 5);
	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.full() == true);
	EXPECT_TRUE(buf.empty() == false);

	buf.push_back('b');

	EXPECT_TRUE(buf.size() == 5);
	EXPECT_TRUE(buf.reserve() == 0);
	EXPECT_TRUE(buf.capacity() == 5);
	EXPECT_TRUE(buf.is_linearized() == false);
	EXPECT_TRUE(buf.full() == true);
	EXPECT_TRUE(buf.empty() == false);

	EXPECT_TRUE(buf.front() == 'a');
	EXPECT_TRUE(buf.back() == 'b');
}

TEST(bufConstructor, copyConstructor) {
	CrtCheckMemory check;

	CircularBuffer buf(5, 'a');
	buf.push_back('b');

	CircularBuffer bufCopy(buf);

	EXPECT_TRUE(bufCopy.size() == 5);
	EXPECT_TRUE(bufCopy.reserve() == 0);
	EXPECT_TRUE(bufCopy.capacity() == 5);
	EXPECT_TRUE(bufCopy.is_linearized() == false);
	EXPECT_TRUE(bufCopy.full() == true);
	EXPECT_TRUE(bufCopy.empty() == false);
				   
	EXPECT_TRUE(bufCopy.front() == 'a');
	EXPECT_TRUE(bufCopy.back() == 'b');
}

TEST(bufConstructor, AssignOperator) {
	CrtCheckMemory check;

	CircularBuffer buf(5, 'a');
	buf.push_back('b');

	CircularBuffer bufCopy = buf;

	EXPECT_TRUE(bufCopy.size() == 5);
	EXPECT_TRUE(bufCopy.reserve() == 0);
	EXPECT_TRUE(bufCopy.capacity() == 5);
	EXPECT_TRUE(bufCopy.is_linearized() == false);
	EXPECT_TRUE(bufCopy.full() == true);
	EXPECT_TRUE(bufCopy.empty() == false);

	EXPECT_TRUE(bufCopy.front() == 'a');
	EXPECT_TRUE(bufCopy.back() == 'b');
}

TEST(inputFunctions, push_back) {
	CrtCheckMemory check;

	CircularBuffer buf1;
	buf1.push_back('2');
	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size()==0);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);
	buf.push_back('d');
	buf.push_back('e');
	buf.push_back('f');
	EXPECT_TRUE(buf[0] == 'd');
	EXPECT_TRUE(buf[1] == 'e');
	EXPECT_TRUE(buf[2] == 'f');
}

TEST(inputFunctions, push_front) {
	CrtCheckMemory check;

	CircularBuffer buf1;
	buf1.push_front('2');
	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_front('a');
	buf.push_front('b');
	buf.push_front('c');
	EXPECT_TRUE(buf[0] == 'c');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'a');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);
	buf.push_front('d');
	buf.push_front('e');
	buf.push_front('f');
	EXPECT_TRUE(buf[0] == 'f');
	EXPECT_TRUE(buf[1] == 'e');
	EXPECT_TRUE(buf[2] == 'd');
}

TEST(inputFunctions, insert) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_back('a');
	buf.push_back('a');
	buf.push_back('a');
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'a');
	EXPECT_TRUE(buf[2] == 'a');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);

	buf.insert(0,'d');
	buf.insert(1,'e');
	buf.insert(2,'f');
	EXPECT_TRUE(buf[0] == 'd');
	EXPECT_TRUE(buf[1] == 'e');
	EXPECT_TRUE(buf[2] == 'f');

	buf.push_back('a');
	buf.insert(0, 'z');
	EXPECT_TRUE(buf[0] == 'z');
}

TEST(deleteFunctions, pop_back) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');

	EXPECT_TRUE(buf[0] == 'b');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_TRUE(buf[2] == 'd');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);

	buf.pop_back();

	EXPECT_TRUE(buf[0] == 'b');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_ANY_THROW(buf.at(2));
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 2);

	buf.pop_back();
	buf.pop_back();
	buf.pop_back();

	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 0);
}

TEST(deleteFunctions, pop_front) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');

	EXPECT_TRUE(buf[0] == 'b');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_TRUE(buf[2] == 'd');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);

	buf.pop_front();

	EXPECT_TRUE(buf[0] == 'c');
	EXPECT_TRUE(buf[1] == 'd');
	EXPECT_ANY_THROW(buf.at(2));
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 2);

	buf.pop_back();
	buf.pop_back();
	buf.pop_back();

	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 0);
}

TEST(etc, set_capacity){
	CrtCheckMemory check;

	CircularBuffer buf(1);
	buf.push_back('a');
	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf.capacity() == 1);
	buf.set_capacity(3);
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf[0] == 'a');
	buf.push_back('b');
	buf.push_back('c');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');

	buf.set_capacity(1);
	EXPECT_TRUE(buf.size() == 0);
	EXPECT_TRUE(buf.capacity() == 1);
	EXPECT_ANY_THROW(buf.at(0));
}

TEST(etc, resize) {
	CrtCheckMemory check;

	CircularBuffer buf(1);
	buf.push_back('a');
	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf.capacity() == 1);
	buf.resize(3,'b');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'b');

	buf.resize(1);
	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_ANY_THROW(buf.at(1));

	buf.resize(2, 'c');
	EXPECT_TRUE(buf.size() == 2);
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_ANY_THROW(buf.at(2));
	
}

TEST(etc, linearize) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.linearize();
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);

	buf.push_back('d');
	EXPECT_TRUE(buf.is_linearized() == false);
	buf.linearize();
	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.capacity() == 3);
	EXPECT_TRUE(buf.size() == 3);
	EXPECT_TRUE(buf[0] == 'b');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_TRUE(buf[2] == 'd');
}

TEST(etc, linearize2) {
	CrtCheckMemory check;

	CircularBuffer buf(4);

	EXPECT_TRUE(buf.size() == 0);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.linearize();
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf.capacity() == 4);
	EXPECT_TRUE(buf.size() == 3);

	buf.push_back('d');
	buf.push_back('e');
	buf.push_back('f');
	buf.pop_back();
	EXPECT_TRUE(buf.is_linearized() == false);
	buf.linearize();
	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.capacity() == 4);
	EXPECT_TRUE(buf.size() == 3);
	EXPECT_TRUE(buf[0] == 'c');
	EXPECT_TRUE(buf[1] == 'd');
	EXPECT_TRUE(buf[2] == 'e');
}

TEST(etc, swap) {
	CrtCheckMemory check;

	CircularBuffer buf(1);
	buf.push_back('z');
	EXPECT_TRUE(buf.size() == 1);
	EXPECT_TRUE(buf.capacity() == 1);

	CircularBuffer buf1(3);
	EXPECT_TRUE(buf1.size() == 0);

	buf1.push_back('a');
	buf1.push_back('b');
	buf1.push_back('c');

	buf.swap(buf1);

	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf.size()==3);

	EXPECT_TRUE(buf1[0] == 'z');
	EXPECT_TRUE(buf1.size()==1);
}

TEST(etc, erase) {
	CrtCheckMemory check;

	CircularBuffer buf(5);
	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');
	buf.push_back('e');
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf[3] == 'd');
	EXPECT_TRUE(buf[4] == 'e');
	buf.erase(1, 4);

	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'e');
	EXPECT_ANY_THROW(buf.at(2));
	EXPECT_TRUE(buf.size()==2);
	EXPECT_TRUE(buf.capacity()==5);
}

TEST(etc, erase2) {
	CrtCheckMemory check;

	CircularBuffer buf(5);
	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');
	buf.push_back('e');
	buf.push_back('f');
	buf.push_back('j');
	EXPECT_TRUE(buf[0] == 'c');
	EXPECT_TRUE(buf[1] == 'd');
	EXPECT_TRUE(buf[2] == 'e');
	EXPECT_TRUE(buf[3] == 'f');
	EXPECT_TRUE(buf[4] == 'j');
	buf.erase(1, 4);

	EXPECT_TRUE(buf[0] == 'c');
	EXPECT_TRUE(buf[1] == 'j');
	EXPECT_ANY_THROW(buf.at(2));
	EXPECT_TRUE(buf.size() == 2);
	EXPECT_TRUE(buf.capacity() == 5);
}

TEST(etc, rotate) {
	CrtCheckMemory check;

	CircularBuffer buf(5);
	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');
	buf.push_back('e');
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf[3] == 'd');
	EXPECT_TRUE(buf[4] == 'e');
	buf.rotate(4);
	EXPECT_TRUE(buf[0] == 'e');
	EXPECT_TRUE(buf[1] == 'a');
	EXPECT_TRUE(buf[2] == 'b');
	EXPECT_TRUE(buf[3] == 'c');
	EXPECT_TRUE(buf[4] == 'd');
}

TEST(etc, rotate2) {
	CrtCheckMemory check;

	CircularBuffer buf(5);
	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');
	buf.push_back('d');
	EXPECT_TRUE(buf[0] == 'a');
	EXPECT_TRUE(buf[1] == 'b');
	EXPECT_TRUE(buf[2] == 'c');
	EXPECT_TRUE(buf[3] == 'd');
	buf.rotate(1);
	EXPECT_TRUE(buf[0] == 'b');
	EXPECT_TRUE(buf[1] == 'c');
	EXPECT_TRUE(buf[2] == 'd');
	EXPECT_TRUE(buf[3] == 'a');
}

TEST(etc, operEqual) {
	CrtCheckMemory check;

	CircularBuffer buf1(5);
	buf1.push_back('a');
	buf1.push_back('b');
	buf1.push_back('c');
	buf1.push_back('d');
	buf1.push_back('e');

	CircularBuffer buf2(5);
	buf2.push_back('a');
	buf2.push_back('b');
	buf2.push_back('c');
	buf2.push_back('d');
	buf2.push_back('e');
	
	EXPECT_TRUE(buf1 == buf2);
	buf2.push_back('z');
	EXPECT_TRUE(buf1 != buf2);
}

TEST(etc, constMethods) {
	CrtCheckMemory check;

	CircularBuffer buf1(5);
	buf1.push_back('a');
	buf1.push_back('b');
	buf1.push_back('c');
	buf1.push_back('d');
	buf1.push_back('e');

	const CircularBuffer buf2(buf1);

	buf1.push_back('z');
	EXPECT_TRUE(buf1.at(0)=='b');

	EXPECT_TRUE(buf2.at(0)=='a');
	EXPECT_FALSE(buf2.at(0)==buf1.at(0));
	EXPECT_ANY_THROW(buf2.at(-1));
	EXPECT_ANY_THROW(buf2.at(buf2.capacity()));

	EXPECT_TRUE(buf2[0]=='a');
	EXPECT_FALSE(buf2[0]== buf1[0]);

	EXPECT_TRUE(buf2.front()== 'a');
	EXPECT_TRUE(buf2.back()== 'e');

	EXPECT_TRUE(buf1 != buf2);
}

TEST(combinationsTest, combo1) {
	CrtCheckMemory check;

	CircularBuffer mybuf(2);
	EXPECT_TRUE(mybuf.size() == 0);
	EXPECT_TRUE(mybuf.capacity() == 2);

	mybuf.push_back('a');
	EXPECT_TRUE(mybuf.size() == 1);
	EXPECT_TRUE(mybuf[0] == 'a');

	mybuf.push_back('b');
	EXPECT_TRUE(mybuf.size() == 2);
	EXPECT_TRUE(mybuf[0] == 'a');
	EXPECT_TRUE(mybuf[1] == 'b');

	mybuf.push_back('c');
	EXPECT_TRUE(mybuf.size() == 2);
	EXPECT_TRUE(mybuf.at(0) == 'b');
	EXPECT_TRUE(mybuf.at(1) == 'c');

	mybuf.push_back('d');
	EXPECT_TRUE(mybuf.size() == 2);
	EXPECT_TRUE(mybuf[0] == 'c');
	EXPECT_TRUE(mybuf[1] == 'd');

	mybuf.clear();
	EXPECT_TRUE(mybuf.size() == 0);
}

TEST(combinationsTest, combo2) {
	CrtCheckMemory check;

	CircularBuffer mybuf(1);
	EXPECT_TRUE(mybuf.size() == 0);
	EXPECT_TRUE(mybuf.capacity() == 1);

	mybuf.push_back('a');
	EXPECT_TRUE(mybuf.size() == 1);
	EXPECT_TRUE(mybuf[0] == 'a');

	mybuf.push_back('b');
	EXPECT_TRUE(mybuf.size() == 1);
	EXPECT_TRUE(mybuf[0] == 'b');

	mybuf.push_back('c');
	EXPECT_TRUE(mybuf.size() == 1);
	EXPECT_TRUE(mybuf[0] == 'c');
}

TEST(combinationsTest, combo3) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');

	EXPECT_TRUE(buf.is_linearized() == true);
	EXPECT_TRUE(buf.front() == 'a');
	EXPECT_TRUE(buf.back() == 'c');

	buf.push_back('d');
	EXPECT_TRUE(buf.is_linearized() == false);

	EXPECT_TRUE(buf.front() == 'b');
	EXPECT_TRUE(buf.back() == 'd');

	value_type& foo = buf.at(0);
	foo = 'z';

	EXPECT_TRUE(buf[0] == 'z');
}

TEST(combinationsTest, combo4) {
	CrtCheckMemory check;

	CircularBuffer buf(3);

	buf.push_back('a');
	buf.push_back('b');
	buf.push_back('c');

	const CircularBuffer boo(buf);
	const value_type& foo = boo.at(0);

	EXPECT_TRUE(boo.front() == 'a');
	EXPECT_TRUE(boo.back() == 'c');

}