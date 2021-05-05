#include <iostream>
#include <unbearabletrials_net.h>

enum class MessageTypes : uint32_t {
	FireBullet,
	MovePlayer
};

int main() {
	unbearabletrials::net::message<MessageTypes> msg;
	msg.header.id = MessageTypes::FireBullet;

	int a = 1;
	bool b = true;
	float c = 3.14159f;

	struct {
		float x;
		float y;
	} d[5];

	msg << a << b << c << d;

	a = 99;
	b = false;
	c = 99.0f;

	msg >> d >> c >> b >> a;

	return 0;
}