#ifndef ITERABLE_STACK_H
#define ITERABLE_STACK_H

#include <vector>

template<typename T>
class ItStack
{
	public:
		T& emplace(T&& value)
		{
			data.emplace_back(value);

			return data.back();
		}	
	
		T& emplace(T& value)
		{
			data.emplace_back(value);

			return data.back();
		}

		T& top()
		{
			return *(data.end() - 1);
		}

		T& pop()
		{
			T value = std::move(*(data.end() - 1));
			data.erase(data.end() - 1);

			return value;
		}

		size_t size()
		{
			return data.size();
		}

		bool empty()
		{
			return data.size() == 0;
		}

		T& operator[] (size_t index)
		{
			return data[index];
		}

	private:
		std::vector<T> data;
};

#endif