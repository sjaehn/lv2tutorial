#ifndef BUTILITIES_BMAP_HPP_
#define BUTILITIES_BMAP_HPP_

#include <array>
#include <utility>

namespace BUtilities
{

/**
 *  @brief A realtime safe partial implementation of the std::map container 
 *  based on std::array. BMap is made up of (key,value) pairs, which can be 
 *  retrieved based on a key. BMap has got a fixed maximal size.
 *
 *  @tparam _Key  Type of key objects. Must support comparison operators.
 *  @tparam  _Tp  Type of mapped objects.
 *  @tparam  _Nm  Maximal number of elements. At least 1.
 *
 *  Meets the requirements of a <a href="tables.html#65">container</a>, a
 *  <a href="tables.html#66">reversible container</a>, and an
 *  <a href="tables.html#69">associative container</a> (using unique keys).
 *  For a @c BMap<Key,T> the key_type is Key, the mapped_type is T, and the
 *  value_type is std::pair<const Key,T>.
 *
 *  BMaps support random access iterators as std::array did.
 */
template <typename _Key, typename _Tp, std::size_t _Nm>
class BMap : protected std::array <std::pair<_Key, _Tp>, _Nm>
{
public:
    typedef _Key key_type;
    typedef _Tp mapped_type;
    typedef std::pair<_Key, _Tp> value_type;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    enum Error
    {
        BMAP_NO_ERROR   = 0,
        BMAP_EMPTY,
        BMAP_FULL,
        BMAP_INVALID_INDEX,
        BMAP_INVALID_KEY
    };

protected:
    std::size_t _size;
    Error _err;

public:

    // Constructors
    BMap () :
        _size (0),
        _err (BMAP_NO_ERROR)
    {}


    // Iterators.

    _GLIBCXX17_CONSTEXPR iterator begin() noexcept
    { return iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data()); }

