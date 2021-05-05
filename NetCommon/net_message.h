#pragma once

#include "net_common.h"

namespace unbearabletrials {
	namespace net {
		// message header is sent at the start of every message, this template allows us
		// to use an enum to ensure that the message is valid at compile time
		template<typename T>
		struct message_header {
			T id{};
			uint32_t size = 0;
		};

		template<typename T>
		struct message {
			message_header<T> header{};
			std::vector<uint8_t> body;

			size_t size() const {
				return sizeof(message_header<T>) + body.size();
			}

			// << operator overwrite using friend
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
				os << "ID: " << int(msg.header.id) << " Size:" << msg.header.size;
				return os;
			}

			//push any pod-like data to our message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data) {
				// check if our data being pushed is actually copyable
				static_assert(std::is_standard_layout<DataType>::value, "data is too complex to be pushed into a vector");

				// cache the size of our vector
				size_t i = msg.body.size();

				// resize our vector by the size of our data
				msg.body.resize(msg.body.size() + sizeof(DataType));

				// copy our data to our newly allocated vector 
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				// recalculate our message size
				msg.header.size = msg.size();

				// return our message so it can be chained
				return msg;
			}
		};
	}
}