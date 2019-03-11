#ifndef GATT_CHARACTERISTIC_WIN_H
#define GATT_CHARACTERISTIC_WIN_H

#include <vector>

namespace Neuro {

class GattCharacteristicWin final {
public:
	std::vector<unsigned char> read() const;
	void write(const std::vector<unsigned char> &);
	ListenerPtr<void, const std::vector<Byte> &>
		subscribeCharacteristicChanged(const std::function<void(const std::vector<Byte> &)> &);
private:

};

}

#endif // GATT_CHARACTERISTIC_WIN_H