    _GLIBCXX17_CONSTEXPR const_iterator begin() const noexcept
    { return const_iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data()); }

    _GLIBCXX17_CONSTEXPR iterator end() noexcept
    { return iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data() + _size); }

    _GLIBCXX17_CONSTEXPR const_iterator end() const noexcept
    { return const_iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data() + _size); }

    _GLIBCXX17_CONSTEXPR reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    _GLIBCXX17_CONSTEXPR const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    _GLIBCXX17_CONSTEXPR reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    _GLIBCXX17_CONSTEXPR const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    _GLIBCXX17_CONSTEXPR const_iterator cbegin() const noexcept
    { return const_iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data()); }

    _GLIBCXX17_CONSTEXPR const_iterator cend() const noexcept
    { return const_iterator(std::array <std::pair<_Key, _Tp>, _Nm>::data() + _size); }

    _GLIBCXX17_CONSTEXPR const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    _GLIBCXX17_CONSTEXPR const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator(begin()); }


    // Capacity.

    constexpr size_type size() const noexcept 
    { return _size; }

    constexpr size_type max_size() const noexcept 
    { return _Nm; }

    _GLIBCXX_NODISCARD constexpr bool empty() const noexcept 
    { return size() == 0; }


    // Element access

    /**
     *  @brief  Indirect access to %BMap data via the subscript ( @c [] ) 
     *          and a key.
     *  @param  __k  The key for which data should be retrieved.
     *  @return  A reference to the data of the (key,data) %pair.
     *
     *  Allows for easy lookup with the subscript ( @c [] )
     *  operator.  Returns data associated with the key specified in
     *  subscript.  If the key does not exist, a pair with that key
     *  is created using default values, which is then returned. And
     *  if the reserved size limit is reached before, an error code
     *  is set and a reference to the last data element (or the
     *  undefifed first element in the case of size 0) is returned.
     *  @{
     */
    mapped_type& operator[] (const key_type& __k)
    {
        _err = BMAP_NO_ERROR;
        iterator it = begin ();
        for (/* empty */ ; it < end (); ++it)
        {
            if (__k == it->first) return (*it).second;

            if (__k < it->first) break;
        }

        // else append
        return insert (__k)->second;
    }

    const mapped_type& operator[] (const key_type& __k) const
    {
        _err = BMAP_NO_ERROR;
        iterator it = begin ();
        for (/* empty */ ; it < end (); ++it)
        {
            if (__k == it->first) return (*it).second;

            if (__k < it->first) break;
        }

        // else append
        return insert (__k)->second;
    }

    // @}

    /**
     *  @brief Attempts to insert a std::pair into the %BMap.
     *  @param  hint  An iterator that serves as a hint as to where the
     *                pair should be inserted.
     *  @param  value  Pair to be inserted (see std::make_pair for easy 
     *                 creation of pairs).
     *  @return An iterator that points to the element with key of
     *           @a value (may or may not be the %pair passed in).
     *
     *  This function tries to insert a value. If the key of the value
     *  %pair already exists, then the data will be replaced. Note that 
     *  the first parameter is only a hint and can potentially improve 
     *  the performance of the insertion process.  A bad hint would
     *  cause no gains in efficiency.
     *  @{
     */
    iterator insert (const iterator hint, const value_type& value)
    {
        _err = BMAP_NO_ERROR;
        iterator it = hint;
        for (/* empty */ ; it < end (); ++it)
        {
            // equal key: replace
            if (value.first == it->first)
            {
                (*it).second = value.second;
                return it;
            }

            // insert
            if (value.first < it->first) break;
        }

        // full
        if (_size >= _Nm)
        {
            _err = BMAP_FULL;
            return it;
        }

        // move and set
        std::copy_backward (it, end(), end() + 1);
        ++_size;
        *it = value;
        return it;
    }

    iterator insert (const value_type& value)
    {
        return insert (begin(), value);
    }

    iterator insert (const key_type& key)
    {
        return insert (begin(), value_type (key, mapped_type()));
    }

    /**
     *  @brief Erases an element from a %BMap.
     *  @param  position  An iterator pointing to the element to be erased.
     *  @return An iterator pointing to the element immediately following
     *          @a position prior to the element being erased. If no such
     *          element exists, end() is returned.
     *
     *  This function erases an element, pointed to by the given
     *  iterator, from a %BMap.
     * 
     *  @{
     */
    iterator erase (const_iterator position)
    { 
        _err = BMAP_NO_ERROR;
        if (_size == 0)
        {
            _err = BMAP_EMPTY;
            return end();
        }

        std::copy (position + 1, end(), position);
        --_size;
        return position;
    }

    iterator erase (iterator position)
    { 
        _err = BMAP_NO_ERROR;
        if (_size == 0)
        {
            _err = BMAP_EMPTY;
            return end();
        }

        std::copy (position + 1, end(), position);
        --_size;
        return position;
    }
    // @}

    /**
     *  @brief Erases elements according to the provided key.
     *  @param  key  Key of element to be erased.
     *  @return  The number of elements erased.
     *
     *  This function erases all the elements located by the given key from
     *  a %BMap.
     */
    size_type erase (const key_type& key)
    {
        const_iterator it = find (key);
        if (it == end()) return 0;
        
        erase (it);
        return 1;
    }

    /**
     *  Erases all elements in a %BMap simply by setting the size to 0.
     */
    void clear() noexcept
    {
        _err = BMAP_NO_ERROR;
        _size = 0;
    }


    // lookup
    /**
     *  @brief Tries to locate an element in a %BMap.
     *  @param  key  Key of (key, value) %pair to be located.
     *  @return  Iterator pointing to sought-after element, or end() if not
     *           found.
     *
     *  This function takes a key and tries to locate the element with which
     *  the key matches.  If successful the function returns an iterator
     *  pointing to the sought after %pair.  If unsuccessful it returns the
     *  past-the-end ( @c end() ) iterator.
     *  @{
     */
    iterator find (const key_type& key)
    {
        iterator it = begin();
        for (/* empty */ ; it < end (); ++it)
        {
            if (key == it->first) return it;
            if (key < it->first) break;
        }

        return end();
    }

    const_iterator find (const key_type& key) const
    {
        const_iterator it = begin();
        for (/* empty */ ; it < end (); ++it)
        {
            if (key == it->first) return it;
            if (key < it->first) break;
        }

        return end();
    }

    // @}

    /**
     *  @brief  Finds the number of elements with given key.
     *  @param  key  Key of (key, value) pairs to be located.
     *  @return  Number of elements with specified key.
     *
     *  The result will either be 0 (not present) or 1 (present).
     */
    size_type count (const key_type& key) const
    {
        return (find (key) != end());
    }

    
    // error handling

    /**
     *  @brief  Returns the Error code for the last element access or 
     *          modifier method called.
     *  @return  Error code.
     */
    Error error() const noexcept
    {
        return _err;
    }


};

}

#endif /* BUTILITIES_BMAP_HPP_ */