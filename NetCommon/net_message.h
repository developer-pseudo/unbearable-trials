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
	}
}