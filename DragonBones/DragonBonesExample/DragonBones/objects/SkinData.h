#ifndef SKINDATA_H
#define SKINDATA_H

#include "SlotData.h"
#include <string>
#include <vector>

namespace DragonBones {
	class SkinData {
	public:
		std::vector<SlotData*> _slotDataList;
		std::string name;
		//std::vector<SlotData*> getSlotDataList(){return _slotDataList;}

		SkinData() { name="";}

		~SkinData() {
			for(SlotData* data : _slotDataList) {
				delete[] data;
			}
			_slotDataList.clear();
		}

		SlotData* getSlotData(std::string slotName) {
			for(SlotData* data : _slotDataList) {
				if(data) {
					if(data->name== slotName) {
						return data;
					}
				}
			}
			//printf("USED! \n");
			return 0;
		}

		void addSlotData(SlotData* slotData) {
			if(slotData) {
				_slotDataList.push_back(slotData);
			} else {
				printf("Tried to enter bad slotData into skinData \n");
			}
		}
	};
};

#endif