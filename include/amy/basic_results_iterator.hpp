#ifndef __AMY_BASIC_RESULTS_ITERATOR_HPP__
#define __AMY_BASIC_RESULTS_ITERATOR_HPP__

#include <amy/result_set.hpp>

#include <boost/iterator/iterator_facade.hpp>

namespace amy {

template<typename MySQLService>
class basic_results_iterator :
    public boost::iterator_facade<
        basic_results_iterator<MySQLService>,
        result_set,
        boost::single_pass_traversal_tag
    >
{
    friend class boost::iterator_core_access;

private:
    typedef MySQLService service_type;

    typedef
        basic_results_iterator<service_type>
        this_type;

    typedef
        boost::iterator_facade<
            this_type,
            result_set,
            boost::single_pass_traversal_tag
        >
        base_type;

    typedef basic_connector<service_type> connector_type;

public:
    typedef typename base_type::reference reference;

    basic_results_iterator() :
        connector_(0),
        end_(true)
    {}

    basic_results_iterator(connector_type& connector) :
        connector_(&connector),
        end_(false)
    {
        if (connector_->is_open() && connector_->has_more_results()) {
            store_result();
            if (connector_->has_more_results()) {
                end_ = true;
            }
        }
        else {
            end_ = true;
        }
    }

private:
    connector_type* connector_;
    mutable result_set result_set_;
    bool end_;

    void increment() {
        if (!connector_->has_more_results()) {
            end_ = true;
        }
        else {
            store_result();
        }
    }

    bool equal(this_type const& other) const {
        return (end_ == other.end_) && (end_ || connector_ == other.connector_);
    }

    reference dereference() const {
        BOOST_ASSERT(!end_);
        return result_set_;
    }

    void store_result() {
        BOOST_ASSERT(connector_);
        result_set_ = connector_->store_result();
    }

};  //  class basic_results_iterator

}   //  namespace amy

#endif  //  __AMY_BASIC_RESULTS_ITERATOR_HPP__

// vim:ft=cpp ts=4 sw=4 et
