/*
 * =============================================================================
 *
 *       Filename:  Date.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2012 02:36:34 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_DATE_H
#define MUDUO_BASE_DATE_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

struct tm;

namespace muduo 
{
///
/// Date in Gregorian calendar.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Date : public muduo::copyable
{
	public :
		struct YearMonthDay
		{
			int year;
			int month;
			int day;
		};

		static const int kDaysPerWeek = 7;

		Date() : julianDayNumber_(0) {}

		Date(int year, int month, int day);

		explicit Date(int julianDayNum) 
			: julianDayNumber_(julianDayNum)
		{}

		explicit Date(const struct tm&);

		void swap(Date& that)
		{
			std::swap(julianDayNumber_, that.julianDayNumber_);
		}

		bool valid() const { return julianDayNumber_ > 0; }

		string toIsoString() const;

		struct YearMonthDay yearMonthDay() const;

		int year() const 
		{
			return yearMonthDay().year;
		}

		int month() const 
		{
			return yearMonthDay().month;
		}

		int day() const 
		{
			return yearMonthDay().day;
		}

		int weekDay() const 
		{
			return (julianDayNumber_ + 1) % kDaysPerWeek;
		}

		int julianDayNumber() const { return julianDayNumber_;}

	private :
		int julianDayNumber_;
};

inline bool operator<(Date x, Date y)
{
	return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y)
{
	return x.julianDayNumber() == y.julianDayNumber();
}

}

#endif
