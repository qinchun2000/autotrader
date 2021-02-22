// Copyright 2016 MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdlib>
#include <memory>

#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>


#ifndef MONGOACCESS_HPP_
#define MONGOACCESS_HPP_



class MongoAccess {
   public:
    static MongoAccess& instance() ;

    void configure(std::unique_ptr<mongocxx::instance> instance,
                   std::unique_ptr<mongocxx::pool> pool) ;

    using connection = mongocxx::pool::entry;

    connection get_connection() ;

    bsoncxx::stdx::optional<connection> try_get_connection() ;

   private:
    mongo_access() = default;

    std::unique_ptr<mongocxx::instance> _instance = nullptr;
    std::unique_ptr<mongocxx::pool> _pool = nullptr;


};


#endif /* MONGOACCESS_HPP_ */

