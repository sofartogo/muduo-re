/*
 * =============================================================================
 *
 *       Filename:  Timestamp.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/2012 04:39:26 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef TUDUO_BASE_TIMESTAMP_H
#define TUDUO_BASE_TIMESTAMP_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

#include <boost/operators.hpp>

namespace muduo 
{

///
/// Time stamp in UTC, in microseconds resolution.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Timestamp: public muduo::copyable,
				 public boost::less_than_comparable<Timestamp>
{
	public :
		Timestamp()
			: microSecondsSinceEpoch_(0)
		{
		}

		explicit Timestamp(int64_t microSecondsSinceEpoch);

		void swap(Timestamp& that)
		{
			std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
		}

		string toString() const;
		string toFormattedString() const;

		bool valid() const {return microSecondsSinceEpoch_ > 0; }

		int64_t microSecondsSinceEpoch() const 
		{ return microSecondsSinceEpoch_; }

		static Timestamp now();
		static Timestamp invalid();

		static const int kMicroSecondsPerSecond = 1000 * 1000;

	private :
		int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microseciond
/// resolution for next 100 years.
inline double timeDifference(Timestamp high, Timestamp low)
{
	int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}
}



#endif
