#include "mongoaccess.hpp"


  static mongo_access& MongoAccess::instance() {
        static mongo_access instance;
        return instance;
    }

    void MongoAccess::configure(std::unique_ptr<mongocxx::instance> instance,
                   std::unique_ptr<mongocxx::pool> pool) {
        _instance = std::move(instance);
        _pool = std::move(pool);
    }

    using connection = mongocxx::pool::entry;

    connection MongoAccess::get_connection() {
        return _pool->acquire();
    }

    bsoncxx::stdx::optional<connection> try_get_connection() {
        return _pool->try_acquire();
    }


void MongoAccess::configure(mongocxx::uri uri) {
    class noop_logger : public mongocxx::logger {
       public:
        virtual void operator()(mongocxx::log_level,
                                bsoncxx::stdx::string_view,
                                bsoncxx::stdx::string_view) noexcept {}
    };

    auto instance =
        bsoncxx::stdx::make_unique<mongocxx::instance>(bsoncxx::stdx::make_unique<noop_logger>());

    mongo_access::instance().configure(std::move(instance),
                                       bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));
}

bool MongoAccess::do_work() {
    auto connection = mongo_access::instance().get_connection();
    if (!connection)
        return false;
    return true;
}
