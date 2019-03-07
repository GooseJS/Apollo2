#include "Apollo/World/tileEntities/TileEntity.h"

namespace Apollo
{
	class TileEntityChest : public TileEntity
	{
	private:
		int storedValue = 0;
	public:
		TileEntityChest(BlockPos pos);

		virtual void tick() override { AP_INFO("Ticking chest!"); }

		inline void setStoredValue(int value) { storedValue = value; }
		inline int getStoredValue() const { return storedValue; }
	};
}