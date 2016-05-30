/*
 * Copyright (c) 2011-2016 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin-protocol.
 *
 * libbitcoin-protocol is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) 
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_PROTOCOL_ZMQ_POLLER_HPP
#define LIBBITCOIN_PROTOCOL_ZMQ_POLLER_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <bitcoin/protocol/define.hpp>
#include <bitcoin/protocol/zmq/socket.hpp>

namespace libbitcoin {
namespace protocol {
namespace zmq {

/// This class is thread safe (including dynamic configuration).
class BCP_API poller
  : public enable_shared_from_base<poller>
{
public:
    /// A shared poller pointer.
    typedef std::shared_ptr<poller> ptr;

    /// Construct an empty poller (sockets must be added).
    poller();

    /// This class is not copyable.
    poller(const poller&) = delete;
    void operator=(const poller&) = delete;

    /// True if the timeout occurred.
    bool expired() const;

    /// True if the connection is closed.
    bool terminated() const;

    /// Add a socket to be polled (not thread safe).
    void add(socket& sock);

    /// Remove all sockets from the poller. Must not be in wait call.
    void clear();

    /// Wait specified milliseconds for any socket to receive, -1 is forever.
    socket::identifier wait(int32_t timeout_milliseconds);

private:
    // zmq_pollitem_t alias, keeps zmq.h out of our headers.
    typedef struct
    {
        void* socket;
        file_descriptor fd;
        short events;
        short revents;
    } zmq_pollitem;

    typedef std::vector<zmq_pollitem> pollers;

    // These values are protected by mutex.
    bool expired_;
    bool terminated_;
    pollers pollers_;
    mutable shared_mutex mutex_;
};

} // namespace zmq
} // namespace protocol
} // namespace libbitcoin

#endif