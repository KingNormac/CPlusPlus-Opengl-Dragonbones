#ifndef SLOTDATA_H
#define SLOTDATA_H

#include <string>
#include <vector>
#include "DisplayData.h"

namespace DragonBones {
	class SlotData {
	public:
		std::vector<DisplayData*> _displayDataList;
		std::string name;
		std::string parent;
		int zOrder;

		std::vector<DisplayData*> getDisplayDataList(){return _displayDataList;};

		SlotData() {
			zOrder = 0;
		}

		~SlotData() {
			for(int i=0; i<_displayDataList.size(); i++) {
				DisplayData* data = _displayDataList[i];
				if(data) {
					delete[] data;
				}
			}
			_displayDataList.clear();
		}

		void addDisplayData(DisplayData* displayData) {
			if(displayData) {
				_displayDataList.push_back(displayData);
			} else {
				printf("Trying to put bad displayData into SlotData \n");
			}
		}

		DisplayData* getDisplayData(std::string displayName) {
			for(int i=0;i<_displayDataList.size();i++) {
				DisplayData* data = _displayDataList[i];
				if(data) {
					if(data->name == displayName) {
						return data;
					}
				}
			}
			return 0;
		}

	};
};

#endif