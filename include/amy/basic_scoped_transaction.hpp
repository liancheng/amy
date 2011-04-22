#ifndef __AMY_TRANSACTION_HPP__
#define __AMY_TRANSACTION_HPP__

#include <amy/basic_connector.hpp>

#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

namespace amy {

template<typename MySQLService>
class basic_scoped_transaction : private boost::noncopyable {
public:
    typedef basic_connector<MySQLService> connector_type;

    basic_scoped_transaction(connector_type& connector) :
        connector_(connector),
        committed_(false)
    {
        connector_.autocommit(false);
    }

    void commit() {
        connector_.commit();
        committed_ = true;
    }

	boost::system::error_code commit(boost::system::error_code& ec) {
		committed_ = !!connector_.commit(ec);
		return ec;
	}

	void rollback() {
		connector_.rollback();
	}

	boost::system::error_code rollback(boost::system::error_code& ec) {
		return connector_.rollback(ec);
	}

    ~basic_scoped_transaction() {
        if (!committed_) {
            boost::system::error_code ec;
            connector_.rollback(ec);
        }
        connector_.autocommit(true);
    }

private:
    connector_type& connector_;
    bool committed_;

};  //  class basic_scoped_transaction

}   //  namespace amy

#endif  //  __AMY_TRANSACTION_HPP__
