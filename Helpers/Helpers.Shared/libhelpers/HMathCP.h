#pragma once

namespace H {
	// Math Cross Platform
	class MathCP {
	public:
		template <typename T>
		static int Sign(T val) {
			return (T(0) < val) - (val < T(0));
		}

		template<class T>
		static T Clamp(T value, T min, T max) {
			T res = (std::min)((std::max)(min, value), max);
			return res;
		}

		template<class T>
		static T Lerp(T start, T end, T t) {
			T res = start + (end - start) * t;
			return res;
		}

		// round up for power of two multiples
		template<class T> static T RoundUpPOT(T numToRound, T potMultiple) {
			T res = (numToRound + potMultiple - 1) & ~(potMultiple - 1);
			return res;
		}

		template<class T> static T RoundUpPOT(T v) {
			v--;
			v |= v >> 1;
			v |= v >> 2;
			v |= v >> 4;
			v |= v >> 8;
			v |= v >> 16;
			v++;
			return v;
		}

		// Using round up +0.5
		template<class T> static T Convert(T value, T valueUnits, T dstUnits) {
			/*T tmp = value * dstUnits;
			T res = (tmp / valueUnits) + !!(tmp % valueUnits);*/

			/*T tmp = value * dstUnits * byteSize;
			T res = (tmp + (valueUnits / 2)) / (valueUnits * byteSize);*/


			T tmp = value * dstUnits;
			T tmp2 = valueUnits / 2;
			T res = (tmp + tmp2) / valueUnits;


			/*double ratio = (double) dstUnits / (double) valueUnits;
			T res = (T)((double) value * ratio);*/

			/*T tmp = value * dstUnits;
			T res = (tmp) / valueUnits;*/

			return res;
		}

		// Drop fractional part
		template<class T> static T Convert2(T value, T valueUnits, T dstUnits) {
			/*T tmp = value * dstUnits;
			T res = (tmp / valueUnits) + !!(tmp % valueUnits);*/

			/*T tmp = value * dstUnits * byteSize;
			T res = (tmp + (valueUnits / 2)) / (valueUnits * byteSize);*/

			T tmp = value * dstUnits;
			T res = (tmp) / valueUnits;


			/*double ratio = (double) dstUnits / (double) valueUnits;
			T res = (T)((double) value * ratio);*/

			/*T tmp = value * dstUnits;
			T res = (tmp) / valueUnits;*/

			return res;
		}

		/*
		start - from what unit item starts
		end - start + length.
		Value at end is a point right after the last inclusive unit of the item and not included in the item.
		Example:
		start = 0
		length = 1
		end = start + length = 1
		Item has length of 1 unit, starting from 0 unit.
		0---item---1---empty---2---
		000000000001111111111112222
		Item exists only on 0s range. When 1s range starts there is no item.
		0s range can be also interpreted as bits or bytes of items data.
		*/
		template<class T> static const bool IntersectRange(
			const T &start0, const T &end0,
			const T &start1, const T &end1)
		{
			// check intersections of items with each other
			// start or/and end of one item can be inside another item and vice versa
			bool intersects =
				(start0 >= start1 && start0 < end1 || end0 > start1 && end0 <= end1) ||
				(start1 >= start0 && start1 < end0 || end1 > start0 && end1 <= end0);

			return intersects;
		}

		template<class T> static const bool IntersectRangeLength(
			const T &start0, const T &length0,
			const T &start1, const T &length1)
		{
			T end0 = start0 + length0;
			T end1 = start1 + length1;

			return Math::IntersectRange(start0, end0, start1, end1);
		}
	};
}