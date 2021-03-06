#include <set>
#include <list>
#include <vector>

#include <iostream>
#include <iterator>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
concept Equality_comparable = requires (T a, T b)
{
    {a == b} -> bool;
    {a != b} -> bool;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T, typename U>
concept Same = std::is_same<T, U>::value;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T, typename U>
concept Equality_comparable_types = requires (T t, U u)
{
	{t == u} -> bool;
	{u == t} -> bool;
	{t != u} -> bool;
	{u != t} -> bool;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<typename I>
using iterator_category_t =
    typename std::iterator_traits<I>::iterator_category;


template <typename I, typename It_Category>
concept Not_same_category_as = std::is_same<
	iterator_category_t<I>, It_Category
		>::value == false;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// The value_type of a class is a member type
template<typename T>
struct value_type;

template<typename T>
using value_type_t = typename value_type<T>::type;


// The value_type of a class is a member type
template<typename T>
requires requires {typename T::value_type;}
struct value_type<T>
{
    using type = typename T::value_type;
};

// The value_type of a pointer is the type of element pointed to.
template<typename T>
struct value_type<T*>
{
    using type = T;
};

// The value_type of an array is its element type.
template<typename T, int N>
struct value_type<T[N]>
{
    using type = T;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename I>
concept Input_iterator = requires(I i)
{
	// Must have a value type
	typename value_type_t<I>;

	// It can be dereferenced
	{ *i } -> const value_type_t<I>&;

	// It can be incremented
	{ ++i } -> I&;        
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Input_iterator I>
void advance (I& iter, int n)
{
    while (n != 0)
    {
        ++iter;
        n--;
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Input_iterator I>
int distance (I first, I limit)
{
    int n = 0;
    for(; first != limit; ++first, ++n)
    {
        ;
    }
    return n;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename I>
concept Forward_iterator = 
	Input_iterator<I> && requires (I i)
{
	typename iterator_category_t<I>;
	requires Not_same_category_as<
			I, std::input_iterator_tag>;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename I>
concept Bidirectional_iterator = 
	Forward_iterator<I> && requires (I i)
{
	 // It can be incremented
	{ --i } -> I&;        
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Bidirectional_iterator I>
void advance (I& iter, int n)
{
    if (n > 0)
        while (n != 0) { ++iter; --n; }
    if (n < 0)
        while (n != 0) { --iter; ++n; }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename I>
concept Random_access_iterator = 
Bidirectional_iterator<I> && requires (I i, int n)
{
	{ i += n } -> I&;
	{ i -= n } -> I&;
	{ i - i } -> int;        
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Random_access_iterator I>
void advance (I& iter, int n)
{
    iter += n;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Random_access_iterator I>
int distance (I first, I limit)
{
    return limit - first;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<Forward_iterator I, typename T>
    requires Same<T, value_type_t<I>>
bool binary_search(I first, I limit, T const& value)
{
    if (first == limit)
        return false;

    auto mid = first;
    advance (mid, distance(first, limit) / 2);
    if (value < *mid)
        return binary_search(first, mid, value);
    else if (*mid < value)
        return binary_search(++mid, limit, value);
    else 
        return true;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <Input_iterator I, typename T>
	requires Same<T, value_type_t<I>>
void save_to_vector(I first, I limit, std::vector<T>& v)
{
	for(;first != limit; ++first)
	{
		v.push_back(*first);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <Forward_iterator I, typename T>
	requires Same<T, value_type_t<I>>
void save_to_vector(I first, I limit, std::vector<T>& v)
{
	v.reserve(distance(first, limit));
	v.insert(v.begin(), first, limit);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main()
{
    const int value = 5;
	bool bs_found;

//-----------------------------------------------------------------------------
	std::istream_iterator<int> cin_it(std::cin);
	std::istream_iterator<int> eos;

	std::vector<int> v_sink_input;
	save_to_vector(cin_it, eos, v_sink_input);
    bs_found = binary_search(v_sink_input.begin(), v_sink_input.end(), value);
	std::cout << std::endl;
    std::cout << std::boolalpha <<
        "binary search in vector sink input it found = " << bs_found << std::endl;

//-----------------------------------------------------------------------------
    std::list<int> l_source = {1,2,4,5,6,7};

	std::vector<int> v_sink_forward;
	save_to_vector(l_source.begin(), l_source.end(), v_sink_forward);
    bs_found = binary_search(v_sink_forward.begin(), v_sink_forward.end(), value);
    std::cout << std::boolalpha <<
        "binary search in vector sink forward it found = " << bs_found << std::endl;

//-----------------------------------------------------------------------------    
    std::list<int> l = {1,2,4,5,6,7};
    bs_found = binary_search(l.begin(), l.end(), value);
    std::cout << std::boolalpha <<
        "binary search list found = " << bs_found << std::endl;

//-----------------------------------------------------------------------------
    int array[] = {1,2,4,5,6,7};
    const size_t len = sizeof(array) / sizeof(int);
    bs_found = binary_search(array, array + len, value);
    std::cout << std::boolalpha <<
        "binary search array found = " << bs_found << std::endl;

//-----------------------------------------------------------------------------    
    std::vector<int> vect = {1,2,4,5,6,7};
    bs_found = binary_search(vect.begin(), vect.end(), value);
    std::cout << std::boolalpha <<
        "binary search vector found = " << bs_found << std::endl;

    return 0;
}

