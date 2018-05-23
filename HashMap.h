// HashMap
// 11.01.2018
// Bartlomiej Kulik

#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include <list>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
    
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:

    std::list<value_type>* hashTablePtr;

    const static size_type SIZEHASHTABLE = 997;
    size_type size;

    void prepareHashMap()
    {
        this->size = 0;
        this->hashTablePtr = new std::list<value_type>[this->SIZEHASHTABLE];
    }

    size_type stdHash(const key_type& key) const
    {
        return std::hash<key_type> {} (key)%this->SIZEHASHTABLE;
    }

public:

    HashMap()
    {
        prepareHashMap();
    }

    ~HashMap()
    {
        if(this->hashTablePtr != nullptr)
        {
            delete[] this->hashTablePtr;
        }
    }

    HashMap(std::initializer_list<value_type> list)
    {
        prepareHashMap();
        this->size = list.size();

        for (auto i = list.begin(); i < list.end(); ++i)
        {
            size_type index = stdHash(i->first);
            this->hashTablePtr[index].push_back(*i);
        }
    }

    HashMap(const HashMap& other)
    {
        prepareHashMap();
        this->size = other.size;

        for (auto i = other.begin(); i != other.end(); ++i)
        {
            size_type index = stdHash(i->first);
            this->hashTablePtr[index].push_back(*i);
        }
    }

    HashMap(HashMap&& other)
    {
        this->hashTablePtr = other.hashTablePtr;
        this->size = other.size;

        other.hashTablePtr = nullptr;
        other.size = 0;
    }

    HashMap& operator=(const HashMap& other)
    {

        if(*this == other)
        {
            return *this;
        }

        delete[] this->hashTablePtr;
        prepareHashMap();

        this->size = other.size;

        for (auto i = other.begin(); i != other.end(); ++i)
        {
            size_type index = stdHash(i->first);
            this->hashTablePtr[index].push_back(*i);
        }

        return *this; // after copying
    }

    HashMap& operator=(HashMap&& other)
    {
        if(*this == other)
        {
            return *this;
        }

        delete[] this->hashTablePtr;

        this->hashTablePtr = other.hashTablePtr;
        this->size = other.size;

        other.hashTablePtr = nullptr;
        other.size = 0;

        return *this;
    }

    bool isEmpty() const
    {
        return this->size == 0;
    }

    mapped_type& operator[](const key_type& key)
    {
        size_type index = stdHash(key);

        for (auto i = this->hashTablePtr[index].begin(); i != this->hashTablePtr[index].end(); ++i) // each list element
        {
            if (i->first == key) // if succes
            {
                return i->second;
            }

        }

        // if not succes
        value_type newPair = std::make_pair(key, mapped_type {});
        this->hashTablePtr[index].push_back(newPair);
        ++this->size;

        return (*(--this->hashTablePtr[index].end())).second;

    }

    const mapped_type& valueOf(const key_type& key) const
    {
        size_type index = stdHash(key);

        for (auto i = this->hashTablePtr[index].begin(); i != this->hashTablePtr[index].end(); ++i)
        {
            if (i->first == key)
            {
                return i->second;
            }

        }

        throw std::out_of_range("const mapped_type& valueOf(const key_type& key) const: No key value");

    }

    mapped_type& valueOf(const key_type& key)
    {
        size_type index = stdHash(key);

        for (auto i = this->hashTablePtr[index].begin(); i != this->hashTablePtr[index].end(); ++i)
        {
            if (i->first == key)
            {
                return i->second;
            }

        }

        throw std::out_of_range("mapped_type& valueOf(const key_type& key): No key value");


    }

    const_iterator find(const key_type& key) const
    {
        size_type index = stdHash(key);

        for (auto i = this->hashTablePtr[index].begin(); i != this->hashTablePtr[index].end(); ++i)
        {
            if (i->first == key)
            {
                return const_iterator(*this, index, i);
            }
        }

        return end(); // if not find

    }

    iterator find(const key_type& key)
    {
        size_type index = stdHash(key);

        for (auto i = this->hashTablePtr[index].begin(); i != this->hashTablePtr[index].end(); ++i)
        {
            if (i->first == key)
            {
                return iterator(*this, index, i); // i - hashTableElementIterator
            }

        }

        return end();

    }

    void remove(const key_type& key)
    {
        remove(find(key));
    }

    void remove(const const_iterator& it)
    {
        if(it == end())
        {
            throw std::out_of_range("void remove(const const_iterator& it): end element");
        }
        size_type index = stdHash(it->first);

        this->hashTablePtr[index].erase(it.hashTableElementIterator); // erase hashTableElementIterator

        --this->size;
    }

    size_type getSize() const
    {
        return this->size;
    }

    bool operator==(const HashMap& other) const
    {
        // return other.hashTablePtr == hashTablePtr; NO!

        if(this->size != other.size)
        {
            return false;
        }

        for (auto i = begin(); i != end(); ++i) // for each element of our hasTable
        {
            auto found = other.find(i->first); // found - iterator
            if(found == other.end() || i->second != found->second) // found is end() or problem wit mapped_type
            {
                return false;
            }
        }

        return true; // if no problem
    }

    bool operator!=(const HashMap& other) const
    {
        return !(*this == other);
    }

    iterator begin()
    {
        return cbegin();
    }

    iterator end()
    {
        return cend();
    }

    const_iterator cbegin() const
    {
        if(this->size == 0)
        {
            return cend();
        }

        size_type index = 0;
        while(this->hashTablePtr[index].size() == 0)
        {
            ++index;
        }
        return const_iterator(*this, index, this->hashTablePtr[index].begin());
    }

    const_iterator cend() const
    {
        return const_iterator(*this, this->SIZEHASHTABLE - 1, this->hashTablePtr[this->SIZEHASHTABLE - 1].end());
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }

};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using pointer = const typename HashMap::value_type*;

    const HashMap<KeyType, ValueType>& hashMapRef;
    size_type index;
    typename std::list<value_type>::iterator hashTableElementIterator;

    explicit ConstIterator(const HashMap<KeyType, ValueType>& referenceToHashMap, const size_type& indexInHashTable,
                           const typename std::list<value_type>::iterator& iteratorInHashTableElement)
        : hashMapRef(referenceToHashMap), index(indexInHashTable), hashTableElementIterator(iteratorInHashTableElement) {}

    ConstIterator(const ConstIterator& other)
        : hashMapRef(other.hashMapRef), index(other.index), hashTableElementIterator(other.hashTableElementIterator) {}

    ConstIterator& operator++()
    {
        if(this->hashTableElementIterator == this->hashMapRef.hashTablePtr[hashMapRef.SIZEHASHTABLE - 1].end()) // if end element
        {
            throw std::out_of_range("ConstIterator& operator++(): end element");
        }

        ++this->hashTableElementIterator;
        if(this->hashTableElementIterator == this->hashMapRef.hashTablePtr[index].end())
        {
            do
            {
                this->index++;
            }
            while(this->index < this->hashMapRef.SIZEHASHTABLE && this->hashMapRef.hashTablePtr[index].size() == 0);

            // now we have good index value

            if (this->index == this->hashMapRef.SIZEHASHTABLE)
            {
                --this->index;
                this->hashTableElementIterator = this->hashMapRef.hashTablePtr[index].end();
                return *this;
            }

            this->hashTableElementIterator = this->hashMapRef.hashTablePtr[this->index].begin();

            return *this;
        }

        return *this;
    }

    ConstIterator operator++(int)
    {
        auto toReturn = *this;
        ++(*this);
        return toReturn;
    }

    ConstIterator& operator--()
    {
        if(this->hashTableElementIterator == this->hashMapRef.hashTablePtr[0].begin())
        {
            throw std::out_of_range("ConstIterator& operator--(): --begin()?");
        }

        if(this->hashTableElementIterator != this->hashMapRef.hashTablePtr[this->index].begin())
        {
            --this->hashTableElementIterator;
            return *this;
        }
        // if not
        do
        {
            --this->index;
        }
        while(this->index > 0 && this->hashMapRef.hashTablePtr[this->index].size() == 0);

        if(this->hashMapRef.hashTablePtr[index].size() == 0)
        {
            throw std::out_of_range("ConstIterator& operator--(): lack od -- element");
        }
        // else
        this->hashTableElementIterator = --this->hashMapRef.hashTablePtr[this->index].end();

        return *this;

    }

    ConstIterator operator--(int)
    {
        auto toReturn = *this;
        --(*this);
        return toReturn;
    }

    reference operator*() const
    {
        if(this->hashTableElementIterator == this->hashMapRef.hashTablePtr[hashMapRef.SIZEHASHTABLE - 1].end()) // *this == hashMapRef.end()
        {
            throw std::out_of_range("reference operator*() const: end element");
        }

        return *this->hashTableElementIterator;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {

        return (this->hashTableElementIterator == other.hashTableElementIterator && this->index == other.index);

    }

    bool operator!=(const ConstIterator& other) const
    {
        return !(*this == other);
    }

};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::value_type*;

    explicit Iterator(const HashMap<KeyType, ValueType>& ref, const size_type& ind, const typename std::list<value_type>::iterator& iter): ConstIterator(ref, ind, iter)
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
